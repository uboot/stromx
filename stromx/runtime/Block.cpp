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

#include "stromx/runtime/Block.h"

#include <boost/thread/condition_variable.hpp>
#include <boost/thread/mutex.hpp>
#include "stromx/runtime/DataContainer.h"
#include "stromx/runtime/DataProvider.h"
#include "stromx/runtime/EnumParameter.h"
#include "stromx/runtime/Id2DataPair.h"
#include "stromx/runtime/Id2DataComposite.h"
#include "stromx/runtime/Locale.h"
#include "stromx/runtime/OperatorException.h"
#include "stromx/runtime/TriggerData.h"

namespace stromx
{
    using namespace runtime;

    namespace runtime
    {
        typedef boost::lock_guard<boost::mutex> lock_t;
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
        
        const std::string Block::TYPE("Block");
        
        const std::string Block::PACKAGE(STROMX_RUNTIME_PACKAGE_NAME);
        const Version Block::VERSION(0, 1, 0);
        
        Block::Block()
          : OperatorKernel(TYPE, PACKAGE, VERSION, setupInitParameters()),
            m_cond(new impl::BoostConditionVariable),
            m_wasTriggered(false),
            m_state(TRIGGER_ACTIVE),
            m_triggerInput(false)
        {
        }
        
        Block::~Block()
        {
            delete m_cond;
        }

        void Block::setParameter(unsigned int id, const Data& value)
        {
            try
            {
                switch(id)
                {
                case TRIGGER:
                {
                    // trigger
                    if(BlockState(int(m_state)) != BLOCK_ALWAYS)
                    {
                        lock_t l(m_cond->m_mutex);
                        m_wasTriggered = true;
                        m_cond->m_cond.notify_all();
                    }
                    break;
                }
                case TRIGGER_INPUT:
                    m_triggerInput = data_cast<Bool>(value);
                    break;
                case STATE:
                {
                    Enum enumValue = data_cast<Enum>(value);
                    
                    switch(int(enumValue))
                    {
                        case PASS_ALWAYS:
                        case BLOCK_ALWAYS:
                        case TRIGGER_ACTIVE:
                            break;
                        default:
                            throw WrongParameterValue(*parameters()[id], *this);
                    }
                    
                    m_state = enumValue;
                    
                    // make sure the thread does not stop at the condition variable
                    // if the trigger is deactivated
                    if(m_state == PASS_ALWAYS)
                    {
                        m_wasTriggered = true;
                        m_cond->m_cond.notify_all();
                    }
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

        const DataRef Block::getParameter(const unsigned int id) const
        {
            switch(id)
            {
            case TRIGGER:
                return TriggerData();
            case STATE:
                return m_state;
            case TRIGGER_INPUT:
                return m_triggerInput;
            default:
                throw WrongParameterId(id, *this);
            }
        }  
        
        void Block::initialize()
        {
            stromx::runtime::OperatorKernel::initialize(setupInputs(),
                                                        setupOutputs(),
                                                        setupParameters());
        }
        
        void Block::activate()
        {
            lock_t l(m_cond->m_mutex);
            m_wasTriggered = false;
        }
        
        void Block::execute(DataProvider& provider)
        {
            Id2DataPair inputDataMapper(INPUT);
            
            if (! m_triggerInput) // triggered by setting the trigger parameter
            {
                provider.receiveInputData(inputDataMapper);
                BlockState state = BlockState(int(m_state));
                if(state == TRIGGER_ACTIVE || state == BLOCK_ALWAYS)
                {
                    try
                    {
                        unique_lock_t lock(m_cond->m_mutex);
                        
                        // allow to trigger while waiting
                        provider.unlockParameters();
                        
                        // wait for trigger
                        while (! m_wasTriggered)
                            m_cond->m_cond.wait(lock);
                        
                        m_wasTriggered = false;
                    }
                    catch(boost::thread_interrupted&)
                    {
                        throw Interrupt();
                    }
                }
            }
            else // triggered by receiving a trigger input
            {
                Id2DataPair triggerDataMapper(TRIGGER_DATA);
                provider.receiveInputData(inputDataMapper && triggerDataMapper);
            }
            
            Id2DataPair outputDataMapper(OUTPUT, inputDataMapper.data());
            provider.sendOutputData( outputDataMapper);
        }
        
        const std::vector<const runtime::Description*> Block::setupInputs()
        {
            std::vector<const Description*> inputs;
            
            Description* input = new Description(OUTPUT, Variant::DATA);
            input->setTitle("Input");
            inputs.push_back(input);
            
            if (m_triggerInput)
            {
                Description* triggerData = new Description(TRIGGER_DATA, Variant::DATA);
                triggerData->setTitle(L_("Trigger"));
                inputs.push_back(triggerData);
            }
            
            return inputs;
        }
        
        const std::vector<const Description*> Block::setupOutputs()
        {
            std::vector<const Description*> outputs;
            
            Description* output = new Description(OUTPUT, Variant::DATA);
            output->setTitle("Output");
            outputs.push_back(output);
            
            return outputs;
        }
        
        const std::vector<const Parameter*> Block::setupInitParameters()
        {
            std::vector<const runtime::Parameter*> parameters;
            
            Parameter* triggerInput = new Parameter(TRIGGER_INPUT, Variant::BOOL);
            triggerInput->setTitle(L_("Trigger input"));
            triggerInput->setAccessMode(runtime::Parameter::NONE_WRITE);
            parameters.push_back(triggerInput);
                                        
            return parameters;
        }        
        
        const std::vector<const Parameter*> Block::setupParameters()
        {
            std::vector<const runtime::Parameter*> parameters;
            
            if (! m_triggerInput)
            {  
                Parameter* trigger = new Parameter(TRIGGER, Variant::TRIGGER);
                trigger->setTitle(L_("Trigger"));
                trigger->setAccessMode(runtime::Parameter::ACTIVATED_WRITE);
                parameters.push_back(trigger);
            
                EnumParameter* state = new EnumParameter(STATE);
                state->setTitle(L_("State"));
                state->setAccessMode(runtime::Parameter::ACTIVATED_WRITE);
                state->add(EnumDescription(Enum(PASS_ALWAYS), L_("Block never")));
                state->add(EnumDescription(Enum(BLOCK_ALWAYS), L_("Block always")));
                state->add(EnumDescription(Enum(TRIGGER_ACTIVE), L_("Wait for trigger")));
                parameters.push_back(state);
            }
                                        
            return parameters;
        }
    } 
}
