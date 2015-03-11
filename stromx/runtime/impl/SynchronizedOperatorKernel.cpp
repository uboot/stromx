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

#include <boost/thread/thread.hpp>
#include "stromx/runtime/Data.h"
#include "stromx/runtime/DataContainer.h"
#include "stromx/runtime/Exception.h"
#include "stromx/runtime/Factory.h"
#include "stromx/runtime/Id2DataPair.h"
#include "stromx/runtime/OperatorException.h"
#include "stromx/runtime/OperatorKernel.h"
#include "stromx/runtime/impl/SynchronizedOperatorKernel.h"

namespace
{
    const stromx::runtime::Factory EMPTY_FACTORY;
}

namespace stromx
{
    namespace runtime
    {
        namespace impl
        {
            SynchronizedOperatorKernel::SynchronizedOperatorKernel(OperatorKernel* const op)
              : m_op(op),
                m_status(NONE),
                m_parametersAreLocked(false),
                m_factory(0)
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
                // allow parameter access while sleeping
                {
                    lock_t lock(m_mutex);
                    m_parametersAreLocked = false;
                }
                
                try
                {
                    boost::this_thread::sleep_for(boost::chrono::microseconds(microseconds));
                }
                catch(boost::thread_interrupted&)
                {
                    throw Interrupt();
                } 
                
                {
                    lock_t lock(m_mutex);
                    m_parametersAreLocked = true;
                }
            }
            
            void SynchronizedOperatorKernel::initialize(const Id2DataMapObserver* const inputObserver, const Id2DataMapObserver* const outputObserver)
            {
                lock_t lock(m_mutex);
                
                if(m_status != NONE)
                    throw WrongOperatorState(*info(), "Operator has already been initialized.");
                
                try
                {
                    m_op->initialize();
                }
                catch(OperatorError &)
                {
                    throw;
                }
                catch(std::exception & e)
                {
                    throw OperatorError(*info(), e.what());
                }
                
                m_inputMap = impl::Id2DataMap(m_op->inputs());
                m_inputMap.setObserver(inputObserver);
                m_outputMap = impl::Id2DataMap(m_op->outputs());
                m_outputMap.setObserver(outputObserver);
                
                BOOST_ASSERT(m_inputMap.empty());
                BOOST_ASSERT(m_outputMap.empty());
                
                m_status = INITIALIZED;
            }
            
            
            void SynchronizedOperatorKernel::activate()
            {
                lock_t lock(m_mutex);
                
                if(m_status != INITIALIZED)
                    throw WrongOperatorState(*info(), "Operator must be initialized.");
                
                BOOST_ASSERT(m_inputMap.empty());
                BOOST_ASSERT(m_outputMap.empty());
                
                try
                {
                    m_op->activate();
                }
                catch(OperatorError &)
                {
                    throw;
                }
                catch(std::exception & e)
                {
                    throw OperatorError(*info(), e.what());
                }
               
                m_status = ACTIVE;
            }
            
            void SynchronizedOperatorKernel::deactivate()
            {
                lock_t lock(m_mutex);
                
                if(m_status == INITIALIZED || m_status == NONE)
                    return;
                
                if(m_status == EXECUTING)
                    throw WrongOperatorState(*info(), "Operator can not be deactivated while it is executing.");
                
                m_inputMap.clear();
                m_outputMap.clear();
                m_status = INITIALIZED;
                
                try
                {
                    if (m_op)
                        m_op->deactivate();
                }
                catch(OperatorError &)
                {
                    throw;
                }
                catch(std::exception & e)
                {
                    throw OperatorError(*info(), e.what());
                }
            }
            
            void SynchronizedOperatorKernel::deinitialize()
            {
                lock_t lock(m_mutex);
                
                if(m_status == NONE)
                    return;
                
                if(m_status == EXECUTING)
                    throw WrongOperatorState(*info(), "Operator must be inactive to be deinitialized.");
                
                m_status = NONE;
                
                try
                {
                    if (m_op)
                        m_op->deinitialize();
                }
                catch(OperatorError &)
                {
                    throw;
                }
                catch(std::exception & e)
                {
                    throw OperatorError(*info(), e.what());
                }
            }
            
            DataRef SynchronizedOperatorKernel::getParameter(unsigned int id, const bool waitWithTimeout, const unsigned int timeout)
            {
                unique_lock_t lock(m_mutex);
                
                while(m_parametersAreLocked)
                    waitForSignal(m_parameterCond, lock, waitWithTimeout, timeout);
                
                validateParameterId(id);
                validateReadAccess(id);
                
                try
                {
                    return m_op->getParameter(id);
                }
                catch(OperatorError &)
                {
                    throw;
                }
                catch(std::exception & e)
                {
                    throw OperatorError(*info(), e.what());
                }
            }
                
            void SynchronizedOperatorKernel::setParameter(unsigned int id, const Data& value, const bool waitWithTimeout, const unsigned int timeout)
            {
                unique_lock_t lock(m_mutex);
                
                validateParameterId(id);
                validateParameterType(id, value.variant());
                
                while(m_parametersAreLocked)
                    waitForSignal(m_parameterCond, lock, waitWithTimeout, timeout);
                
                validateWriteAccess(id);
                
                try
                {
                    m_op->setParameter(id, value);
                }
                catch(OperatorError &)
                {
                    throw;
                }
                catch(std::exception & e)
                {
                    throw OperatorError(*info(), e.what());
                }
            }

            void SynchronizedOperatorKernel::receiveInputData(const Id2DataMapper& mapper)
            {   
                unique_lock_t lock(m_mutex);
                m_parametersAreLocked = false;
                
                BOOST_ASSERT(m_status == EXECUTING); // this function can only be called from OperatorKernel::execute();
                
                try
                {
                    while(! mapper.tryGet(m_inputMap))
                        waitForSignal(m_dataCond, lock, false);
                    
                    mapper.get(m_inputMap);
                }
                catch(Interrupt&)
                {
                    m_parametersAreLocked = true;
                    throw;
                }   
                
                m_parametersAreLocked = true;
                m_dataCond.notify_all();
            }

            void SynchronizedOperatorKernel::sendOutputData(const runtime::Id2DataMapper& mapper)
            {
                unique_lock_t lock(m_mutex);
                m_parametersAreLocked = false;
                
                BOOST_ASSERT(m_status == EXECUTING); // this function can only be called from OperatorKernel::execute()
                
                try
                {
                    while(! mapper.trySet(m_outputMap))
                        waitForSignal(m_dataCond, lock, false);
                    
                    mapper.set(m_outputMap);
                }
                catch(Interrupt&)
                {
                    m_parametersAreLocked = true;
                    throw;
                }   
                
                m_parametersAreLocked = true;
                m_dataCond.notify_all();  
            }
            
            void SynchronizedOperatorKernel::validateDataAccess()
            {
                if( m_status != ACTIVE
                    && m_status != EXECUTING)
                {
                    throw WrongOperatorState(*info(), "Operator must be active to access data.");
                }
            }
            
            DataContainer SynchronizedOperatorKernel::getOutputData(const unsigned int id)
            {
                unique_lock_t lock(m_mutex);
                validateDataAccess();
                
                while(m_outputMap.get(id).empty())
                {
                    bool success = false;
                    {
                        lock.unlock();
                        
                        // this section might throw an InterruptionException which will fall
                        // through to the calling function
                        success = tryExecute();
                        
                        lock.lock();
                    }
                    
                    if(! success)
                        waitForSignal(m_dataCond, lock, false); // wait for the situation to change
                    else
                        testForInterrupt(); // give us some chance to escape the while loop
                }
                
                m_dataCond.notify_all();
                return m_outputMap.get(id);
            }

            void SynchronizedOperatorKernel::setInputData(const unsigned int id, DataContainer data)
            {
                unique_lock_t lock(m_mutex);
                validateDataAccess();
                
                while(! m_inputMap.get(id).empty())
                {
                    bool success = false;
                    {
                        lock.unlock();
                        
                        // this section might throw an Interrupt exception which will fall
                        // through to the calling function
                        success = tryExecute();
                        
                        lock.lock();
                    }
                    
                    if(! success)
                        waitForSignal(m_dataCond, lock, false); // wait for the situation to change
                    else
                        testForInterrupt(); // give us some chance to escape the while loop
                }
                
                m_dataCond.notify_all();
                m_inputMap.set(id, data);
                
                // if this is a greedy operator try to execute it immediately
                if (m_op->properties().isGreedy)
                {
                   lock.unlock();
                   tryExecute();
                }
            }
            
            void SynchronizedOperatorKernel::clearOutputData(unsigned int id)
            {
                lock_t lock(m_mutex);
                validateDataAccess();
                
                m_outputMap.set(id, DataContainer());
                m_dataCond.notify_all();
            }
            
            void SynchronizedOperatorKernel::lockParameters()
            {
                lock_t lock(m_mutex);
                m_parametersAreLocked = true;
            }

            void SynchronizedOperatorKernel::unlockParameters()
            {
                lock_t lock(m_mutex);
                m_parametersAreLocked = false;
            }
            
            const AbstractFactory& SynchronizedOperatorKernel::factory() const
            {
                if (m_factory)
                    return *m_factory;
                else
                    return EMPTY_FACTORY;                
            }

            void SynchronizedOperatorKernel::setFactory(const AbstractFactory*const factory)
            {
                lock_t lock(m_mutex);
                
                if (m_status == EXECUTING || m_status == ACTIVE)
                    throw WrongState("Factory can not be set if the operator is active or executing.");
                
                m_factory = factory;
            }
            
            bool SynchronizedOperatorKernel::tryExecute()
            {
                {
                    lock_t lock(m_mutex);
                    
                    if(m_status == EXECUTING)
                    {
                        return false;
                    }
                    else
                    {
                        try
                        {
                            boost::this_thread::interruption_point();
                        }
                        catch(boost::thread_interrupted&)
                        {
                            m_parameterCond.notify_all();
                            throw Interrupt();
                        }
                        m_status = EXECUTING;
                        m_parametersAreLocked = true;
                    }
                }
                
                try
                {
                    m_op->execute(*this);
                }
                catch(Interrupt &)
                {
                    // pass interrupts to the caller
                    lock_t lock(m_mutex);
                    m_status = ACTIVE;
                    m_parametersAreLocked = false;
                    m_parameterCond.notify_all();
                    
                    throw;
                }
                catch(OperatorError &)
                {
                    // pass all operator exceptions to the caller
                    lock_t lock(m_mutex);
                    m_status = ACTIVE;
                    m_parametersAreLocked = false;
                    m_parameterCond.notify_all();
                    
                    throw;
                }
                catch(boost::thread_interrupted&)
                {
                    // pass interrupts to the caller
                    lock_t lock(m_mutex);
                    m_status = ACTIVE;
                    m_parametersAreLocked = false;
                    m_parameterCond.notify_all();
                    
                    throw Interrupt();
                }
                catch(std::exception & e)
                {
                    // wrap other exceptions
                    lock_t lock(m_mutex);
                    m_status = ACTIVE;
                    m_parametersAreLocked = false;
                    m_parameterCond.notify_all();
                    
                    throw OperatorError(*info(), e.what());
                }
                catch(...)
                {
                    // handle remaining exceptions
                    lock_t lock(m_mutex);
                    m_status = ACTIVE;
                    m_parametersAreLocked = false;
                    m_parameterCond.notify_all();
                    
                    throw OperatorError(*info(), "Unknown error.");
                }
                
                {
                    lock_t lock(m_mutex);
                    m_status = ACTIVE;
                    m_parametersAreLocked = false;
                    m_parameterCond.notify_all();
                }
        
                return true;
            }
            
            void SynchronizedOperatorKernel::waitForSignal(boost::condition_variable & condition, unique_lock_t& lock,
                                                           const bool waitWithTimeout, const unsigned int timeout)
            {
                try
                {
                    if(waitWithTimeout)
                    {
                        if(condition.wait_for(lock, boost::chrono::milliseconds(timeout))
                            == boost::cv_status::timeout)
                        {
                            throw Timeout();
                        }
                    }
                    else
                    {
                        condition.wait(lock);
                    }
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
                    default:
                        ;
                    }
                    break;
                case INITIALIZED:
                    switch(param.accessMode())
                    {
                    case Parameter::NO_ACCESS:
                        throw ParameterAccessViolation(param, *this->info());
                    default:
                        ;
                    }
                    break;
                case ACTIVE:
                case EXECUTING:
                    switch(param.accessMode())
                    {
                    case Parameter::NO_ACCESS:
                        throw ParameterAccessViolation(param, *this->info());
                    default:
                        ;
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
                    default:
                        ;
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
                    default:
                        ;
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
                    default:
                        ;
                    }
                    break;
                default:
                    BOOST_ASSERT(false);    
                }
            }
            
            void SynchronizedOperatorKernel::validateParameterType(const unsigned int id, const runtime::VariantInterface& type)
            {
                const Parameter& param = info()->parameter(id);
                if(! type.isVariant(param.variant()))
                    throw WrongParameterType(param, *this->info());
            }
            
            void SynchronizedOperatorKernel::interrupt()
            {
                lock_t lock(m_mutex);
                    
                if(m_status != EXECUTING)
                    return;
                
                try
                {
                    m_op->interrupt();
                }
                catch(OperatorError &)
                {
                    throw;
                }
                catch(std::exception & e)
                {
                    throw OperatorError(*info(), e.what());
                }
            }
        }
    }
}
