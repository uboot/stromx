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

#include "stromx/runtime/Exception.h"
#include "stromx/runtime/ImageWrapper.h"

namespace stromx
{
    namespace runtime
    {
        ImageWrapper::ImageWrapper(const unsigned int bufferSize, uint8_t* const buffer)
          : m_width(0),
            m_height(0),
            m_cols(0),
            m_stride(0),
            m_bufferSize(bufferSize),
            m_pixelType(Image::NONE),
            m_valueType(Matrix::NONE),
            m_data(0),
            m_buffer(buffer),
            m_variant(Variant::IMAGE)
        {
        }
        
        ImageWrapper::ImageWrapper()
          : m_width(0),
            m_height(0),
            m_cols(0),
            m_stride(0),
            m_bufferSize(0),
            m_pixelType(Image::NONE),
            m_valueType(Matrix::NONE),
            m_data(0),
            m_buffer(0),
            m_variant(Variant::IMAGE)
        {
        }

        void ImageWrapper::setBuffer(uint8_t*const buffer, const unsigned int bufferSize)
        {
            m_bufferSize = bufferSize;
            m_buffer = buffer;
        }
        
        void ImageWrapper::initializeImage(const unsigned int width, const unsigned int height, 
                                           const unsigned int stride, uint8_t*const data, 
                                           const PixelType pixelType)
        {
            validate(width, height, stride, data, pixelType);
            
            m_width = width;
            m_height = height;
            m_stride = stride;
            m_data = data;
            m_pixelType = pixelType;
            m_variant = dataVariantFromPixelType(pixelType);
            
            // set the matrix specific fields
            m_cols = m_width * numChannels();
            m_valueType = valueTypeFromPixelType(pixelType);
        }
        
        void ImageWrapper::validate(const unsigned int width,
                                    const unsigned int height,
                                    const unsigned int stride,
                                    const uint8_t*const data,
                                    const runtime::Image::PixelType pixelType) const
        {
            if(width == 0 || height == 0)
                return;
            
            // check row length
            if(width * depth(pixelType) * numChannels(pixelType) > stride)
                throw WrongArgument("Too small stride.");
            
            // check total data size
            unsigned int dataSize = stride * (height - 1) + width;
            if(data + dataSize > m_buffer + m_bufferSize)
                throw WrongArgument("Too small buffer.");
        }
        
        unsigned int ImageWrapper::valueSize() const
        {
            return Matrix::valueSize(valueType());
        }

        void ImageWrapper::initializeMatrix(const unsigned int rows, const unsigned int cols, const unsigned int stride, uint8_t*const data, const stromx::runtime::Matrix::ValueType valueType)
        {
            validate(cols * Matrix::valueSize(valueType), rows, stride, data, NONE);
            
            m_width = cols * Matrix::valueSize(valueType);
            m_height = rows;
            m_stride = stride;
            m_data = data;
            m_pixelType = NONE;
            m_variant = dataVariantFromValueType(valueType);
            
            // set the matrix specific fiels as required by the caller
            m_cols = cols;
            m_valueType = valueType;
        }
        
        void ImageWrapper::resize(const unsigned int width, const unsigned int height, const runtime::Image::PixelType pixelType)
        {
            allocate(width, height, pixelType);
        }
        
        void ImageWrapper::resize(const unsigned int size)
        {
            allocate(size, 1, runtime::Image::NONE);
        }
    }
}
