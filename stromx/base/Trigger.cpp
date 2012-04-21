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

#include <boost/thread/condition_variable.hpp>
#include <boost/thread/mutex.hpp>
#include "Trigger.h"
#include <stromx/core/DataContainer.h>
#include <stromx/core/DataProvider.h>
#include <stromx/core/Id2DataPair.h>
#include <stromx/core/OperatorException.h>

namespace stromx
{
    using namespace core;

    namespace base
    {
        typedef boost::unique_lock<boost::mutex> unique_lock_t;
        
        /** \cond */
        namespace impl
        {
            struct BoostConditionVariable
            {
                boost::condition_variable_any m_cond;
                boost::mutex m_mutex;
            };
        }
        /** \endcond */
        
        const std::string Trigger::TYPE("Trigger");
        
        const std::string Trigger::PACKAGE(STROMX_BASE_PACKAGE_NAME);
        const Version Trigger::VERSION(BASE_VERSION_MAJOR, BASE_VERSION_MINOR, BASE_VERSION_PATCH);
        
        Trigger::Trigger()
          : OperatorKernel(TYPE, PACKAGE, VERSION, setupInputs(), setupOutputs(), setupParameters()),
            m_cond(new impl::BoostConditionVariable),
            m_active(true)
        {
        }
        
        Trigger::~Trigger()
        {
            delete m_cond;
        }

        void Trigger::setParameter(unsigned int id, const Data& value)
        {
            try
            {
                switch(id)
                {
                case TRIGGER:
                {
                    // trigger
                    m_cond->m_cond.notify_all();
                    break;
                }
                case ACTIVE:
                {
                    bool boolValue = dynamic_cast<const Bool&>(value);
                    m_active = boolValue;
                    
                    // make sure the thread does not stop at the condition variable
                    // if the trigger is deactivated
                    if(! boolValue)
                        m_cond->m_cond.notify_all();
                    break;
                }
                default:
                    throw WrongParameterId(id, *this);
                }
            }
            catch(std::bad_cast&)
            {
                throw WrongParameterType(parameter(id), *this);
            }
        }

        const Data& Trigger::getParameter(const unsigned int id) const
        {
            switch(id)
            {
            case TRIGGER:
                throw ParameterAccessViolation(parameter(id), *this);
            case ACTIVE:
                return m_active;
            default:
                throw WrongParameterId(id, *this);
            }
        }  
        
        void Trigger::execute(DataProvider& provider)
        {
            Id2DataPair inputDataMapper(INPUT);
            provider.receiveInputData(inputDataMapper);
            
            if(m_active)
            {
                try
                {
                    // wait for trigger
                    unique_lock_t lock(m_cond->m_mutex);
                    
                    // allow to trigger while waiting
                    provider.unlockParameters();
                    m_cond->m_cond.wait(lock);
                }
                catch(boost::thread_interrupted&)
                {
                    throw Interrupt();
                }
            }
            
            Id2DataPair outputDataMapper(OUTPUT, inputDataMapper.data());
            provider.sendOutputData( outputDataMapper);
        }
        
        const std::vector<const core::Description*> Trigger::setupInputs()
        {
            std::vector<const Description*> inputs;
            
            Description* input = new Description(OUTPUT, DataVariant::DATA);
            input->setName("Input");
            inputs.push_back(input);
            
            return inputs;
        }
        
        const std::vector<const Description*> Trigger::setupOutputs()
        {
            std::vector<const Description*> outputs;
            
            Description* output = new Description(OUTPUT, DataVariant::DATA);
            output->setName("Output");
            outputs.push_back(output);
            
            return outputs;
        }
        
        const std::vector<const Parameter*> Trigger::setupParameters()
        {
            std::vector<const core::Parameter*> parameters;
            
            Parameter* trigger = new Parameter(TRIGGER, DataVariant::TRIGGER);
            trigger->setName("Trigger");
            trigger->setAccessMode(core::Parameter::ACTIVATED_WRITE);
            parameters.push_back(trigger);
            
            Parameter* active = new Parameter(ACTIVE, DataVariant::BOOL);
            active->setName("Active");
            active->setAccessMode(core::Parameter::ACTIVATED_WRITE);
            parameters.push_back(active);
                                        
            return parameters;
        }
    } 
}
