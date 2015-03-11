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

#include "stromx/runtime/Image.h"

namespace stromx
{
    namespace runtime
    {
        unsigned int Image::pixelSize() const
        {
            return Image::pixelSize(pixelType());
        }
        
        unsigned int Image::depth() const
        {
            return Image::depth(pixelType());
        }

        unsigned int Image::numChannels() const
        {
            return Image::numChannels(pixelType());
        }
        
        unsigned int Image::numChannels(const PixelType pixelType)
        {
            switch(pixelType)
            {
            case runtime::Image::NONE:
            case runtime::Image::MONO_8:
            case runtime::Image::MONO_16:
            case runtime::Image::BAYERBG_8:
            case runtime::Image::BAYERGB_8:
            case runtime::Image::BAYERBG_16:
            case runtime::Image::BAYERGB_16:
                return 1;
            case runtime::Image::RGB_24:
            case runtime::Image::BGR_24:
            case runtime::Image::RGB_48:
            case runtime::Image::BGR_48:
                return 3;
            default:
                throw runtime::WrongArgument("Unknown pixel type.");    
            }
        }
        
        unsigned int Image::depth(const PixelType pixelType)
        {
            switch(pixelType)
            {
            case runtime::Image::NONE:
            case runtime::Image::MONO_8:
            case runtime::Image::RGB_24:
            case runtime::Image::BGR_24:
            case runtime::Image::BAYERBG_8:
            case runtime::Image::BAYERGB_8:
                return 1;
            case runtime::Image::MONO_16:
            case runtime::Image::RGB_48:
            case runtime::Image::BGR_48:
            case runtime::Image::BAYERBG_16:
            case runtime::Image::BAYERGB_16:
                return 2;
            default:
                throw runtime::WrongArgument("Unknown pixel type.");    
            }
        }
        
        Matrix::ValueType Image::valueTypeFromPixelType(const Image::PixelType pixelType)
        {
            switch(pixelType)
            {
            case runtime::Image::NONE:
                return Matrix::NONE;
            case runtime::Image::MONO_8:
            case runtime::Image::RGB_24:
            case runtime::Image::BGR_24:
            case runtime::Image::BAYERBG_8:
            case runtime::Image::BAYERGB_8:
                return Matrix::UINT_8;
            case runtime::Image::MONO_16:
            case runtime::Image::RGB_48:
            case runtime::Image::BGR_48:
            case runtime::Image::BAYERBG_16:
            case runtime::Image::BAYERGB_16:
                return Matrix::UINT_16;
            default:
                throw runtime::WrongArgument("Unknown pixel type.");    
            }
        }
        
        unsigned int Image::pixelSize(const PixelType pixelType)
        {
            return depth(pixelType) * numChannels(pixelType);
        }
        
        const runtime::VariantHandle Image::dataVariantFromPixelType(const runtime::Image::PixelType pixelType)
        {
            switch(pixelType)
            {
            case runtime::Image::NONE:
                return runtime::Variant::IMAGE;
            case runtime::Image::MONO_8:
                return runtime::Variant::MONO_8_IMAGE;
            case runtime::Image::MONO_16:
                return runtime::Variant::MONO_16_IMAGE;
            case runtime::Image::BAYERBG_8:
                return runtime::Variant::BAYERBG_8_IMAGE;
            case runtime::Image::BAYERBG_16:
                return runtime::Variant::BAYERBG_16_IMAGE;
            case runtime::Image::BAYERGB_8:
                return runtime::Variant::BAYERGB_8_IMAGE;
            case runtime::Image::BAYERGB_16:
                return runtime::Variant::BAYERGB_16_IMAGE;
            case runtime::Image::RGB_24:
                return runtime::Variant::RGB_24_IMAGE;
            case runtime::Image::RGB_48:
                return runtime::Variant::RGB_48_IMAGE;
            case runtime::Image::BGR_24:
                return runtime::Variant::BGR_24_IMAGE;
            case runtime::Image::BGR_48:
                return runtime::Variant::BGR_48_IMAGE;
            default:
                throw runtime::WrongArgument("Unknown pixel type.");  
            }
        }
    }
}
