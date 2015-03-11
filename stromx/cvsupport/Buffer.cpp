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

#include "stromx/cvsupport/Buffer.h"
#include "stromx/cvsupport/Config.h"
#include "stromx/cvsupport/Image.h"
#include <stromx/runtime/DataContainer.h>
#include <stromx/runtime/DataProvider.h>
#include <stromx/runtime/Id2DataComposite.h>
#include <stromx/runtime/Id2DataPair.h>
#include <stromx/runtime/NumericParameter.h>
#include <stromx/runtime/OperatorException.h>

namespace stromx
{
    using namespace runtime;
    
    namespace cvsupport
    {
        const std::string Buffer::TYPE("Buffer");
        const std::string Buffer::PACKAGE(STROMX_CVSUPPORT_PACKAGE_NAME);
        const Version Buffer::VERSION(0, 1, 0);
        
        Buffer::Buffer()
          : OperatorKernel(TYPE, PACKAGE, VERSION, setupInputs(), setupOutputs(), setupParameters()),
            m_bufferSize(0),
            m_numBuffers(1)
        {
        }

        void Buffer::setParameter(const unsigned int id, const Data& value)
        {
            try
            {
                switch(id)
                {
                case BUFFER_SIZE:
                    m_bufferSize = stromx::runtime::data_cast<UInt32>(value);;
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
        
        void Buffer::activate()
        {
            Data* buffer = 0;
            
            // delete all remaining buffers in the recycling access
            while((buffer = m_buffers()))
                delete buffer;
            
            // allocate all buffers and add them to the recycler
            for(unsigned int i = 0; i < m_numBuffers; ++i)
                m_buffers.add(DataContainer(new Image(m_bufferSize)));
        }

        void Buffer::deactivate()
        {
        }

        const DataRef Buffer::getParameter(const unsigned int id) const
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
        
        void Buffer::execute(DataProvider& provider)
        {
            Data* buffer = 0;
            
            // try to get a free buffer
            try
            {
                provider.unlockParameters();
                buffer = m_buffers();
                provider.lockParameters();
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
                
                Id2DataPair outputMapper(OUTPUT, bufferContainer);
                
                // send it to the output (together with the input image and the current index)
                provider.sendOutputData(outputMapper);
            }
        }
        
        const std::vector<const Description*> Buffer::setupInputs()
        {
            std::vector<const Description*> inputs;
            return inputs;
        }
        
        const std::vector<const Description*> Buffer::setupOutputs()
        {
            std::vector<const Description*> outputs;
        
            Description* output = new Description(OUTPUT, Variant::IMAGE);
            output->setTitle("Output");
            outputs.push_back(output);
            
            return outputs;
        }
        
        const std::vector<const Parameter*> Buffer::setupParameters()
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
