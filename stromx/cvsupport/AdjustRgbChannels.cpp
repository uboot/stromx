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

#include "stromx/cvsupport/AdjustRgbChannels.h"
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
        const std::string AdjustRgbChannels::TYPE("AdjustRgbChannels");
        const std::string AdjustRgbChannels::PACKAGE(STROMX_CVSUPPORT_PACKAGE_NAME);
        const Version AdjustRgbChannels::VERSION(0, 1, 0);
        
        AdjustRgbChannels::AdjustRgbChannels()
        : OperatorKernel(TYPE, PACKAGE, VERSION, setupInputs(), setupOutputs(), setupParameters()),
            m_red(1.0),
            m_green(1.0),
            m_blue(1.0)
        {
        }

        void AdjustRgbChannels::setParameter(unsigned int id, const Data& value)
        {
            try
            {
                switch(id)
                {
                case RED:
                    m_red = stromx::runtime::data_cast<Float64>(value);
                    break;
                case GREEN:
                    m_green = stromx::runtime::data_cast<Float64>(value);
                    break;
                case BLUE:
                    m_blue = stromx::runtime::data_cast<Float64>(value);
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

        const DataRef AdjustRgbChannels::getParameter(const unsigned int id) const
        {
            switch(id)
            {
            case RED:
                return m_red;
            case GREEN:
                return m_green;
            case BLUE:
                return m_blue;
            default:
                throw WrongParameterId(id, *this);
            }
        }  
        
        void AdjustRgbChannels::execute(DataProvider& provider)
        {
            Id2DataPair inputDataMapper(INPUT);
            provider.receiveInputData(inputDataMapper);
            
            DataContainer container = inputDataMapper.data();
            WriteAccess<runtime::Image> access(container);
            runtime::Image& image = access();

            cv::Mat cvImage = getOpenCvMat(image);
            
            switch(image.pixelType())
            {
            case runtime::Image::RGB_24:
            {
                typedef cv::Vec<unsigned char, 3> pixel_t;

                cv::MatIterator_<pixel_t> it = cvImage.begin<pixel_t>();
                cv::MatIterator_<pixel_t> it_end = cvImage.end<pixel_t>();
                for(; it != it_end; ++it)
                {
                    (*it)[0] = cv::saturate_cast<unsigned char>((double((*it)[0]) * m_red));
                    (*it)[1] = cv::saturate_cast<unsigned char>((double((*it)[1]) * m_green));
                    (*it)[2] = cv::saturate_cast<unsigned char>((double((*it)[2]) * m_blue));
                }
                break;
            }
            case runtime::Image::BGR_24:
            {
                typedef cv::Vec<unsigned char, 3> pixel_t;

                cv::MatIterator_<pixel_t> it = cvImage.begin<pixel_t>();
                cv::MatIterator_<pixel_t> it_end = cvImage.end<pixel_t>();
                for(; it != it_end; ++it)
                {
                    (*it)[0] = cv::saturate_cast<unsigned char>((double((*it)[0]) * m_blue));
                    (*it)[1] = cv::saturate_cast<unsigned char>((double((*it)[1]) * m_green));
                    (*it)[2] = cv::saturate_cast<unsigned char>((double((*it)[2]) * m_red));
                }
                break;
            }
            default:
                throw WrongInputType(INPUT, *this);
            }
            
            Id2DataPair outputDataMapper(OUTPUT, container);
            provider.sendOutputData( outputDataMapper);
        }
        
        const std::vector<const runtime::Description*> AdjustRgbChannels::setupInputs()
        {
            std::vector<const Description*> inputs;
            
            Description* input = new Description(INPUT, Variant::RGB_IMAGE);
            input->setTitle("Input");
            inputs.push_back(input);
            
            return inputs;
        }
        
        const std::vector<const Description*> AdjustRgbChannels::setupOutputs()
        {
            std::vector<const Description*> outputs;
            
            Description* output = new Description(OUTPUT, Variant::RGB_IMAGE);
            output->setTitle("Output");
            outputs.push_back(output);
            
            return outputs;
        }
        
        const std::vector<const Parameter*> AdjustRgbChannels::setupParameters()
        {
            std::vector<const runtime::Parameter*> parameters;
            
            NumericParameter<Float64>* red = new NumericParameter<Float64>(RED, Float64(0.0), Float64::MAX);
            red->setTitle("Red");
            red->setAccessMode(runtime::Parameter::ACTIVATED_WRITE);
            parameters.push_back(red);
            
            NumericParameter<Float64>* green = new NumericParameter<Float64>(GREEN, Float64(0.0), Float64::MAX);
            green->setTitle("Green");
            green->setAccessMode(runtime::Parameter::ACTIVATED_WRITE);
            parameters.push_back(green);
            
            NumericParameter<Float64>* blue = new NumericParameter<Float64>(BLUE, Float64(0.0), Float64::MAX);
            blue->setTitle("Blue");
            blue->setAccessMode(runtime::Parameter::ACTIVATED_WRITE);
            parameters.push_back(blue);
                                        
            return parameters;
        }
    } 
}
