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

#include "stromx/cvsupport/Clip.h"
#include "stromx/cvsupport/Image.h"
#include "stromx/cvsupport/Utilities.h"
#include <stromx/runtime/DataContainer.h>
#include <stromx/runtime/DataProvider.h>
#include <stromx/runtime/Id2DataPair.h>
#include <stromx/runtime/NumericParameter.h>
#include <stromx/runtime/OperatorException.h>
#include <stromx/runtime/Primitive.h>
#include <stromx/runtime/WriteAccess.h>

namespace stromx
{
    using namespace runtime;

    namespace cvsupport
    {
        const std::string Clip::TYPE("Clip");
        
        const std::string Clip::PACKAGE(STROMX_CVSUPPORT_PACKAGE_NAME);
        const Version Clip::VERSION(0, 1, 0);
        
        Clip::Clip()
        : OperatorKernel(TYPE, PACKAGE, VERSION, setupInputs(), setupOutputs(), setupParameters())
        {
        }

        void Clip::setParameter(unsigned int id, const Data& value)
        {
            try
            {
                switch(id)
                {
                case TOP:
                    m_top = stromx::runtime::data_cast<UInt32>(value);
                    break;
                case LEFT:
                    m_left = stromx::runtime::data_cast<UInt32>(value);
                    break;
                case WIDTH:
                    m_width = stromx::runtime::data_cast<UInt32>(value);
                    break;
                case HEIGHT:
                    m_height = stromx::runtime::data_cast<UInt32>(value);
                    break;
                default:
                    throw WrongParameterId(id, *this);
                }
            }
            catch(std::bad_cast&)
            {
                throw WrongParameterType(parameter(id), *this);
            }
        }

        const DataRef Clip::getParameter(const unsigned int id) const
        {
            switch(id)
            {
            case TOP:
                return m_top;
            case LEFT:
                return m_left;
            case WIDTH:
                return m_width;
            case HEIGHT:
                return m_height;
            default:
                throw WrongParameterId(id, *this);
            }
        }  
        
        void Clip::execute(DataProvider& provider)
        {
            Id2DataPair inputDataMapper(INPUT);
            provider.receiveInputData(inputDataMapper);
            
            DataContainer container = inputDataMapper.data();
            WriteAccess<runtime::Image> access(container);
            runtime::Image& image = access();
            
            unsigned int top = m_top;
            unsigned int left = m_left;
            unsigned int height = m_height;
            unsigned int width = m_width;
            
            adjustClipRegion(image.width(), image.height(), left, top, width, height);
            
            uint8_t* data = image.data() + top * image.stride() + left * image.pixelSize();
            image.initializeImage(width, height, image.stride(), data, image.pixelType());
            
            Id2DataPair outputDataMapper(OUTPUT, container);
            provider.sendOutputData( outputDataMapper);
        }
        
        const std::vector<const runtime::Description*> Clip::setupInputs()
        {
            std::vector<const Description*> inputs;
            
            Description* input = new Description(INPUT, Variant::IMAGE);
            input->setTitle("Input");
            inputs.push_back(input);
            
            return inputs;
        }
        
        const std::vector<const Description*> Clip::setupOutputs()
        {
            std::vector<const Description*> outputs;
            
            Description* output = new Description(OUTPUT, Variant::IMAGE);
            output->setTitle("Output");
            outputs.push_back(output);
            
            return outputs;
        }
        
        const std::vector<const Parameter*> Clip::setupParameters()
        {
            std::vector<const runtime::Parameter*> parameters;
            
            NumericParameter<UInt32>* top = new NumericParameter<UInt32>(TOP);
            top->setTitle("Top");
            top->setAccessMode(runtime::Parameter::ACTIVATED_WRITE);
            parameters.push_back(top);
            
            NumericParameter<UInt32>* left = new NumericParameter<UInt32>(LEFT);
            left->setTitle("Left");
            left->setAccessMode(runtime::Parameter::ACTIVATED_WRITE);
            parameters.push_back(left);
            
            NumericParameter<UInt32>* width = new NumericParameter<UInt32>(WIDTH);
            width->setTitle("Width");
            width->setAccessMode(runtime::Parameter::ACTIVATED_WRITE);
            parameters.push_back(width);
            
            NumericParameter<UInt32>* height = new NumericParameter<UInt32>(HEIGHT);
            height->setTitle("Height");
            height->setAccessMode(runtime::Parameter::ACTIVATED_WRITE);
            parameters.push_back(height);
                                        
            return parameters;
        }
        
        void Clip::adjustClipRegion(const unsigned int destWidth, const unsigned int destHeight, unsigned int& left, unsigned int& top, unsigned int& width, unsigned int& height)
        {
            left = std::min(left, destWidth);
            width = std::min(width, destWidth - left);
            top = std::min(top, destHeight);
            height = std::min(height, destHeight - top);
        }
    } 
}
