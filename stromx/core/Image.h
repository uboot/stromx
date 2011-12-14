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

#ifndef STROMX_CORE_IMAGE_H
#define STROMX_CORE_IMAGE_H

#include <stdint.h>
#include "Data.h"

namespace stromx
{
    namespace core
    {
        /** \brief Abstract image. */
        class STROMX_CORE_API Image : public Data
        {
        public:
            enum PixelType
            {
                NONE,
                MONO_8,
                MONO_16,
                RGB_24,
                RGB_48,
                BGR_24,
                BGR_48,
                BAYERBG_8,
                BAYERBG_16,
                BAYERGB_8,
                BAYERGB_16
            };
            
            virtual ~Image() {}

            /** 
             * Returns the address of the buffer of the image. This is not necessarily
             * a pointer to the image data which can be located at a different position
             * in the buffer.
             */
            virtual uint8_t* const buffer() = 0;
            
            /** Returns the size of the buffer. */
            virtual const unsigned int bufferSize() const = 0;
            
            /** Returns the width of the image. */
            virtual const unsigned int width() const = 0;
            
            /** Returns the height of the image. */
            virtual const unsigned int height() const = 0;
            
            /** Returns the size of a line in the image in bytes. */
            virtual const unsigned int stride() const = 0;
            
            /** Returns the type of the pixel data of the image. */
            virtual const PixelType pixelType() const = 0;
            
            /** Returns the size of a single pixel in bytes. */
            virtual const unsigned int pixelSize() const = 0;
            
            /** Returns the address of the image data. */
            virtual uint8_t* const data() = 0;
            
            /** Returns the address of the image data as a constant pointer. */
            virtual const uint8_t* const data() const = 0;
            
            /** Sets the data variant. */
            virtual void setVariant(const DataVariant& variant) = 0;
            
            /** 
             * Initializes the image to the given data. Note that this function does not
             * change the image buffer but merely changes the description of the data
             * contained in the image buffer.
             */
            virtual void initialize(const unsigned int width, 
                                    const unsigned int height, 
                                    const unsigned int stride, 
                                    uint8_t* const data, 
                                    const PixelType pixelType) = 0;
        protected:
        };
    }
}

#endif // STROMX_CORE_IMAGE_H