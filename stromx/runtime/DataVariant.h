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

#ifndef STROMX_RUNTIME_DATAVARIANT_H
#define STROMX_RUNTIME_DATAVARIANT_H

#include <string>
#include "stromx/runtime/Config.h"
#include "stromx/runtime/Version.h"

namespace stromx
{
    namespace runtime
    {
        /** \brief %Description of a data variant.
         *
         * A data variant describes type of a Data object, i.e. which subclass of Data the object
         * is an instance of. Some data variants give further details about the data object, e.g the
         * type of the entries of a Matrix object (their type is not characterized by the class type
         * but by a class member).
         */
        class STROMX_RUNTIME_API DataVariant
        {            
        public:
            /** A subclass of Data which contains no data. */
            const static DataVariant NONE;
            
            /** Any subclass of Data. */
            const static DataVariant DATA;
            
            /** A Trigger object. */
            const static DataVariant TRIGGER;
            
            /** Any Primitive object. */
            const static DataVariant PRIMITIVE;
            
            /** A Bool object. */
            const static DataVariant BOOL;
            
            /** An Enum object. */
            const static DataVariant ENUM;
            
            /** Any of UInt8, UInt16, UInt32, Int8, Int16 or Int32. */
            const static DataVariant INT;
            
            /** Any of UInt8, UInt16 or UInt32. */
            const static DataVariant UINT;
            
            /** An Int8 object. */
            const static DataVariant INT_8;
            
            /** An UInt8 object. */
            const static DataVariant UINT_8;
            
            /** An Int16 object. */
            const static DataVariant INT_16;
            
            /** An UInt16 object. */
            const static DataVariant UINT_16;
            
            /** An Int32 object. */
            const static DataVariant INT_32;
            
            /** An UInt32 object. */
            const static DataVariant UINT_32;
            
            /** Any of Float32 or Float64. */
            const static DataVariant FLOAT;
            
            /** A Float32 object. */
            const static DataVariant FLOAT_32;
            
            /** A Float64 object. */
            const static DataVariant FLOAT_64;
            
            /** A String object. */
            const static DataVariant STRING;
            
            /** An List object. */
            const static DataVariant LIST;
            
            /** Any Matrix object. */
            const static DataVariant MATRIX;
            
            /** 
             * A Matrix object with value type Matrix::INT_8, Matrix::INT_16,
             * Matrix::INT_32, Matrix::UINT_8, Matrix::UINT_16 or
             * Matrix::UINT_32.
             */
            const static DataVariant INT_MATRIX;
            
            /** A Matrix object with value type Matrix::UINT_8, Matrix::UINT_16 or Matrix::UINT_32. */
            const static DataVariant UINT_MATRIX;
            
            /** A Matrix object with Matrix::INT_8 value type. */
            const static DataVariant INT_8_MATRIX;
            
            /** A Matrix object with Matrix::UINT_8 value type. */
            const static DataVariant UINT_8_MATRIX;
            
            /** A Matrix object with Matrix::INT_16 value type. */
            const static DataVariant INT_16_MATRIX;
            
            /** A Matrix object with Matrix::UINT_16 value type. */
            const static DataVariant UINT_16_MATRIX;
            
            /** A Matrix object with Matrix::INT_32 value type. */
            const static DataVariant INT_32_MATRIX;
            
            /** A Matrix object with Matrix::UINT_32 value type. */
            const static DataVariant UINT_32_MATRIX;
            
            /** A Matrix object with value type Matrix::FLOAT_32 or Matrix::FLOAT_64. */
            const static DataVariant FLOAT_MATRIX;
            
            /** A Matrix object with Matrix::FLOAT_32 value type. */
            const static DataVariant FLOAT_32_MATRIX;
            
            /** A Matrix object with Matrix::FLOAT_64 value type. */
            const static DataVariant FLOAT_64_MATRIX;
            
            /** Any Image object. */
            const static DataVariant IMAGE;
            
            /** An Image object with pixel type Image::MONO_8 or Image::MONO_16. */
            const static DataVariant MONO_IMAGE;
            
            /** 
             * An Image object with pixel type Image::RGB_24, Image::BGR_24, 
             * Image::RGB_48 or Image::BGR_48.
             */
            const static DataVariant RGB_IMAGE;
            
            /** An Image object with Image::MONO_8 pixel type. */
            const static DataVariant MONO_8_IMAGE;
            
            /** An Image object with Image::RGB_24 pixel type. */
            const static DataVariant RGB_24_IMAGE;
            
            /** An Image object with Image::BGR_24 pixel type. */
            const static DataVariant BGR_24_IMAGE;
            
            /** An Image object with Image::BAYERBG_8 pixel type. */
            const static DataVariant BAYERBG_8_IMAGE;
            
            /** An Image object with Image::BAYERGB_8 pixel type. */
            const static DataVariant BAYERGB_8_IMAGE;
            
            /** An Image object with Image::BAYERBG_16 pixel type. */
            const static DataVariant BAYERBG_16_IMAGE;
            
            /** An Image object with Image::BAYERGB_16 pixel type. */
            const static DataVariant BAYERGB_16_IMAGE;
            
            /** An Image object with Image::MONO_16 pixel type. */
            const static DataVariant MONO_16_IMAGE;
            
            /** An Image object with Image::RGB_48 pixel type. */
            const static DataVariant RGB_48_IMAGE;
            
            /** An Image object with Image::BGR_48 pixel type. */
            const static DataVariant BGR_48_IMAGE;
            
            /** 
             * Constructs a data variant. The pair (\c package, \c id) uniquely identifies
             * the data variant.
             */
            DataVariant(const unsigned int id, const std::string & package)
              : m_id(id),
                m_package(package)
            {}
            
            ~DataVariant() {}
            
            /** Returns the title of the data variant. */
            virtual const std::string title() const;
            
            /**
             * Returns true if \c this is a subtype of \c type. E.g.
             * <tt>INT_16.isVariant(INT) == true</tt>.
             */
            virtual bool isVariant(const DataVariant& type) const;
            
            /** Returns the integer ID of this data variant. */
            unsigned int id() const { return m_id; }
            
            /** Returns the package this data variant belongs to. */
            const std::string & package() const { return m_package; }
        
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
                BGR_48_IMAGE_ID
            };
            
            unsigned int m_id;
            std::string m_package;
        };
        
        STROMX_RUNTIME_API bool operator==(const DataVariant & lhs, const DataVariant & rhs);
        STROMX_RUNTIME_API std::ostream& operator<< (std::ostream& out, const DataVariant & container);
    }
}

#endif // STROMX_RUNTIME_DATAVARIANT_H
