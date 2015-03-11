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

#include <iostream>
#include <stromx/runtime/DataContainer.h>
#include <stromx/runtime/DataProvider.h>
#include <stromx/runtime/Id2DataComposite.h>
#include <stromx/runtime/Id2DataPair.h>
#include <stromx/runtime/NumericParameter.h>
#include <stromx/runtime/OperatorException.h>
#include "stromx/cvsupport/Config.h"
#include "stromx/cvsupport/Image.h"
#include "stromx/cvsupport/impl/CameraBuffer.h"

namespace stromx
{
    using namespace runtime;
    
    namespace cvsupport
    {
        namespace impl
        {
            const std::string CameraBuffer::TYPE("CameraBuffer");
            const std::string CameraBuffer::PACKAGE(STROMX_CVSUPPORT_PACKAGE_NAME);
            const Version CameraBuffer::VERSION(0, 1, 0);
            
            CameraBuffer::CameraBuffer()
              : OperatorKernel(TYPE, PACKAGE, VERSION, setupInputs(), setupOutputs(), setupParameters()),
                m_bufferSize(0),
                m_numBuffers(1),
                m_id(0)
            {
            }

            void CameraBuffer::setParameter(const unsigned int id, const Data& value)
            {
                try
                {
                    switch(id)
                    {
                    case BUFFER_SIZE:
                        m_bufferSize = stromx::runtime::data_cast<UInt32>(value);
                        break;
                    case NUM_BUFFERS:
                    {
                        UInt32 newNumBuffers = stromx::runtime::data_cast<UInt32>(value);
                        if(newNumBuffers < 1)
                            throw WrongParameterValue(parameter(NUM_BUFFERS), *this);
                        m_numBuffers = newNumBuffers;
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
            
            void CameraBuffer::activate()
            {
                Data* buffer = 0;
                
                // delete all remaining buffers in the recycling access
                while((buffer = m_buffers()))
                    delete buffer;
                
                // allocate all buffers and add them to the recycler
                for(unsigned int i = 0; i < m_numBuffers; ++i)
                    m_buffers.add(DataContainer(new cvsupport::Image(m_bufferSize)));
                
                m_id = 0;
            }

            void CameraBuffer::deactivate()
            {
            }

            const DataRef CameraBuffer::getParameter(const unsigned int id) const
            {
                switch(id)
                {
                case BUFFER_SIZE:
                    return m_bufferSize;
                case NUM_BUFFERS:
                    return m_numBuffers;
                default:
                    throw WrongParameterId(id, *this);
                }
            }  
            
            void CameraBuffer::execute(DataProvider& provider)
            {
                // get the input data
                Id2DataPair inputMapper(INPUT);
                provider.receiveInputData(inputMapper);
                
                // try to get a free buffer
                Data* buffer = 0;
                try
                {
                    buffer = m_buffers(0);
                }
                catch(Timeout&)
                {
                }
                
                if(buffer)
                {
                    // there was a free buffer
                    DataContainer bufferContainer(buffer);
                    
                    // remember it in the recycling access
                    m_buffers.add(bufferContainer);
                    
                    Id2DataPair outputMapper(OUTPUT, inputMapper.data());
                    Id2DataPair bufferMapper(BUFFER, bufferContainer);
                    Id2DataPair idMapper(INDEX, DataContainer(new UInt32(m_id)));
                    
                    // send it to the output (together with the input image and the current index)
                    provider.sendOutputData(outputMapper && bufferMapper && idMapper);
                }
                
                // increase the index
                ++m_id;
            }
            
            const std::vector<const Description*> CameraBuffer::setupInputs()
            {
                std::vector<const Description*> inputs;
            
                Description* input = new Description(INPUT, Variant::IMAGE);
                input->setTitle("Input");
                inputs.push_back(input);
                
                return inputs;
            }
            
            const std::vector<const Description*> CameraBuffer::setupOutputs()
            {
                std::vector<const Description*> outputs;
            
                Description* output = new Description(OUTPUT, Variant::IMAGE);
                output->setTitle("Output");
                outputs.push_back(output);
            
                Description* buffer = new Description(BUFFER, Variant::IMAGE);
                buffer->setTitle("Buffer");
                outputs.push_back(buffer);
            
                Description* index = new Description(INDEX, Variant::UINT_32);
                index->setTitle("Index");
                outputs.push_back(index);
                
                return outputs;
            }
            
            const std::vector<const Parameter*> CameraBuffer::setupParameters()
            {
                std::vector<const Parameter*> parameters;
            
                NumericParameter<UInt32>* numBuffers = new NumericParameter<UInt32>(NUM_BUFFERS);
                numBuffers->setTitle("Number of buffers");
                numBuffers->setAccessMode(runtime::Parameter::INITIALIZED_WRITE);
                numBuffers->setMin(UInt32(1));
                parameters.push_back(numBuffers);
            
                NumericParameter<UInt32>* bufferSize = new NumericParameter<UInt32>(BUFFER_SIZE);
                bufferSize->setTitle("Buffer size in bytes");
                bufferSize->setAccessMode(runtime::Parameter::INITIALIZED_WRITE);
                parameters.push_back(bufferSize);
                                            
                return parameters;
            }
        }
    } 
}
