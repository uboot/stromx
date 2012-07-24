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

#ifndef STROMX_BASE_BLUR_H
#define STROMX_BASE_BLUR_H

#include "Config.h"
#include "ImageFilter.h"

struct _IplImage;

namespace cv
{
    class Mat;
}

namespace stromx
{
    namespace base
    {
        /** \brief Blurs the input image. */
        class STROMX_BASE_API Blur : public ImageFilter
        {
        public:
            enum ParameterId
            {
                FILTER_TYPE = ImageFilter::FILTER_PARAMETERS,
                KERNEL_SIZE
            };
            
            enum FilterType
            {
                MEDIAN,
                GAUSSIAN
            };
            
            Blur();
            
            virtual OperatorKernel*const clone() const { return new Blur; }
            virtual void setParameter(const unsigned int id, const core::Data& value);
            virtual const core::Data& getParameter(const unsigned int id) const;
            
        protected:
            virtual const std::vector<const core::Parameter*> setupInitParameters();
            virtual void applyFilter(const cv::Mat & in, cv::Mat & out);
            virtual void validateSourceImage(const core::Image & source);
            virtual const unsigned int computeDestinationSize(const core::Image & source);
            
        private:
            static const std::string TYPE;
            
            core::Enum m_filterType;
            core::UInt32 m_kernelSize;
        };
    }
}

#endif // STROMX_BASE_BLUR_H
