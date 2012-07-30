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

#ifndef STROMX_BASE_MORPHOLOGICALFILTER_H
#define STROMX_BASE_MORPHOLOGICALFILTER_H

#include "Config.h"
#include "ImageFilter.h"
#include <stromx/core/Enum.h>

namespace stromx
{
    namespace base
    {
        /** \brief Abstract base class of morphological filters. */
        class STROMX_BASE_API MorphologicalFilter : public ImageFilter
        {
        public:
            enum ParameterId
            {
                KERNEL_SHAPE = ImageFilter::FILTER_PARAMETERS,
                KERNEL_SIZE_X,
                KERNEL_SIZE_Y,
                ITERATIONS
            };
            
            enum KernelShape
            {
                RECTANGLE,
                ELLIPSE,
                CROSS
            };
            
            MorphologicalFilter(const std::string & type);
            
            virtual void setParameter(const unsigned int id, const core::Data& value);
            virtual const core::Data& getParameter(const unsigned int id) const;
            
        protected:
            virtual void applyMorphologicalFilter(const cv::Mat & in, cv::Mat & out,
                                                  const cv::Mat & kernel, int iterations) = 0;
            
        private:
            const std::vector<const core::Parameter*> setupParameters();
            void applyFilter(const cv::Mat & in, cv::Mat & out);
            void validateSourceImage(const core::Image & source);
            const unsigned int computeDestinationSize(const core::Image & source);
            
            core::Enum m_kernelShape;
            core::UInt32 m_kernelSizeX;
            core::UInt32 m_kernelSizeY;
            core::UInt32 m_iterations;
        };
    }
}

#endif // STROMX_BASE_MORPHOLOGICALFILTER_H

