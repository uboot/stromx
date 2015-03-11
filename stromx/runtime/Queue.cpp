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

#include "stromx/runtime/DataContainer.h"
#include "stromx/runtime/DataProvider.h"
#include "stromx/runtime/Id2DataPair.h"
#include "stromx/runtime/NumericParameter.h"
#include "stromx/runtime/OperatorException.h"
#include "stromx/runtime/Primitive.h"
#include "stromx/runtime/Queue.h"
#include "stromx/runtime/Try.h"

namespace stromx
{
    namespace runtime
    {
        const std::string Queue::TYPE("Queue");
        
        const std::string Queue::PACKAGE(STROMX_RUNTIME_PACKAGE_NAME);
        const Version Queue::VERSION(0, 1, 0);
        
        Queue::Queue()
        : OperatorKernel(TYPE, PACKAGE, VERSION, setupInputs(), setupOutputs(), setupParameters()),
            m_size(1)
        {
        }

        void Queue::setParameter(unsigned int id, const Data& value)
        {
            try
            {
                switch(id)
                {
                case SIZE:
                    if(m_size == 0)
                        throw WrongParameterValue(parameter(SIZE), *this);
                    
                    m_size = data_cast<UInt32>(value);
                    break;
                default:
                    throw WrongParameterId(id, *this);
                }
            }
            catch(std::bad_cast&)
            {
                throw WrongParameterValue(parameter(id), *this);
            }
        }

        const DataRef Queue::getParameter(const unsigned int id) const
        {
            switch(id)
            {
            case SIZE:
                return m_size;
            default:
                throw WrongParameterId(id, *this);
            }
        }  
        
        void Queue::deactivate()
        {
            m_deque.clear();
        }
        
        void Queue::execute(DataProvider& provider)
        {
            // if the queue is not full
            if(m_deque.size() < m_size)
            {
                Id2DataPair inputDataMapper(INPUT);
                if(! m_deque.empty())
                {
                    // there is some data in the queue
                    // try to get data from the input
                    provider.receiveInputData(Try(inputDataMapper));
                }
                else
                {
                    // there is no data in the queue
                    // wait here forever
                    provider.receiveInputData(inputDataMapper);
                }
                
                // if there was data push it on the queue
                if(! inputDataMapper.data().empty())
                    m_deque.push_back(inputDataMapper.data());
            }
                
            // if the queue is not empty
            if(! m_deque.empty())
            {
                Id2DataPair outputDataMapper(OUTPUT, m_deque.front());
                if(m_deque.size() < m_size)
                {
                    // there is some space left
                    // try to push data to the output
                    provider.sendOutputData(Try(outputDataMapper));
                }
                else
                {
                    // there is no space left
                    // wait here forever
                    provider.sendOutputData(outputDataMapper);
                }
                
                // if this was successful delete the data from the queue
                if(outputDataMapper.data().empty())
                    m_deque.pop_front();
            }
        }
        
        const std::vector<const runtime::Description*> Queue::setupInputs()
        {
            std::vector<const Description*> inputs;
            
            Description* input = new Description(INPUT, Variant::DATA);
            input->setTitle("Input");
            inputs.push_back(input);
            
            return inputs;
        }
        
        const std::vector<const Description*> Queue::setupOutputs()
        {
            std::vector<const Description*> outputs;
            
            Description* output = new Description(OUTPUT, Variant::DATA);
            output->setTitle("Output");
            outputs.push_back(output);
            
            return outputs;
        }
        
        const std::vector<const Parameter*> Queue::setupParameters()
        {
            std::vector<const runtime::Parameter*> parameters;
            
            NumericParameter<UInt32>* size = new NumericParameter<UInt32>(SIZE);
            size->setTitle("Size");
            size->setMin(UInt32(1));
            size->setAccessMode(runtime::Parameter::INITIALIZED_WRITE);
            parameters.push_back(size);
                                        
            return parameters;
        }
    } 
}
