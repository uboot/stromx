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

#include "MorphologicalFilter.h"

#include <stromx/core/Image.h>
#include <stromx/core/OperatorException.h>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>

namespace stromx
{
    namespace base
    {
        using namespace core;
        
        MorphologicalFilter::MorphologicalFilter(const std::string& type)
          : ImageFilter(type),
            m_kernelShape(RECTANGLE),
            m_kernelSizeX(3),
            m_kernelSizeY(3),
            m_iterations(1)
        {
        }  
        
        const stromx::core::Data& MorphologicalFilter::getParameter(const unsigned int id) const
        {
            switch(id)
            {
            case KERNEL_SHAPE:
                return m_kernelShape;
            case KERNEL_SIZE_X:
                return m_kernelSizeX;
            case KERNEL_SIZE_Y:
                return m_kernelSizeY;
            case ITERATIONS:
                return m_iterations;
            default:
                return stromx::base::ImageFilter::getParameter(id);
            }
        }

        void MorphologicalFilter::setParameter(const unsigned int id, const stromx::core::Data& value)
        {
            try
            {
                switch(id)
                {
                case KERNEL_SHAPE:
                    m_kernelShape = data_cast<const Enum &>(value);
                    break;
                case KERNEL_SIZE_X:
                    if(data_cast<const UInt32 &>(value) < 1)
                    {
                        throw WrongParameterValue(parameter(KERNEL_SIZE_X), *this,
                                                  "Kernel size must be positive.");
                    }
                    m_kernelSizeX = data_cast<const UInt32 &>(value);
                    break;
                case KERNEL_SIZE_Y:
                    if(data_cast<const UInt32 &>(value) < 1)
                    {
                        throw WrongParameterValue(parameter(KERNEL_SIZE_Y), *this,
                                                  "Kernel size must be positive.");
                    }
                    m_kernelSizeY = data_cast<const UInt32 &>(value);
                    break;
                case ITERATIONS:
                    m_iterations = data_cast<const UInt32 &>(value);
                    break;
                default:
                    stromx::base::ImageFilter::setParameter(id, value);
                }
            }
            catch(BadCast&)
            {
                throw WrongParameterType(parameter(id), *this);
            }
        }
        
        const unsigned int MorphologicalFilter::computeDestinationSize(const stromx::core::Image& source)
        {
            return source.width() * source.pixelSize() * source.height();;
        }

        void MorphologicalFilter::applyFilter(const cv::Mat& in, cv::Mat& out)
        {
            cv::Mat kernel;
            switch(int(m_kernelShape))
            {
                case RECTANGLE:
                    kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(m_kernelSizeX, m_kernelSizeY));
                    break;
                case ELLIPSE:
                    kernel = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(m_kernelSizeX, m_kernelSizeY));
                    break;
                case CROSS:
                    kernel = cv::getStructuringElement(cv::MORPH_CROSS, cv::Size(m_kernelSizeX, m_kernelSizeY));
                    break;
                default:
                    throw WrongArgument("Unsupported kernel shape.");
            }
            
            applyMorphologicalFilter(in, out, kernel, m_iterations);
        }

        const std::vector<const Parameter*> MorphologicalFilter::setupInitParameters()
        {
            return std::vector<const Parameter*>();
        }

        void MorphologicalFilter::validateSourceImage(const stromx::core::Image& source)
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
  