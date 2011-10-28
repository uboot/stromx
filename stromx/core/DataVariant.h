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

#ifndef STROM_DATAVARIANT_H
#define STROM_DATAVARIANT_H

#include <string>
#include "Version.h"

namespace stromx
{
    namespace core
    {
        /** \brief %Description of a data variant */
        class DataVariant
        {
        public:
            const static DataVariant NONE;
            const static DataVariant DATA;
            const static DataVariant TRIGGER;
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
            
            explicit DataVariant(const unsigned int id, const std::string & package, const std::string & description)
            : m_id(id),
                m_package(package),
                m_description(description)
            {}
            
            ~DataVariant() {}
            
            const std::string & description() const { return m_description; }
            
            virtual const bool is(const DataVariant& type) const;
            
        protected:
            const unsigned int id() const { return m_id; }
            const std::string & package() const { return m_package; }
        
        private: 
            enum Types
            {
                NONE_ID,
                DATA_ID,
                TRIGGER_ID,
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
            std::string m_description;
        };
    }
}

#endif // STROM_DATAVARIANT_H
