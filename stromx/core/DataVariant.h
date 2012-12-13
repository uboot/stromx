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

#ifndef STROMX_CORE_DATAVARIANT_H
#define STROMX_CORE_DATAVARIANT_H

#include <string>
#include "Config.h"
#include "Documentation.h"
#include "Version.h"

namespace stromx
{
    namespace core
    {
        /** \brief %Description of a data variant. */
        class STROMX_CORE_API DataVariant
        {
            STROMX_CORE_API friend bool operator==(const DataVariant & lhs, const DataVariant & rhs);
            STROMX_CORE_API friend std::ostream& operator<< (std::ostream& out, const DataVariant & container);
            
        public:
            const static DataVariant NONE;
            const static DataVariant DATA;
            const static DataVariant TRIGGER;
            const static DataVariant PRIMITIVE;
            const static DataVariant BOOL;
            const static DataVariant ENUM;
            const static DataVariant INT;
            const static DataVariant UINT;
            const static DataVariant INT_8;
            const static DataVariant UINT_8;
            const static DataVariant INT_16;
            const static DataVariant UINT_16;
            const static DataVariant INT_32;
            const static DataVariant UINT_32;
            const static DataVariant FLOAT;
            const static DataVariant DOUBLE;
            const static DataVariant STRING;
            const static DataVariant MATRIX;
            const static DataVariant INT_MATRIX;
            const static DataVariant UINT_MATRIX;
            const static DataVariant INT_8_MATRIX;
            const static DataVariant UINT_8_MATRIX;
            const static DataVariant INT_16_MATRIX;
            const static DataVariant UINT_16_MATRIX;
            const static DataVariant INT_32_MATRIX;
            const static DataVariant UINT_32_MATRIX;
            const static DataVariant FLOAT_MATRIX;
            const static DataVariant DOUBLE_MATRIX;
            const static DataVariant IMAGE;
            const static DataVariant MONO_IMAGE;
            const static DataVariant RGB_IMAGE;
            const static DataVariant MONO_8_IMAGE;
            const static DataVariant RGB_24_IMAGE;
            const static DataVariant BGR_24_IMAGE;
            const static DataVariant BAYERBG_8_IMAGE;
            const static DataVariant BAYERGB_8_IMAGE;
            const static DataVariant MONO_16_IMAGE;
            const static DataVariant RGB_48_IMAGE;
            const static DataVariant BGR_48_IMAGE;
            
            DataVariant(const unsigned int id, const std::string & package, const Documentation & doc)
              : m_id(id),
                m_package(package),
                m_doc(doc)
            {}
            
            ~DataVariant() {}
            
            const Documentation & doc() const { return m_doc; }
            
            virtual bool isVariant(const DataVariant& type) const;
            
        protected:
            unsigned int id() const { return m_id; }
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
                DOUBLE_ID,
                STRING_ID,
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
                DOUBLE_MATRIX_ID,
                IMAGE_ID,
                MONO_IMAGE_ID,
                RGB_IMAGE_ID,
                MONO_8_IMAGE_ID,
                RGB_24_IMAGE_ID,
                BGR_24_IMAGE_ID,
                BAYERBG_8_IMAGE_ID,
                BAYERGB_8_IMAGE_ID,
                MONO_16_IMAGE_ID,
                RGB_48_IMAGE_ID,
                BGR_48_IMAGE_ID
            };
            
            unsigned int m_id;
            std::string m_package;
            Documentation m_doc;
        };
        
        STROMX_CORE_API bool operator==(const DataVariant & lhs, const DataVariant & rhs);
        STROMX_CORE_API std::ostream& operator<< (std::ostream& out, const DataVariant & container);
    }
}

#endif // STROMX_CORE_DATAVARIANT_H
