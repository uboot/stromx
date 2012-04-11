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

#include "Image.h"
#include "Utilities.h"
#include <stromx/core/DataContainer.h>
#include <stromx/core/Exception.h>
#include <stromx/core/Image.h>

namespace stromx
{
    using namespace core;

    namespace base
    {
        cv::Mat getOpenCvMat(const core::Image& image)
        {
            int cvType = 0;
            switch(image.pixelType())
            {
            case core::Image::RGB_24:
            case core::Image::BGR_24:
                cvType = CV_8UC3;
                break;
            case core::Image::MONO_8:
            case core::Image::BAYERBG_8:
            case core::Image::BAYERGB_8:
                cvType = CV_8UC1;
                break;
            case core::Image::NONE:
                if(image.width() == 0 && image.height() == 0)
                    return cv::Mat();
                else
                    throw core::WrongArgument("Image with no pixel type must have zero width and height.");
            default:
                throw core::WrongArgument("Unknown pixel type.");
            }
            
            uint8_t* data = const_cast<uint8_t*>(image.data());
            
            return cv::Mat(image.height(), image.width(), cvType, data, image.stride());
        }
        
        void adjustImage(const unsigned int width, const unsigned int height, const core::Image::PixelType pixelType,
                        base::Image*& image)
        {
            if(! image)
            {
                image = new base::Image(width, height, pixelType);
            }
            else
            {
                if(pixelType != image->pixelType()
                    || width != image->width()
                    || height != image->height())
                {
                    image->resize(width, height, pixelType);
                }
            }
        } 
    }
}
