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

#ifndef STROMX_RUNTIME_IMAGE_H
#define STROMX_RUNTIME_IMAGE_H

#include <stdint.h>
#include "stromx/runtime/Matrix.h"

namespace stromx
{
    namespace runtime
    {
        /** \brief Abstract image. */
        class STROMX_RUNTIME_API Image : public Matrix
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
            
            /** Returns the width of the image. */
            virtual unsigned int width() const = 0;
            
            /** Returns the height of the image. */
            virtual unsigned int height() const = 0;
            
            /** Returns the size of a line in the image in bytes. */
            virtual unsigned int stride() const = 0;
            
            /** Returns the type of the pixel data of the image. */
            virtual PixelType pixelType() const = 0;
            
            /** Returns the size of a single pixel in bytes. */
            virtual unsigned int pixelSize() const;
            
            /** Returns the number of channels of the image. */
            virtual unsigned int numChannels() const;
            
            /** Returns the size of a pixel of a single channel of the image in bytes. */
            virtual unsigned int depth() const;
            
            /** 
             * Initializes the image to the given data. Note that this function does not
             * change the image buffer but merely changes the description of the data
             * contained in the image buffer.
             */
            virtual void initializeImage(const unsigned int width, 
                                         const unsigned int height, 
                                         const unsigned int stride, 
                                         uint8_t* data, 
                                         const PixelType pixelType) = 0;
            
            /** Returns the size of a single pixel of type \c pixelType in bytes. */
            static unsigned int pixelSize(const PixelType pixelType);
                      
            /** Returns the number of channels for a given pixel type. */
            static unsigned int numChannels(const PixelType pixelType);
            
            /** Returns the size of a single channel of a given pixel type in bytes. */
            static unsigned depth(const PixelType pixelType);
            
        protected:
            /** Returns the data variant defined by the input \c pixelType. */
            static const runtime::VariantHandle dataVariantFromPixelType(const PixelType pixelType);
            
            /** Returns the value type which corresponds to the input \c pixelType. */
            static Matrix::ValueType valueTypeFromPixelType(const PixelType pixelType);
        };
        
        /** \cond */
        template <>
        class data_traits<Image>
        {
        public:
            static const VariantHandle & variant() { return Variant::IMAGE; }
        };
        /** \endcond */
    }
}

#endif // STROMX_RUNTIME_IMAGE_H