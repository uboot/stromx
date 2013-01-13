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

#ifndef STROMX_EXAMPLE_DILATE_H
#define STROMX_EXAMPLE_DILATE_H

#include "MorphologicalFilter.h"

namespace stromx
{
    namespace example
    {
        /** \brief Dilates the input image. */
        class STROMX_EXAMPLE_API Dilate : public MorphologicalFilter
        {
        public:
            Dilate();
            virtual OperatorKernel* clone() const { return new Dilate; }
            
        private:
            virtual void applyMorphologicalFilter(const cv::Mat & in, cv::Mat & out,
                                                  const cv::Mat& kernel, int iterations);
            static const std::string TYPE;
        };
    }
}

#endif // STROMX_EXAMPLE_DILATE_H
