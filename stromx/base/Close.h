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

#ifndef STROMX_BASE_CLOSE_H
#define STROMX_BASE_CLOSE_H

#include "MorphologicalFilter.h"

namespace stromx
{
    namespace base
    {
        /** \brief Closes the input image. */
        class STROMX_BASE_API Close : public MorphologicalFilter
        {
        public:
            Close();
            virtual OperatorKernel*const clone() const { return new Close; }
            
        private:
            virtual void applyMorphologicalFilter(const cv::Mat & in, cv::Mat & out,
                                                  const cv::Mat& kernel, int iterations);
            static const std::string TYPE;
        };
    }
}

#endif // STROMX_BASE_CLOSE_H