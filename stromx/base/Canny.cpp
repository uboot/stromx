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

#include "Canny.h"
#include "Image.h"
#include "Utilities.h"
#include <opencv2/imgproc/imgproc.hpp>
#include <stromx/core/DataContainer.h>
#include <stromx/core/DataProvider.h>
#include <stromx/core/Id2DataComposite.h>
#include <stromx/core/Id2DataPair.h>
#include <stromx/core/OperatorException.h>
#include <stromx/core/ReadAccess.h>
#include <stromx/core/WriteAccess.h>
#include <stromx/core/NumericParameter.h>

namespace stromx
{
    using namespace core;

    namespace base
    {
        const std::string Canny::TYPE("Canny");
        
        Canny::Canny()
          : ImageFilter(TYPE)
        {
        }

        void Canny::setParameter(unsigned int id, const Data& value)
        {
            try
            {
                switch(id)
                {
                case THRESHOLD_1:
                    m_threshold1 = dynamic_cast<const Double &>(value);
                    break;
                case THRESHOLD_2:
                    m_threshold2 = dynamic_cast<const Double &>(value);
                    break;
                default:
                    ImageFilter::setParameter(id, value);
                }
            }
            catch(std::bad_cast&)
            {
                throw WrongParameterType(parameter(id), *this);
            }
        }

        const Data& Canny::getParameter(const unsigned int id) const
        {
            switch(id)
            {
            case THRESHOLD_1:
                return m_threshold1;
            case THRESHOLD_2:
                return m_threshold2;
            default:
                return ImageFilter::getParameter(id);
            }
        }  
        
        void Canny::validateSourceImage(const stromx::core::Image& source)
        {

            if(source.pixelType() != Image::MONO_8)
                throw InputError(SOURCE, *this, "Source image is not a grayscale image.");
        }
        
        const unsigned int Canny::computeDestinationSize(const stromx::core::Image& source)
        {
            return  source.width() * source.pixelSize() * source.height();
        }

        void Canny::applyFilter(const cv::Mat& in, cv::Mat& out)
        {
            cv::Canny(in, out, m_threshold1, m_threshold2);
        }
        
        const std::vector<const Parameter*> Canny::setupParameters()
        {
            std::vector<const core::Parameter*> parameters;
            
            Parameter* threshold1 = new NumericParameter<Double>(THRESHOLD_1, DataVariant::DOUBLE);
            threshold1->setDoc("Threshold 1");
            threshold1->setAccessMode(core::Parameter::ACTIVATED_WRITE);
            parameters.push_back(threshold1);
            
            Parameter* threshold2 = new NumericParameter<Double>(THRESHOLD_2, DataVariant::DOUBLE);
            threshold2->setDoc("Threshold 2");
            threshold2->setAccessMode(core::Parameter::ACTIVATED_WRITE);
            parameters.push_back(threshold2);
                                        
            return parameters;
        }
    } 
}