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


#include "Exception.h"
#include "ImageWrapper.h"

namespace stromx
{
    namespace core
    {
        ImageWrapper::ImageWrapper(const unsigned int size, uint8_t* const buffer)
          : m_width(0),
            m_height(0),
            m_stride(0),
            m_size(size),
            m_pixelType(MONO_8),
            m_data(0),
            m_buffer(buffer),
            m_variant(DataVariant::MONO_8_IMAGE)
        {
        }
        
        ImageWrapper::ImageWrapper()
          : m_width(0),
            m_height(0),
            m_stride(0),
            m_size(0),
            m_pixelType(MONO_8),
            m_data(0),
            m_buffer(0),
            m_variant(DataVariant::MONO_8_IMAGE)
        {
        }
        
        void ImageWrapper::setVariant(const core::DataVariant& variant)
        {
            m_variant = variant;
        }
        
        void ImageWrapper::setSize(const unsigned int size)
        {
            m_size = size;
        }

        void ImageWrapper::setBuffer(uint8_t*const buffer)
        {
            m_buffer = buffer;
        }

        const int ImageWrapper::numChannels(const core::Image::PixelType pixelType)
        {
            switch(pixelType)
            {
            case core::Image::MONO_8:
            case core::Image::MONO_16:
            case core::Image::BAYERBG_8:
            case core::Image::BAYERGB_8:
            case core::Image::BAYERBG_16:
            case core::Image::BAYERGB_16:
                return 1;
            case core::Image::RGB_24:
            case core::Image::BGR_24:
            case core::Image::RGB_48:
            case core::Image::BGR_48:
                return 3;
            default:
                throw core::WrongArgument("Unknown pixel type.");    
            }
        }
        
        const int ImageWrapper::depth(const core::Image::PixelType pixelType)
        {
            switch(pixelType)
            {
            case core::Image::MONO_8:
            case core::Image::RGB_24:
            case core::Image::BGR_24:
            case core::Image::BAYERBG_8:
            case core::Image::BAYERGB_8:
                return 1;
            case core::Image::MONO_16:
            case core::Image::RGB_48:
            case core::Image::BGR_48:
            case core::Image::BAYERBG_16:
            case core::Image::BAYERGB_16:
                return 2;
            default:
                throw core::WrongArgument("Unknown pixel type.");    
            }
        }
        
        const unsigned int ImageWrapper::pixelSize() const
        {
            return depth(m_pixelType) * numChannels(m_pixelType);
        }
        
        
            
        void ImageWrapper::initialize(const unsigned int width, 
                                const unsigned int height, 
                                const unsigned int stride,
                                uint8_t*const data, 
                                const PixelType pixelType)
        {
            validate(width, height, stride, data, pixelType);
            
            m_width = width;
            m_height = height;
            m_stride = stride;
            m_data = data;
            m_pixelType = pixelType;
        }
        
        void ImageWrapper::validate(const unsigned int width,
                                    const unsigned int height,
                                    const unsigned int stride,
                                    const uint8_t*const data,
                                    const core::Image::PixelType pixelType) const
        {
            if(width == 0 || height == 0)
                return;
            
            // check row length
            if(width * depth(pixelType) * numChannels(pixelType) > stride)
                throw WrongArgument("Too small stride.");
            
            // check total data size
            unsigned int dataSize = stride * (height - 1) + width;
            if(data + dataSize > m_buffer + m_size)
                throw WrongArgument("Too small buffer.");
        }
    }
}
