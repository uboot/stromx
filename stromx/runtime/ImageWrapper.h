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

#ifndef STROMX_RUNTIME_IMAGEWRAPPER_H
#define STROMX_RUNTIME_IMAGEWRAPPER_H

#include "stromx/runtime/Image.h"

namespace stromx
{
        namespace runtime
    {
        /** \brief Concrete image without memory management. */
        class STROMX_RUNTIME_API ImageWrapper : public Image
        {
        public:
            /** 
             * Constructs an image wrapper from a given memory buffer. The buffer is not
             * owned by the image and is thus not freed upon destruction of the image wrapper.
             */
            ImageWrapper(const unsigned int bufferSize, uint8_t* const buffer);
            
            /** 
             * Constructs an empty image wrapper, i.e. is an image wrapper without associated
             * memory buffer.
             */
            ImageWrapper();
            
            // Implementation of stromx::runtime::Matrix
            virtual const VariantHandle & variant() const { return m_variant; }
            virtual uint8_t* buffer() { return m_buffer; }
            virtual unsigned int bufferSize() const { return m_bufferSize; };
            virtual unsigned int rows() const { return m_height; }
            virtual unsigned int cols() const { return m_cols; }
            virtual uint8_t* data() { return m_data; }
            virtual const uint8_t* data() const { return m_data; }
            virtual unsigned int stride() const { return m_stride; }
            virtual ValueType valueType() const { return m_valueType; }
            virtual unsigned int valueSize() const;
            virtual void initializeMatrix(const unsigned int rows, 
                                          const unsigned int cols, 
                                          const unsigned int stride, 
                                          uint8_t* data, 
                                          const ValueType valueType);
            
            // Implementation of stromx::runtime::Image
            virtual unsigned int width() const { return m_width; }
            virtual unsigned int height() const { return m_height; }
            virtual PixelType pixelType() const { return m_pixelType; }
            virtual void initializeImage(const unsigned int width, 
                                         const unsigned int height, 
                                         const unsigned int stride, 
                                         uint8_t* data, 
                                         const PixelType pixelType);
            
            /** Resizes the image and changes the pixel type of the image. */
            void resize(const unsigned int width, const unsigned int height, const PixelType pixelType);
            
            /**
             * Resizes the buffer of the image to the given size in bytes. The width of the resized image is
             * \c size and its height is 1. Its pixel type is Image::NONE. The buffer size of the image is 
             * guaranteed to be at least \c size.
             */
            void resize(const unsigned int size);
            
        protected:
            /** 
             * Sets a new image buffer. Note that the image data defined by width, height, pixel type
             * and data must always be contained in the image buffer.
             * 
             * \param buffer The new buffer. Note that this memory is \em not released by
             *               the ImageWrapper.
             * \param bufferSize The size of \a buffer in bytes.
             */
            void setBuffer(uint8_t* const buffer, const unsigned int bufferSize);
            
            /**
             * Allocates or resizes the image to the given dimension. Must be defined
             * in derived class which implements the memory management. Implementations
             * of this function should call setBuffer() and initializeImage().
             * 
             * \since 0.2
             */
            virtual void allocate(const unsigned int width, const unsigned int height,
                                  const Image::PixelType pixelType) = 0;
        
            
        private:            
            void validate(const unsigned int width,
                          const unsigned int height,
                          const unsigned int stride,
                          const uint8_t* data,
                          const PixelType pixelType) const;
                        
            
            unsigned int m_width;
            unsigned int m_height;
            unsigned int m_cols;
            unsigned int m_stride;
            unsigned int m_bufferSize;
            PixelType m_pixelType;
            ValueType m_valueType;
            uint8_t* m_data;
            uint8_t* m_buffer;
            VariantHandle m_variant;
        };
    }
}

#endif // STROMX_RUNTIME_IMAGEWRAPPER_H