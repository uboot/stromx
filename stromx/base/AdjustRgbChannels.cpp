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

#include "AdjustRgbChannels.h"
#include "Image.h"
#include "Utilities.h"
#include <stromx/core/DataContainer.h>
#include <stromx/core/DataProvider.h>
#include <stromx/core/Id2DataPair.h>
#include <stromx/core/NumericParameter.h>
#include <stromx/core/OperatorException.h>
#include <stromx/core/Primitive.h>
#include <stromx/core/WriteAccess.h>

namespace stromx
{
    using namespace core;

    namespace base
    {
        const std::string AdjustRgbChannels::TYPE("AdjustRgbChannels");
        const std::string AdjustRgbChannels::PACKAGE(STROMX_BASE_PACKAGE_NAME);
        const Version AdjustRgbChannels::VERSION(BASE_VERSION_MAJOR, BASE_VERSION_MINOR, BASE_VERSION_PATCH);
        
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
                    m_red = dynamic_cast<const Double&>(value);
                    break;
                case GREEN:
                    m_green = dynamic_cast<const Double&>(value);
                    break;
                case BLUE:
                    m_blue = dynamic_cast<const Double&>(value);
                    break;
                default:
                    throw WrongParameterId(id, *this);
                }
            }
            catch(std::bad_cast&)
            {
                throw WrongParameterId(id, *this);
            }
        }

        const Data& AdjustRgbChannels::getParameter(const unsigned int id) const
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
            WriteAccess<Image> access(container);
            Image& image = access();

            cv::Mat cvImage = getOpenCvMat(image);
            
            switch(image.pixelType())
            {
            case core::Image::RGB_24:
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
            case core::Image::BGR_24:
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
        
        const std::vector<const core::Description*> AdjustRgbChannels::setupInputs()
        {
            std::vector<const Description*> inputs;
            
            Description* input = new Description(INPUT, DataVariant::COLOR_IMAGE);
            input->setName("Input");
            inputs.push_back(input);
            
            return inputs;
        }
        
        const std::vector<const Description*> AdjustRgbChannels::setupOutputs()
        {
            std::vector<const Description*> outputs;
            
            Description* output = new Description(OUTPUT, DataVariant::COLOR_IMAGE);
            output->setName("Output");
            outputs.push_back(output);
            
            return outputs;
        }
        
        const std::vector<const Parameter*> AdjustRgbChannels::setupParameters()
        {
            std::vector<const core::Parameter*> parameters;
            
            NumericParameter<Double>* red = new NumericParameter<Double>(RED, DataVariant::DOUBLE, Double(0.0), Double::MAX);
            red->setName("Red");
            red->setAccessMode(core::Parameter::ACTIVATED_WRITE);
            parameters.push_back(red);
            
            NumericParameter<Double>* green = new NumericParameter<Double>(GREEN, DataVariant::DOUBLE, Double(0.0), Double::MAX);
            green->setName("Green");
            green->setAccessMode(core::Parameter::ACTIVATED_WRITE);
            parameters.push_back(green);
            
            NumericParameter<Double>* blue = new NumericParameter<Double>(BLUE, DataVariant::DOUBLE, Double(0.0), Double::MAX);
            blue->setName("Blue");
            blue->setAccessMode(core::Parameter::ACTIVATED_WRITE);
            parameters.push_back(blue);
                                        
            return parameters;
        }
    } 
}
