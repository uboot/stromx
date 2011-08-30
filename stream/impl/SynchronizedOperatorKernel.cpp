/* 
 *  Copyright 2011 Matthias Fuchs
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

#include "SynchronizedOperatorKernel.h"

#include "../OperatorKernel.h"
#include "../Exception.h"
#include "../Id2DataPair.h"
#include "../Data.h"
#include "../DataContainer.h"
#include "../OperatorException.h"

#include <boost/thread/thread.hpp>

namespace stream
{
    namespace impl
    {
        SynchronizedOperatorKernel::SynchronizedOperatorKernel(OperatorKernel* const op)
        : m_op(op),
            m_status(NONE)
        {
            if(!op)
                throw WrongArgument("Passed null pointer as operator.");
        }
        
        SynchronizedOperatorKernel::~SynchronizedOperatorKernel()
        {
            delete m_op;
        }
        
        void SynchronizedOperatorKernel::testForInterrupt()
        {
            try
            {
                boost::this_thread::interruption_point();
            }
            catch(boost::thread_interrupted&)
            {
                throw Interrupt();
            } 
        }
        
        void SynchronizedOperatorKernel::sleep(const unsigned int microseconds)
        {
            try
            {
                boost::this_thread::sleep(boost::posix_time::microseconds(1));
            }
            catch(boost::thread_interrupted&)
            {
                throw Interrupt();
            } 
        }
        
        void SynchronizedOperatorKernel::initialize()
        {
            lock_t lock(m_mutex);
            
            if(m_status != NONE)
                throw InvalidState("Operator has already been initialized.");
            
            m_op->initialize();
            
            m_inputMap = impl::Id2DataMap(m_op->inputs());
            m_outputMap = impl::Id2DataMap(m_op->outputs());
            
            BOOST_ASSERT(m_inputMap.isEmpty());
            BOOST_ASSERT(m_outputMap.isEmpty());
            
            m_status = INITIALIZED;
        }
        
        
        void SynchronizedOperatorKernel::activate()
        {
            lock_t lock(m_mutex);
            
            if(m_status != INITIALIZED)
                throw InvalidState("Operator must be initialized.");
            
            BOOST_ASSERT(m_inputMap.isEmpty());
            BOOST_ASSERT(m_outputMap.isEmpty());
            
            m_op->activate();
            m_status = ACTIVE;
        }
        
        void SynchronizedOperatorKernel::deactivate()
        {
            lock_t lock(m_mutex);
            
            if(m_status == INITIALIZED)
                return;
            
            if(m_status == EXECUTING)
                throw InvalidState("Operator can not be deactivated while it is executing.");
            
            m_op->deactivate();
            
            m_inputMap.clear();
            m_outputMap.clear();
            
            m_status = INITIALIZED;
        }
        
        const Data& SynchronizedOperatorKernel::getParameter(unsigned int id)
        {
            lock_t lock(m_executeMutex);
            
            validateParameterId(id);
            validateReadAccess(id);
            return m_op->getParameter(id);
        }
            
        void SynchronizedOperatorKernel::setParameter(unsigned int id, const Data& value)
        {
            validateParameterId(id);
            validateWriteAccess(id);
            validateParameterType(id, value.variant());
            
            DataVariant parameterType = info()->parameter(id).type();
            if(parameterType.is(DataVariant::TRIGGER))
            {
                m_op->setParameter(id, value);
            }
            else
            {
                lock_t lock(m_executeMutex);
                m_op->setParameter(id, value);
            }
        }

        void SynchronizedOperatorKernel::receiveInputData(const Id2DataMapper& mapper)
        {   
            BOOST_ASSERT(m_status == EXECUTING); // this function can only be called from OperatorKernel::execute()
            
            // This function is called from OperatorKernel::execute which again is called by SynchronizedOperatorKernel::execute().
            // SynchronizedOperatorKernel::execute() lock the mutex which is unlocked here.
            m_mutex.unlock();
            
            bool interruptExceptionWasThrown = false;
            
            {
                unique_lock_t lock(m_mutex);
                
                try
                {
                    while(! mapper.tryGet(m_inputMap))
                        waitForSignal(lock);
                    
                    mapper.get(m_inputMap);
                }
                catch(Interrupt&)
                {
                    interruptExceptionWasThrown = true;
                }   
                
                if(! interruptExceptionWasThrown)
                    m_cond.notify_all();
            }
            
            // lock again before entering OperatorKernel::execute()
            m_mutex.lock();
            
            // rethrow exception if necessary
            if(interruptExceptionWasThrown)
                throw Interrupt();
        }

        void SynchronizedOperatorKernel::sendOutputData(const stream::Id2DataMapper& mapper)
        {
            BOOST_ASSERT(m_status == EXECUTING); // this function can only be called from OperatorKernel::execute()
            
            // This function is called from OperatorKernel::execute which again is called by SynchronizedOperatorKernel::execute().
            // SynchronizedOperatorKernel::execute() lock the mutex which is unlocked here.
            m_mutex.unlock();
            
            bool interruptExceptionWasThrown = false;
            
            {
                unique_lock_t lock(m_mutex);
                
                try
                {
                    while(! mapper.trySet(m_outputMap))
                        waitForSignal(lock);
                    
                    mapper.set(m_outputMap);
                }
                catch(Interrupt&)
                {
                    interruptExceptionWasThrown = true;
                }   
                
                if(! interruptExceptionWasThrown)
                    m_cond.notify_all();
            }
            
            // lock again before entering OperatorKernel::execute()
            m_mutex.lock();
            
            // rethrow exception if necessary
            if(interruptExceptionWasThrown)
                throw Interrupt();   
        }
        
        DataContainer SynchronizedOperatorKernel::getOutputData(const unsigned int id)
        {
            unique_lock_t lock(m_mutex);
            
            if( m_status != ACTIVE
                && m_status != EXECUTING)
            {
                throw InvalidState("Can not get output data if operator is inactive.");
            }
        
            while(m_outputMap[id].empty())
            {
                // this section might throw an InterruptionException which will fall
                // through to the calling function
                if(m_status == ACTIVE)
                {
                    // try to get some output data by executing
                    execute();
                }
                else
                {
                    // wait for the situation to change
                    waitForSignal(lock);
                }
            }
            
            return m_outputMap[id];
        }

        void SynchronizedOperatorKernel::setInputData(const unsigned int id, DataContainer data)
        {
            unique_lock_t lock(m_mutex);
            
            if( m_status != ACTIVE
                && m_status != EXECUTING)
            {
                throw InvalidState("Can not get output data if operator is inactive.");
            }
        
            while(! m_inputMap[id].empty())
            {
                // this section might throw an InterruptionException which will fall
                // through to the calling function
                if(m_status == ACTIVE)
                {
                    // try to get some output data by executing
                    execute();
                }
                else
                {
                    // wait for the situation to change
                    waitForSignal(lock);
                }
            }
            
            m_inputMap[id] = data;
                
            m_cond.notify_all();
        }
        
        void SynchronizedOperatorKernel::clearOutputData(unsigned int id)
        {
            lock_t lock(m_mutex);
            
            m_outputMap[id] = DataContainer();
                
            m_cond.notify_all();
        }
        
        void SynchronizedOperatorKernel::execute()
        {
            // m_mutex is locked if this function is called
            m_status = EXECUTING;
            
            bool interruptExceptionWasThrown = false;
            
            try
            {
                lock_t lock(m_executeMutex);
                m_op->execute(*this);
            }
            catch(std::exception &e)
            {
                try
                {
                    dynamic_cast<Interrupt&>(e);
                    interruptExceptionWasThrown = true;
                }
                catch(std::bad_cast&)
                {
                }
            }
            
            m_status = ACTIVE;
            
            if(interruptExceptionWasThrown)
                throw Interrupt();
            
            // a signal is emitted in setInputData() and getOutputData()
        }
        
        void SynchronizedOperatorKernel::waitForSignal(unique_lock_t& lock)
        {
            try
            {
                m_cond.wait(lock);
            }
            catch(boost::thread_interrupted&)
            {
                throw Interrupt();
            } 
        }
        
        void SynchronizedOperatorKernel::validateParameterId(const unsigned int id)
        {
            bool isValid = false;
            for(std::vector<const Parameter*>::const_iterator iter = info()->parameters().begin();
                iter != info()->parameters().end();
                ++iter)
            {
                if((*iter)->id() == id)
                {
                    isValid = true;
                    break;
                }
            }
            
            if(! isValid)
                throw WrongParameterId(id, *this->info());
        }
        
        void SynchronizedOperatorKernel::validateReadAccess(const unsigned int id)
        {
            const Parameter& param = info()->parameter(id);
            
            switch(status())
            {
            case NONE:
                switch(param.accessMode())
                {
                case Parameter::NO_ACCESS:
                case Parameter::INITIALIZED_READ:
                case Parameter::INITIALIZED_WRITE:
                case Parameter::ACTIVATED_WRITE:
                    throw ParameterAccessViolation(param, *this->info());
                }
                break;
            case INITIALIZED:
                switch(param.accessMode())
                {
                case Parameter::NO_ACCESS:
                    throw ParameterAccessViolation(param, *this->info());
                }
                break;
            case ACTIVE:
            case EXECUTING:
                switch(param.accessMode())
                {
                case Parameter::NO_ACCESS:
                    throw ParameterAccessViolation(param, *this->info());
                }
                break;
            default:
                BOOST_ASSERT(false);    
            }
        }
        
        void SynchronizedOperatorKernel::validateWriteAccess(const unsigned int id)
        {
            const Parameter& param = info()->parameter(id);
            
            switch(status())
            {
            case NONE:
                switch(param.accessMode())
                {
                case Parameter::NO_ACCESS:
                case Parameter::NONE_READ:
                case Parameter::INITIALIZED_READ:
                case Parameter::INITIALIZED_WRITE:
                case Parameter::ACTIVATED_WRITE:
                    throw ParameterAccessViolation(param, *this->info());
                }
                break;
            case INITIALIZED:
                switch(param.accessMode())
                {
                case Parameter::NO_ACCESS:
                case Parameter::NONE_READ:
                case Parameter::NONE_WRITE:
                case Parameter::INITIALIZED_READ:
                    throw ParameterAccessViolation(param, *this->info());
                }
                break;
            case ACTIVE:
            case EXECUTING:
                switch(param.accessMode())
                {
                case Parameter::NO_ACCESS:
                case Parameter::NONE_READ:
                case Parameter::NONE_WRITE:
                case Parameter::INITIALIZED_READ:
                case Parameter::INITIALIZED_WRITE:
                    throw ParameterAccessViolation(param, *this->info());
                }
                break;
            default:
                BOOST_ASSERT(false);    
            }
        }
        
        void SynchronizedOperatorKernel::validateParameterType(const unsigned int id, const stream::DataVariant& type)
        {
            const Parameter& param = info()->parameter(id);
            if(! type.is(param.type()))
                throw WrongParameterType(param, *this->info());
        }  
    }
}
