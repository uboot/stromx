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
        /** \brief Abstract image */
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

            virtual uint8_t* const buffer() = 0;
            virtual const unsigned int size() const = 0;
            virtual const unsigned int width() const = 0;
            virtual const unsigned int height() const = 0;
            virtual const unsigned int stride() const = 0;
            virtual const PixelType pixelType() const = 0;
            virtual const unsigned int pixelSize() const = 0;
            
            virtual uint8_t* const data() = 0;
            virtual const uint8_t* const data() const = 0;
            
            virtual void setVariant(const DataVariant& variant) = 0;
            
            virtual void initialize(const unsigned int width, 
                                    const unsigned int height, 
                                    const unsigned int stride, 
                                    uint8_t* const data, 
                                    const PixelType pixelType) = 0;
        };
    }
}

#endif // STROMX_CORE_IMAGE_H