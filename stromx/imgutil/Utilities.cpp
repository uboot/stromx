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

#include "stromx/imgutil/Matrix.h"
#include "stromx/imgutil/Image.h"
#include "stromx/imgutil/Utilities.h"
#include <stromx/runtime/DataContainer.h>
#include <stromx/runtime/Exception.h>

namespace stromx
{
    using namespace runtime;

    namespace imgutil
    {
        cv::Mat getOpenCvMat(const runtime::Image& image)
        {
            int cvType = Image::cvTypeFromPixelType(image.pixelType());
            uint8_t* data = const_cast<uint8_t*>(image.data());
            
            return cv::Mat(image.height(), image.width(), cvType, data, image.stride());
        }
        
        cv::Mat getOpenCvMat(const runtime::Matrix& matrix)
        {
            int cvType = Matrix::cvTypeFromValueType(matrix.valueType());
            uint8_t* data = const_cast<uint8_t*>(matrix.data());
            
            return cv::Mat(matrix.cols(), matrix.rows(), cvType, data, matrix.stride());
        }
    }
}
