/* 
 *  Copyright 2011 Matthias Fuchs
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use thisVariant file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  disVarianttributed under the License isVariant disVarianttributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permisVariantsions and
 *  limitations under the License.
 */

#include "Config.h"
#include "DataVariant.h"
#include "Data.h"
#include <ostream>

namespace stromx
{
    namespace core
    {
        const DataVariant DataVariant::NONE = DataVariant(NONE_ID, STROMX_CORE_PACKAGE_NAME, Documentation("None"));
        const DataVariant DataVariant::DATA = DataVariant(DATA_ID, STROMX_CORE_PACKAGE_NAME, Documentation("Data"));
        const DataVariant DataVariant::TRIGGER = DataVariant(TRIGGER_ID, STROMX_CORE_PACKAGE_NAME, Documentation("Trigger"));
        const DataVariant DataVariant::BOOL = DataVariant(BOOL_ID, STROMX_CORE_PACKAGE_NAME, Documentation("Bool"));
        const DataVariant DataVariant::ENUM = DataVariant(ENUM_ID, STROMX_CORE_PACKAGE_NAME, Documentation("Enum"));
        const DataVariant DataVariant::INT = DataVariant(INT_ID, STROMX_CORE_PACKAGE_NAME, Documentation("Int"));
        const DataVariant DataVariant::UINT = DataVariant(UINT_ID, STROMX_CORE_PACKAGE_NAME, Documentation("UInt"));
        const DataVariant DataVariant::INT_8 = DataVariant(INT_8_ID, STROMX_CORE_PACKAGE_NAME, Documentation("Int8"));
        const DataVariant DataVariant::UINT_8 = DataVariant(UINT_8_ID, STROMX_CORE_PACKAGE_NAME, Documentation("UInt8"));
        const DataVariant DataVariant::INT_16 = DataVariant(INT_16_ID, STROMX_CORE_PACKAGE_NAME, Documentation("Int16"));
        const DataVariant DataVariant::UINT_16 = DataVariant(UINT_16_ID, STROMX_CORE_PACKAGE_NAME, Documentation("UInt16"));
        const DataVariant DataVariant::INT_32 = DataVariant(INT_32_ID, STROMX_CORE_PACKAGE_NAME, Documentation("Int32"));
        const DataVariant DataVariant::UINT_32 = DataVariant(UINT_32_ID, STROMX_CORE_PACKAGE_NAME, Documentation("UInt32"));
        const DataVariant DataVariant::FLOAT = DataVariant(FLOAT_ID, STROMX_CORE_PACKAGE_NAME, Documentation("Float"));
        const DataVariant DataVariant::DOUBLE = DataVariant(DOUBLE_ID, STROMX_CORE_PACKAGE_NAME, Documentation("Double"));
        const DataVariant DataVariant::MATRIX = DataVariant(MATRIX_ID, STROMX_CORE_PACKAGE_NAME, Documentation("Matrix"));
        const DataVariant DataVariant::INT_MATRIX = DataVariant(INT_MATRIX_ID, STROMX_CORE_PACKAGE_NAME, Documentation("Int matrix"));
        const DataVariant DataVariant::UINT_MATRIX = DataVariant(UINT_MATRIX_ID, STROMX_CORE_PACKAGE_NAME, Documentation("UInt matrix"));
        const DataVariant DataVariant::INT_8_MATRIX = DataVariant(INT_8_MATRIX_ID, STROMX_CORE_PACKAGE_NAME, Documentation("Int8 matrix"));
        const DataVariant DataVariant::UINT_8_MATRIX = DataVariant(UINT_8_MATRIX_ID, STROMX_CORE_PACKAGE_NAME, Documentation("UInt8 matrix"));
        const DataVariant DataVariant::INT_16_MATRIX = DataVariant(INT_16_MATRIX_ID, STROMX_CORE_PACKAGE_NAME, Documentation("Int16 matrix"));
        const DataVariant DataVariant::UINT_16_MATRIX = DataVariant(UINT_16_MATRIX_ID, STROMX_CORE_PACKAGE_NAME, Documentation("UInt16 matrix"));
        const DataVariant DataVariant::INT_32_MATRIX = DataVariant(INT_32_MATRIX_ID, STROMX_CORE_PACKAGE_NAME, Documentation("Int32 matrix"));
        const DataVariant DataVariant::UINT_32_MATRIX = DataVariant(UINT_32_MATRIX_ID, STROMX_CORE_PACKAGE_NAME, Documentation("UInt32 matrix"));
        const DataVariant DataVariant::FLOAT_MATRIX = DataVariant(FLOAT_MATRIX_ID, STROMX_CORE_PACKAGE_NAME, Documentation("Float matrix"));
        const DataVariant DataVariant::DOUBLE_MATRIX = DataVariant(DOUBLE_MATRIX_ID, STROMX_CORE_PACKAGE_NAME, Documentation("Double matrix"));
        const DataVariant DataVariant::STRING = DataVariant(STRING_ID, STROMX_CORE_PACKAGE_NAME, Documentation("String"));
        const DataVariant DataVariant::IMAGE = DataVariant(IMAGE_ID, STROMX_CORE_PACKAGE_NAME, Documentation("Image"));
        const DataVariant DataVariant::MONO_IMAGE = DataVariant(MONO_IMAGE_ID, STROMX_CORE_PACKAGE_NAME, Documentation("Mono image"));
        const DataVariant DataVariant::RGB_IMAGE = DataVariant(RGB_IMAGE_ID, STROMX_CORE_PACKAGE_NAME, Documentation("RGB image"));
        const DataVariant DataVariant::MONO_8_IMAGE = DataVariant(MONO_8_IMAGE_ID, STROMX_CORE_PACKAGE_NAME, Documentation("Mono image 8-bit"));
        const DataVariant DataVariant::RGB_24_IMAGE = DataVariant(RGB_24_IMAGE_ID, STROMX_CORE_PACKAGE_NAME, Documentation("RGB image 24-bit"));
        const DataVariant DataVariant::BGR_24_IMAGE = DataVariant(BGR_24_IMAGE_ID, STROMX_CORE_PACKAGE_NAME, Documentation("BGR image 24-bit"));
        const DataVariant DataVariant::BAYERBG_8_IMAGE = DataVariant(BAYERBG_8_IMAGE_ID, STROMX_CORE_PACKAGE_NAME, Documentation("Bayer BG image 8-bit"));
        const DataVariant DataVariant::BAYERGB_8_IMAGE = DataVariant(BAYERGB_8_IMAGE_ID, STROMX_CORE_PACKAGE_NAME, Documentation("Bayer GB image 8-bit"));
        const DataVariant DataVariant::MONO_16_IMAGE = DataVariant(MONO_16_IMAGE_ID, STROMX_CORE_PACKAGE_NAME, Documentation("Mono image 16-bit"));
        const DataVariant DataVariant::RGB_48_IMAGE = DataVariant(RGB_48_IMAGE_ID, STROMX_CORE_PACKAGE_NAME, Documentation("RGB image 48-bit"));
        const DataVariant DataVariant::BGR_48_IMAGE = DataVariant(BGR_48_IMAGE_ID, STROMX_CORE_PACKAGE_NAME, Documentation("BGR image 48-bit"));
            
        const bool DataVariant::isVariant(const core::DataVariant& variant) const
        {
            if(variant.package() != package())
                return false;
            
            if(variant.id() == DATA_ID)
                return true;
            
            if(variant.id() == id())
                return true;
            
            switch(variant.id())
            {
            case UINT_ID:
                return isVariant(UINT_8) 
                    || isVariant(UINT_16) 
                    || isVariant(UINT_32);
            case INT_ID:
                return isVariant(UINT)
                    || isVariant(INT_8) 
                    || isVariant(INT_16) 
                    || isVariant(INT_32);
            case MATRIX_ID:
                return isVariant(INT_MATRIX)
                    || isVariant(UINT_MATRIX)
                    || isVariant(FLOAT_MATRIX)
                    || isVariant(DOUBLE_MATRIX);
            case INT_MATRIX_ID:
                return isVariant(INT_8_MATRIX)
                    || isVariant(INT_16_MATRIX)
                    || isVariant(INT_32_MATRIX);
            case UINT_MATRIX_ID:
                return isVariant(UINT_8_MATRIX)
                    || isVariant(UINT_16_MATRIX)
                    || isVariant(UINT_32_MATRIX);
            case IMAGE_ID:
                return isVariant(MONO_IMAGE)
                    || isVariant(RGB_IMAGE)
                    || isVariant(BAYERBG_8_IMAGE)
                    || isVariant(BAYERGB_8_IMAGE);
            case MONO_IMAGE_ID:
                return isVariant(MONO_8_IMAGE)
                    || isVariant(MONO_16_IMAGE);
            case RGB_IMAGE_ID:
                return isVariant(RGB_24_IMAGE)
                    || isVariant(BGR_24_IMAGE)
                    || isVariant(RGB_48_IMAGE)
                    || isVariant(BGR_48_IMAGE);
            default:
                return false;
            }
        }
        
        const bool operator==(const stromx::core::DataVariant& lhs, const stromx::core::DataVariant& rhs)
        {
            return lhs.m_id == rhs.m_id && lhs.m_package == rhs.m_package;
        }
        
        std::ostream& operator<<(std::ostream& out, const DataVariant& variant)
        {
            out << variant.package() << ", " << variant.id();
            return out;
        }
    }
}
