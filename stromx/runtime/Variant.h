/* 
 *  Copyright 2015 Matthias Fuchs
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

#ifndef STROMX_RUNTIME_VARIANT_H
#define STROMX_RUNTIME_VARIANT_H

#include "stromx/runtime/VariantHandle.h"

namespace stromx
{
    namespace runtime
    {
        /** \brief %VariantInterface implementation of the runtime package. */
        class STROMX_RUNTIME_API Variant : public VariantInterface
        {            
        public:
            /** A subclass of Data which contains no data. */
            const static VariantHandle NONE;
            
            /** Any subclass of Data. */
            const static VariantHandle DATA;
            
            /** A Trigger object. */
            const static VariantHandle TRIGGER;
            
            /** Any Primitive object. */
            const static VariantHandle PRIMITIVE;
            
            /** A Bool object. */
            const static VariantHandle BOOL;
            
            /** An Enum object. */
            const static VariantHandle ENUM;
            
            /** Any of UInt8, UInt16, UInt32, Int8, Int16 or Int32. */
            const static VariantHandle INT;
            
            /** Any of UInt8, UInt16 or UInt32. */
            const static VariantHandle UINT;
            
            /** An Int8 object. */
            const static VariantHandle INT_8;
            
            /** An UInt8 object. */
            const static VariantHandle UINT_8;
            
            /** An Int16 object. */
            const static VariantHandle INT_16;
            
            /** An UInt16 object. */
            const static VariantHandle UINT_16;
            
            /** An Int32 object. */
            const static VariantHandle INT_32;
            
            /** An UInt32 object. */
            const static VariantHandle UINT_32;
            
            /** An Int64 object. */
            const static VariantHandle INT_64;
            
            /** An UInt64 object. */
            const static VariantHandle UINT_64;
            
            /** Any of Float32 or Float64. */
            const static VariantHandle FLOAT;
            
            /** A Float32 object. */
            const static VariantHandle FLOAT_32;
            
            /** A Float64 object. */
            const static VariantHandle FLOAT_64;
            
            /** A String object. */
            const static VariantHandle STRING;
            
            /** An List object. */
            const static VariantHandle LIST;
            
            /** Any Matrix object. */
            const static VariantHandle MATRIX;
            
            /** 
             * A Matrix object with value type Matrix::INT_8, Matrix::INT_16,
             * Matrix::INT_32, Matrix::UINT_8, Matrix::UINT_16 or
             * Matrix::UINT_32.
             */
            const static VariantHandle INT_MATRIX;
            
            /** A Matrix object with value type Matrix::UINT_8, Matrix::UINT_16 or Matrix::UINT_32. */
            const static VariantHandle UINT_MATRIX;
            
            /** A Matrix object with Matrix::INT_8 value type. */
            const static VariantHandle INT_8_MATRIX;
            
            /** A Matrix object with Matrix::UINT_8 value type. */
            const static VariantHandle UINT_8_MATRIX;
            
            /** A Matrix object with Matrix::INT_16 value type. */
            const static VariantHandle INT_16_MATRIX;
            
            /** A Matrix object with Matrix::UINT_16 value type. */
            const static VariantHandle UINT_16_MATRIX;
            
            /** A Matrix object with Matrix::INT_32 value type. */
            const static VariantHandle INT_32_MATRIX;
            
            /** A Matrix object with Matrix::UINT_32 value type. */
            const static VariantHandle UINT_32_MATRIX;
            
            /** A Matrix object with value type Matrix::FLOAT_32 or Matrix::FLOAT_64. */
            const static VariantHandle FLOAT_MATRIX;
            
            /** A Matrix object with Matrix::FLOAT_32 value type. */
            const static VariantHandle FLOAT_32_MATRIX;
            
            /** A Matrix object with Matrix::FLOAT_64 value type. */
            const static VariantHandle FLOAT_64_MATRIX;
            
            /** Any Image object. */
            const static VariantHandle IMAGE;
            
            /** An Image object with pixel type Image::MONO_8 or Image::MONO_16. */
            const static VariantHandle MONO_IMAGE;
            
            /** 
             * An Image object with pixel type Image::RGB_24, Image::BGR_24, 
             * Image::RGB_48 or Image::BGR_48.
             */
            const static VariantHandle RGB_IMAGE;
            
            /** An Image object with Image::MONO_8 pixel type. */
            const static VariantHandle MONO_8_IMAGE;
            
            /** An Image object with Image::RGB_24 pixel type. */
            const static VariantHandle RGB_24_IMAGE;
            
            /** An Image object with Image::BGR_24 pixel type. */
            const static VariantHandle BGR_24_IMAGE;
            
            /** An Image object with Image::BAYERBG_8 pixel type. */
            const static VariantHandle BAYERBG_8_IMAGE;
            
            /** An Image object with Image::BAYERGB_8 pixel type. */
            const static VariantHandle BAYERGB_8_IMAGE;
            
            /** An Image object with Image::BAYERBG_16 pixel type. */
            const static VariantHandle BAYERBG_16_IMAGE;
            
            /** An Image object with Image::BAYERGB_16 pixel type. */
            const static VariantHandle BAYERGB_16_IMAGE;
            
            /** An Image object with Image::MONO_16 pixel type. */
            const static VariantHandle MONO_16_IMAGE;
            
            /** An Image object with Image::RGB_48 pixel type. */
            const static VariantHandle RGB_48_IMAGE;
            
            /** An Image object with Image::BGR_48 pixel type. */
            const static VariantHandle BGR_48_IMAGE;
            
            /** A file object. */
            const static VariantHandle FILE;
            
            /** A file object containing binary data. */
            const static VariantHandle BINARY_FILE;
            
            /** A file object containing text data. */
            const static VariantHandle TEXT_FILE;
            
            unsigned int id() const { return m_id; }
            
            const std::string & package() const { return m_package; }
            
            virtual const std::string title() const;
            
            virtual bool isVariant(const VariantInterface& variant) const;
        
        private:
            enum Types
            {
                NONE_ID,
                DATA_ID,
                TRIGGER_ID,
                PRIMITIVE_ID,
                BOOL_ID,
                ENUM_ID,
                INT_ID,
                UINT_ID,
                INT_8_ID,
                UINT_8_ID,
                INT_16_ID,
                UINT_16_ID,
                INT_32_ID,
                UINT_32_ID,
                INT_64_ID,
                UINT_64_ID,
                FLOAT_ID,
                FLOAT_32_ID,
                FLOAT_64_ID,
                STRING_ID,
                LIST_ID,
                MATRIX_ID,
                INT_MATRIX_ID,
                UINT_MATRIX_ID,
                INT_8_MATRIX_ID,
                UINT_8_MATRIX_ID,
                INT_16_MATRIX_ID,
                UINT_16_MATRIX_ID,
                INT_32_MATRIX_ID,
                UINT_32_MATRIX_ID,
                FLOAT_MATRIX_ID,
                FLOAT_32_MATRIX_ID,
                FLOAT_64_MATRIX_ID,
                IMAGE_ID,
                MONO_IMAGE_ID,
                RGB_IMAGE_ID,
                MONO_8_IMAGE_ID,
                RGB_24_IMAGE_ID,
                BGR_24_IMAGE_ID,
                BAYERBG_8_IMAGE_ID,
                BAYERGB_8_IMAGE_ID,
                BAYERBG_16_IMAGE_ID,
                BAYERGB_16_IMAGE_ID,
                MONO_16_IMAGE_ID,
                RGB_48_IMAGE_ID,
                BGR_48_IMAGE_ID,
                FILE_ID,
                BINARY_FILE_ID,
                TEXT_FILE_ID             
            };
            
            Variant(const unsigned int id)
              : m_id(id),
                m_package(STROMX_RUNTIME_PACKAGE_NAME)
            {}
            
            unsigned int m_id;
            std::string m_package;
        };
    }
}

#endif // STROMX_RUNTIME_VARIANT_H
