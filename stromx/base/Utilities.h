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

#ifndef STROMX_BASE_UTILITIES_H
#define STROMX_BASE_UTILITIES_H

#include <opencv2/opencv.hpp>
#include <stromx/core/Image.h>

namespace stromx
{
    namespace core
    {
        class Image;
        class DataContainer;
    }

    namespace base
    {
        class Image;
        
        cv::Mat getOpenCvMat(const core::Image& image);
        void adjustImage(const unsigned int width, const unsigned int height, const core::Image::PixelType pixelType,
                        base::Image*& image);
    }
}

#endif // STROMX_BASE_UTILITIES_H
