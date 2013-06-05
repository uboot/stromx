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

#ifndef STROMX_IMGUTIL_UTILITIES_H
#define STROMX_IMGUTIL_UTILITIES_H

#include <opencv2/core/core.hpp>
#include <stromx/runtime/Image.h>

namespace stromx
{
    namespace runtime
    {
        class Image;
        class DataContainer;
    }

    namespace imgutil
    {
        class Image;
        
        /** Returns an OpenCV matrix header for \c image. */
        cv::Mat getOpenCvMat(const runtime::Image& image);
        
        /** Returns an OpenCV matrix header for \c matrix. */
        cv::Mat getOpenCvMat(const runtime::Matrix& matrix);
        
        /** 
         * Computes the stromx pixel type of the input type \c inPixelType 
         * after conversion to the OpenCV depth \c outDdepth.
         */
        runtime::Image::PixelType computeOutPixelType(const int outDdepth,
                                    const runtime::Image::PixelType inPixelType);
    }
}

#endif // STROMX_IMGUTIL_UTILITIES_H
