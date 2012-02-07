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
#include "Description.h"
#include "Exception.h"
#include "Operator.h"
#include "OperatorInfo.h"
#include "Input.h"
#include "Output.h"
#include "impl/InputNode.h"
#include "impl/OutputNode.h"
#include "impl/SynchronizedOperatorKernel.h"
#include "impl/Id2DataMap.h"

namespace stromx
{
    namespace core
    {
        
        Operator::InternalObserver::InternalObserver(const Operator* const op, const Type type)
          : m_op(op),
            m_type(type)
        {}
        
        void Operator::InternalObserver::observe(const unsigned int id, const DataContainer & data) const
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
        
        using namespace impl;
        
        Operator::Operator(OperatorKernel*const kernel)
          : m_inputObserver(0),
            m_outputObserver(0),
            m_kernel(new SynchronizedOperatorKernel(kernel)),
            m_observerMutex(new boost::mutex()),
            m_isPartOfStream(false)
        {
            m_inputObserver = new InternalObserver(this, InternalObserver::INPUT);
            m_outputObserver = new InternalObserver(this, InternalObserver::OUTPUT);
        }

        Operator::~Operator()
        {
            deinitialize();
            
            delete m_kernel;

            delete m_inputObserver;
            delete m_outputObserver;
        }
        
        const OperatorInfo& Operator::info() const
        { 
            return *m_kernel->info(); 
        }
        
        const Operator::Status Operator::status() const
        { 
            return Status(m_kernel->status());
        }
        
        void Operator::setParameter(const unsigned int id, const Data& value)
        { 
            m_kernel->setParameter(id, value);
        }
        
        const Data& Operator::getParameter(const unsigned int id) const
        { 
            return m_kernel->getParameter(id); 
        }
        
        const DataContainer Operator::getOutputData(const unsigned int id) const
        { 
            return m_kernel->getOutputData(id); 
        }
        
        void Operator::setInputData(const unsigned int id, const DataContainer data)
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
                
            m_kernel->deinitialize();
            
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
        
        InputNode*const Operator::getInputNode(const unsigned int id) const
        {
            std::map<unsigned int, impl::InputNode*>::const_iterator iter = m_inputs.find(id);
            
            if(iter == m_inputs.end())
                throw WrongArgument("Input with this ID does not exist.");
            
            return iter->second;
        }

        OutputNode*const Operator::getOutputNode(const unsigned int id) const
        {
            std::map<unsigned int, impl::OutputNode*>::const_iterator iter = m_outputs.find(id);
            
            if(iter == m_outputs.end())
                throw WrongArgument("Output with this ID does not exist.");
            
            return iter->second;
        }

        void Operator::activate()
        { 
            m_kernel->activate(); 
        }
        
        void Operator::deactivate()
        {
            m_kernel->deactivate();
        }
        
        void Operator::addObserver(const ConnectorObserver*const observer)
        {
            boost::lock_guard<boost::mutex> lock(*m_observerMutex);
            
            if(! observer)
                throw WrongArgument("Passed 0 as observer.");
            
            m_observers.insert(observer);
        }

        void Operator::removeObserver(const ConnectorObserver*const observer)
        {
            boost::lock_guard<boost::mutex> lock(*m_observerMutex);
            
            if(m_observers.erase(observer) != 1)
                throw WrongArgument("Observer has not been added to operator.");
        }
        
        void Operator::observeInput(const unsigned int id, const stromx::core::DataContainer& data) const
        {
            boost::lock_guard<boost::mutex> lock(*m_observerMutex);
            
            for(std::set<const ConnectorObserver*>::const_iterator iter = m_observers.begin();
                iter != m_observers.end();
                ++iter)
            {
                (*iter)->observe(Input(this, id), data);
            }
        }

        void Operator::observeOutput(const unsigned int id, const stromx::core::DataContainer& data) const
        {
            boost::lock_guard<boost::mutex> lock(*m_observerMutex);
            
            for(std::set<const ConnectorObserver*>::const_iterator iter = m_observers.begin();
                iter != m_observers.end();
                ++iter)
            {
                (*iter)->observe(Output(this, id), data);
            }
        }
        
        void Operator::addToStream()
        {
            if(m_isPartOfStream)
                throw WrongState("Operator has already been added to a stream.");
            
            if(status() != INITIALIZED)
                throw WrongState("Operator must be initialized to be added to a stream.");
            
            m_isPartOfStream = true;
        }

        void Operator::removeFromStream()
        {
            if(! m_isPartOfStream)
                throw WrongState("Operator has not been added to a stream.");
            
            if(status() != INITIALIZED)
                throw WrongState("Operator must be initialized but not active to be removed from a stream.");
            
            m_isPartOfStream = false;
        }
    }
}