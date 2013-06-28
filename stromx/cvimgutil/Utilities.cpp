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

#include "stromx/cvimgutil/Utilities.h"

#include <stromx/runtime/DataContainer.h>
#include <stromx/runtime/Exception.h>

#include "stromx/cvimgutil/Matrix.h"
#include "stromx/cvimgutil/Image.h"

namespace stromx
{
    using namespace runtime;

    namespace cvimgutil
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
        
        runtime::Image::PixelType computeOutPixelType(const int outDdepth,
                                    const runtime::Image::PixelType inPixelType)
        {
            int inDdepth = 0;
            switch(inPixelType)
            {
            case runtime::Image::MONO_8:
            case runtime::Image::RGB_24:
            case runtime::Image::BGR_24:
                inDdepth = 0;
                break;
            case runtime::Image::MONO_16:
            case runtime::Image::RGB_48:
            case runtime::Image::BGR_48:
                inDdepth = 1;
                break;
            default:
                throw runtime::WrongArgument("Invalid input pixel type.");
            }
            
            int ddepth = 0;
            switch(outDdepth)
            {
            case -1:
                ddepth = inDdepth;
                break;
            case CV_8U:
                ddepth = 0;
                break;
            case CV_16U:
                ddepth = 1;
                break;
            default:
                throw runtime::WrongArgument("Invalid output pixel depth.");
            }
            
            int pixelType = 0;
            switch(inPixelType)
            {
            case runtime::Image::MONO_8:
            case runtime::Image::MONO_16:
                pixelType = 0;
                break;
            case runtime::Image::RGB_24:
            case runtime::Image::RGB_48:
                pixelType = 1;
                break;
            case runtime::Image::BGR_24:
            case runtime::Image::BGR_48:
                pixelType = 2;
                break;
            default:
                throw runtime::WrongArgument("Invalid input pixel type.");
            }
                
            
            // the decision table
            int table[2][3] =
            /* ddepth      0  0  0    1  1  1 
             * pixelType   0  1  2    0  1  2 */
            /* result */ {{0, 1, 2}, {3, 4, 5}};
            
            int result = table[ddepth][pixelType];
            
            switch(result)
            {
            case 0:
                return runtime::Image::MONO_8;
            case 1:
                return runtime::Image::RGB_24;
            case 2:
                return runtime::Image::BGR_24;
            case 3:
                return runtime::Image::MONO_16;
            case 4:
                return runtime::Image::RGB_48;
            case 5:
                return runtime::Image::BGR_48;
            default:
                throw runtime::InternalError("Invalid output pixel type.");
            }
        }
    }
}
