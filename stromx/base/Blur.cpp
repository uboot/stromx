/* 
*  Copyright 2012 Matthias Fuchs
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

#include "Blur.h"
#include "Image.h"
#include "Utilities.h"
#include <opencv2/imgproc/imgproc.hpp>
#include <stromx/core/DataContainer.h>
#include <stromx/core/DataProvider.h>
#include <stromx/core/EnumParameter.h>
#include <stromx/core/Id2DataComposite.h>
#include <stromx/core/Id2DataPair.h>
#include <stromx/core/NumericParameter.h>
#include <stromx/core/OperatorException.h>
#include <stromx/core/ReadAccess.h>
#include <stromx/core/WriteAccess.h>

namespace stromx
{
    using namespace core;

    namespace base
    {
        const std::string Blur::TYPE("Blur");
        
        Blur::Blur()
          : ImageFilter(TYPE),
            m_filterType(GAUSSIAN),
            m_kernelSize(3)
        {
        }

        void Blur::setParameter(unsigned int id, const Data& value)
        {
            try
            {
                switch(id)
                {
                case FILTER_TYPE:
                    m_filterType = data_cast<Enum>(value);
                    break;
                case KERNEL_SIZE:
                    if(data_cast<UInt32>(value) % 2 == 0)
                    {
                        throw WrongParameterValue(parameter(KERNEL_SIZE), *this,
                                                  "Kernel size must be an odd value.");
                    }
                    
                    m_kernelSize = data_cast<UInt32>(value);
                    break;
                default:
                    ImageFilter::setParameter(id, value);
                }
            }
            catch(BadCast&)
            {
                throw WrongParameterType(parameter(id), *this);
            }
        }

        const DataRef Blur::getParameter(const unsigned int id) const
        {
            switch(id)
            {
            case FILTER_TYPE:
                return m_filterType;
            case KERNEL_SIZE:
                return m_kernelSize;
            default:
                return ImageFilter::getParameter(id);
            }
        }  
        
        const std::vector<const Parameter*> Blur::setupParameters()
        {
            std::vector<const core::Parameter*> parameters;
            
            EnumParameter* filterType = new EnumParameter(FILTER_TYPE);
            filterType->setTitle("Filter type");
            filterType->setAccessMode(core::Parameter::ACTIVATED_WRITE);
            filterType->add(EnumDescription(Enum(MEDIAN), "Median"));
            filterType->add(EnumDescription(Enum(GAUSSIAN), "Gaussian"));
            parameters.push_back(filterType);
            
            NumericParameter<UInt32>* kernelSize = new NumericParameter<UInt32>(KERNEL_SIZE);
            kernelSize->setTitle("Kernel size");
            kernelSize->setAccessMode(core::Parameter::ACTIVATED_WRITE);
            kernelSize->setStep(UInt32(2));
            kernelSize->setMin(UInt32(1));
            parameters.push_back(kernelSize);
                                        
            return parameters;
        }
        
        void Blur::applyFilter(const cv::Mat& in, cv::Mat& out)
        {
            switch(int(m_filterType))
            {
                case MEDIAN:
                    cv::medianBlur(in, out, int(m_kernelSize));
                    break;
                case GAUSSIAN:
                    cv::GaussianBlur(in, out, cv::Size(int(m_kernelSize), int(m_kernelSize)), 0);
                    break;
                default:
                    throw WrongArgument("Unsupported filter type.");
            }
        }
        
        unsigned int Blur::computeDestinationSize(const stromx::core::Image& source)
        {
            return source.width() * source.pixelSize() * source.height();
        }

        void Blur::validateSourceImage(const stromx::core::Image& source)
        {
            switch(source.pixelType())
            {
            case Image::BAYERBG_8:
            case Image::BAYERBG_16:
            case Image::BAYERGB_8:
            case Image::BAYERGB_16:
                throw InputError(ImageFilter::SOURCE, *this, "Source image must not be a Bayer pattern image.");
            default:
                ; // everything else is valid
            };
        }
    } 
}