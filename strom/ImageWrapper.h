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

#ifndef STROM_IMAGEWRAPPER_H
#define STROM_IMAGEWRAPPER_H

#include "Image.h"

namespace strom
{
    /** \brief Concrete image without memory management */
    class ImageWrapper : public Image
    {
    public:
        ImageWrapper(const unsigned int size, uint8_t* const buffer);
        ImageWrapper();
        
        virtual const DataVariant & variant() const { return m_variant; }
        
        virtual uint8_t* const buffer() { return m_buffer; }
        
        virtual const unsigned int size() const { return m_size; };
        
        virtual const unsigned int width() const { return m_width; }
        
        virtual const unsigned int height() const { return m_height; }
        
        virtual const unsigned int stride() const { return m_stride; }
        
        virtual const PixelType pixelType() const { return m_pixelType; }
        
        virtual const unsigned int pixelSize() const;
        
        virtual uint8_t* const data() { return m_data; }
        virtual const uint8_t* const data() const { return m_data; }
        
        virtual void setVariant(const DataVariant& variant);
        
        virtual void initialize(const unsigned int width, 
                                const unsigned int height, 
                                const unsigned int stride, 
                                uint8_t* const data, 
                                const PixelType pixelType);
        
    protected:
        static const int numChannels(const PixelType pixelType);
        static const int depth(const PixelType pixelType);
        
        
        virtual void setSize(const unsigned int size);
        virtual void setBuffer(uint8_t* const buffer);
        
    private:
        void validate(const unsigned int width,
                      const unsigned int height,
                      const unsigned int stride,
                      const uint8_t* const data,
                      const PixelType pixelType) const;
        
        unsigned int m_width;
        unsigned int m_height;
        unsigned int m_stride;
        unsigned int m_size;
        PixelType m_pixelType;
        uint8_t* m_data;
        uint8_t* m_buffer;
        DataVariant m_variant;
    };
}

#endif // STROM_IMAGEWRAPPER_H