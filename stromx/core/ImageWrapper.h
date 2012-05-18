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

#ifndef STROMX_CORE_IMAGEWRAPPER_H
#define STROMX_CORE_IMAGEWRAPPER_H

#include "Image.h"

namespace stromx
{
        namespace core
    {
        /** \brief Concrete image without memory management. */
        class STROMX_CORE_API ImageWrapper : public Image
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
            
            virtual const DataVariant & variant() const { return m_variant; }
            
            virtual uint8_t* const buffer() { return m_buffer; }
            
            virtual const unsigned int bufferSize() const { return m_bufferSize; };
            
            virtual const unsigned int width() const { return m_width; }
            
            virtual const unsigned int height() const { return m_height; }
            
            virtual const unsigned int stride() const { return m_stride; }
            
            virtual const PixelType pixelType() const { return m_pixelType; }
            
            virtual const unsigned int pixelSize() const;
            
            virtual uint8_t* const data() { return m_data; }
            virtual const uint8_t* const data() const { return m_data; }
            
            virtual void initialize(const unsigned int width, 
                                    const unsigned int height, 
                                    const unsigned int stride, 
                                    uint8_t* const data, 
                                    const PixelType pixelType);
            
        protected:
            /** Returns the number of channels for a given pixel type. */
            static const int numChannels(const PixelType pixelType);
            
            /** Returns the size of a single channel of a given pixel type in bytes. */
            static const int depth(const PixelType pixelType);
            
            /** Sets the size of the image buffer. */
            virtual void setBufferSize(const unsigned int bufferSize);
            
            /** 
             * Sets the buffer. Note that the image data defined by width, height, pixel type
             * and data must always be contained in the image buffer.
             */
            virtual void setBuffer(uint8_t* const buffer);
            
        private:
            static const core::DataVariant dataVariantFromPixelType(const PixelType pixelType);
            
            void validate(const unsigned int width,
                        const unsigned int height,
                        const unsigned int stride,
                        const uint8_t* const data,
                        const PixelType pixelType) const;
                        
            
            unsigned int m_width;
            unsigned int m_height;
            unsigned int m_stride;
            unsigned int m_bufferSize;
            PixelType m_pixelType;
            uint8_t* m_data;
            uint8_t* m_buffer;
            DataVariant m_variant;
        };
    }
}

#endif // STROMX_CORE_IMAGEWRAPPER_H