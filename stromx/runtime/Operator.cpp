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

#include <boost/thread.hpp>
#include "stromx/runtime/Description.h"
#include "stromx/runtime/Exception.h"
#include "stromx/runtime/Input.h"
#include "stromx/runtime/Operator.h"
#include "stromx/runtime/OperatorException.h"
#include "stromx/runtime/OperatorInfo.h"
#include "stromx/runtime/Output.h"
#include "stromx/runtime/impl/Id2DataMap.h"
#include "stromx/runtime/impl/InputNode.h"
#include "stromx/runtime/impl/MutexHandle.h"
#include "stromx/runtime/impl/OutputNode.h"
#include "stromx/runtime/impl/SynchronizedOperatorKernel.h"


namespace stromx
{
    namespace runtime
    {
        /** \cond */        
        class Operator::InternalObserver : public impl::Id2DataMapObserver
        {
        public:
            enum Type
            {
                INPUT,
                OUTPUT
            };
            
            InternalObserver(const Operator* const op, const Type type)
             : m_op(op),
                m_type(type)
            {}
            
            virtual ~InternalObserver() {}
            
            virtual void observe(const unsigned int id, const DataContainer & data) const
            {
                switch(m_type)
                {
                case INPUT:
                    m_op->observeInput(id, data);
                    break;
                case OUTPUT:
                    m_op->observeOutput(id, data);
                    break;
                default:
                    BOOST_ASSERT(false);
                }
            }
            
        private:
            const Operator* m_op;
            Type m_type;
        };
        /** \endcond */
        
        using namespace impl;
        
        Operator::Operator(OperatorKernel*const kernel)
          : m_inputObserver(0),
            m_outputObserver(0),
            m_kernel(new SynchronizedOperatorKernel(kernel)),
            m_observerMutex(new impl::MutexHandle())
        {
            m_inputObserver = new InternalObserver(this, InternalObserver::INPUT);
            m_outputObserver = new InternalObserver(this, InternalObserver::OUTPUT);
        }

        Operator::~Operator()
        {
            try
            {
                deinitialize();
            }
            catch(OperatorError&)
            {
                // ignore all exceptions in destructor
            }
            
            delete m_kernel;

            delete m_inputObserver;
            delete m_outputObserver;
            delete m_observerMutex;
        }
        
        const OperatorInfo& Operator::info() const
        { 
            return *m_kernel->info(); 
        }
        
        Operator::Status Operator::status() const
        { 
            return Status(m_kernel->status());
        }
        
        void Operator::setParameter(const unsigned int id, const Data& value)
        { 
            try
            {
                m_kernel->setParameter(id, value, false);
            }
            catch(OperatorError & e)
            {
                e.setName(m_name);
                throw;
            }
        }
        
        void Operator::setParameter(const unsigned int id, const Data& value, const unsigned int timeout)
        { 
            try
            {
                m_kernel->setParameter(id, value, true, timeout);
            }
            catch(OperatorError & e)
            {
                e.setName(m_name);
                throw;
            }
        }
        
        DataRef Operator::getParameter(const unsigned int id) const
        { 
            return m_kernel->getParameter(id, false); 
        }
        
        DataRef Operator::getParameter(const unsigned int id, const unsigned int timeout) const
        { 
            return m_kernel->getParameter(id, true, timeout); 
        }
        
        const DataContainer Operator::getOutputData(const unsigned int id) const
        { 
            return m_kernel->getOutputData(id); 
        }
        
        void Operator::setInputData(const unsigned int id, const DataContainer & data)
        {
            m_kernel->setInputData(id, data);
        }
        
        void Operator::clearOutputData(const unsigned int id)
        {
            m_kernel->clearOutputData(id);
        }
        
        void Operator::initialize()
        {
            m_kernel->initialize(m_inputObserver, m_outputObserver);
            
            for(std::vector<const Description*>::const_iterator iter = m_kernel->info()->inputs().begin();
                iter != m_kernel->info()->inputs().end();
                ++iter)
            {
                // this check is most probably redundant because OperatorKernel::initialize()
                // makes sure the IDs are unique
                if(m_inputs.count((*iter)->id()))
                    throw WrongArgument("Two inputs with the same ID.");
                
                m_inputs[(*iter)->id()] = new InputNode(this, (*iter)->id());
            }
            
            for(std::vector<const Description*>::const_iterator iter = m_kernel->info()->outputs().begin();
                iter != m_kernel->info()->outputs().end();
                ++iter)
            {
                // this check is most probably redundant because OperatorKernel::initialize()
                // makes sure the IDs are unique
                if(m_outputs.count((*iter)->id()))
                    throw WrongArgument("Two outputs with the same ID.");
                
                m_outputs[(*iter)->id()] = new OutputNode(this, (*iter)->id());
            }
        }
        
        void Operator::deinitialize()
        {
            if(status() == ACTIVE)
                deactivate();
                
            try
            {
                m_kernel->deinitialize();
            }
            catch(OperatorError& e)
            {
                e.setName(name());
                throw;
            }
            
            for(std::map<unsigned int, InputNode*>::iterator iter = m_inputs.begin();
                iter != m_inputs.end();
                ++iter)
            {
                delete iter->second;
            } 
            
            for(std::map<unsigned int, OutputNode*>::iterator iter = m_outputs.begin();
                iter != m_outputs.end();
                ++iter)
            {
                delete iter->second;
            }
            
            m_inputs.clear();
            m_outputs.clear();
        }
        
        InputNode* Operator::getInputNode(const unsigned int id) const
        {
            std::map<unsigned int, impl::InputNode*>::const_iterator iter = m_inputs.find(id);
            
            if(iter == m_inputs.end())
                throw WrongArgument("Input with this ID does not exist.");
            
            return iter->second;
        }

        OutputNode* Operator::getOutputNode(const unsigned int id) const
        {
            std::map<unsigned int, impl::OutputNode*>::const_iterator iter = m_outputs.find(id);
            
            if(iter == m_outputs.end())
                throw WrongArgument("Output with this ID does not exist.");
            
            return iter->second;
        }

        void Operator::activate()
        {   
            std::map<unsigned int, impl::OutputNode*>::const_iterator iter = m_outputs.begin();
            for( ; iter != m_outputs.end(); ++iter)
                iter->second->reset();
            
            try
            {
                m_kernel->activate(); 
            }
            catch(OperatorError& e)
            {
                e.setName(name());
                throw;
            }
        }
        
        void Operator::deactivate()
        {
            try
            {
                m_kernel->deactivate();
            }
            catch(OperatorError& e)
            {
                e.setName(name());
                throw;
            }
        }
        
        void Operator::addObserver(const ConnectorObserver*const observer)
        {
            boost::lock_guard<boost::mutex> lock(m_observerMutex->mutex());
            
            if(! observer)
                throw WrongArgument("Passed 0 as observer.");
            
            m_observers.insert(observer);
        }

        void Operator::removeObserver(const ConnectorObserver*const observer)
        {
            boost::lock_guard<boost::mutex> lock(m_observerMutex->mutex());
            
            if(m_observers.erase(observer) != 1)
                throw WrongArgument("Observer has not been added to operator.");
        }
        
        void Operator::observeInput(const unsigned int id, const stromx::runtime::DataContainer& data) const
        {
            boost::lock_guard<boost::mutex> lock(m_observerMutex->mutex());
            
            for(std::set<const ConnectorObserver*>::const_iterator iter = m_observers.begin();
                iter != m_observers.end();
                ++iter)
            {
                try
                {
                    (*iter)->observe(Input(this, id), data);
                }
                catch(Interrupt &)
                {
                    throw; // rethrow interrupts
                }
                catch(...)
                {
                    // ignore all other exceptions which are thrown by the observer
                }
            }
        }

        void Operator::observeOutput(const unsigned int id, const stromx::runtime::DataContainer& data) const
        {
            boost::lock_guard<boost::mutex> lock(m_observerMutex->mutex());
            
            for(std::set<const ConnectorObserver*>::const_iterator iter = m_observers.begin();
                iter != m_observers.end();
                ++iter)
            {
                try
                {
                    (*iter)->observe(Output(this, id), data);
                }
                catch(Interrupt &)
                {
                    throw; // rethrow interrupts
                }
                catch(...)
                {
                    // ignore all other exceptions which are thrown by the observer
                }
            }
        }
        
        void Operator::setFactory(const AbstractFactory*const factory)
        {
            m_kernel->setFactory(factory);
        }
        
        const AbstractFactory* Operator::factory() const
        {
            return m_kernel->factoryPtr();
        }
    }
}