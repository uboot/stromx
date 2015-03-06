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

#include <ostream>

#include "stromx/runtime/Config.h"
#include "stromx/runtime/Data.h"
#include "stromx/runtime/DataVariant.h"
#include "stromx/runtime/Locale.h"

namespace stromx
{
    namespace runtime
    {
        const DataVariant DataVariant::NONE = DataVariant(NONE_ID, STROMX_RUNTIME_PACKAGE_NAME);
        const DataVariant DataVariant::DATA = DataVariant(DATA_ID, STROMX_RUNTIME_PACKAGE_NAME);
        const DataVariant DataVariant::TRIGGER = DataVariant(TRIGGER_ID, STROMX_RUNTIME_PACKAGE_NAME);
        const DataVariant DataVariant::PRIMITIVE = DataVariant(PRIMITIVE_ID, STROMX_RUNTIME_PACKAGE_NAME);
        const DataVariant DataVariant::BOOL = DataVariant(BOOL_ID, STROMX_RUNTIME_PACKAGE_NAME);
        const DataVariant DataVariant::ENUM = DataVariant(ENUM_ID, STROMX_RUNTIME_PACKAGE_NAME);
        const DataVariant DataVariant::INT = DataVariant(INT_ID, STROMX_RUNTIME_PACKAGE_NAME);
        const DataVariant DataVariant::UINT = DataVariant(UINT_ID, STROMX_RUNTIME_PACKAGE_NAME);
        const DataVariant DataVariant::INT_8 = DataVariant(INT_8_ID, STROMX_RUNTIME_PACKAGE_NAME);
        const DataVariant DataVariant::UINT_8 = DataVariant(UINT_8_ID, STROMX_RUNTIME_PACKAGE_NAME);
        const DataVariant DataVariant::INT_16 = DataVariant(INT_16_ID, STROMX_RUNTIME_PACKAGE_NAME);
        const DataVariant DataVariant::UINT_16 = DataVariant(UINT_16_ID, STROMX_RUNTIME_PACKAGE_NAME);
        const DataVariant DataVariant::INT_32 = DataVariant(INT_32_ID, STROMX_RUNTIME_PACKAGE_NAME);
        const DataVariant DataVariant::UINT_32 = DataVariant(UINT_32_ID, STROMX_RUNTIME_PACKAGE_NAME);
        const DataVariant DataVariant::FLOAT = DataVariant(FLOAT_ID, STROMX_RUNTIME_PACKAGE_NAME);
        const DataVariant DataVariant::FLOAT_32 = DataVariant(FLOAT_32_ID, STROMX_RUNTIME_PACKAGE_NAME);
        const DataVariant DataVariant::FLOAT_64 = DataVariant(FLOAT_64_ID, STROMX_RUNTIME_PACKAGE_NAME);
        const DataVariant DataVariant::LIST = DataVariant(LIST_ID, STROMX_RUNTIME_PACKAGE_NAME);
        const DataVariant DataVariant::MATRIX = DataVariant(MATRIX_ID, STROMX_RUNTIME_PACKAGE_NAME);
        const DataVariant DataVariant::INT_MATRIX = DataVariant(INT_MATRIX_ID, STROMX_RUNTIME_PACKAGE_NAME);
        const DataVariant DataVariant::UINT_MATRIX = DataVariant(UINT_MATRIX_ID, STROMX_RUNTIME_PACKAGE_NAME);
        const DataVariant DataVariant::INT_8_MATRIX = DataVariant(INT_8_MATRIX_ID, STROMX_RUNTIME_PACKAGE_NAME);
        const DataVariant DataVariant::UINT_8_MATRIX = DataVariant(UINT_8_MATRIX_ID, STROMX_RUNTIME_PACKAGE_NAME);
        const DataVariant DataVariant::INT_16_MATRIX = DataVariant(INT_16_MATRIX_ID, STROMX_RUNTIME_PACKAGE_NAME);
        const DataVariant DataVariant::UINT_16_MATRIX = DataVariant(UINT_16_MATRIX_ID, STROMX_RUNTIME_PACKAGE_NAME);
        const DataVariant DataVariant::INT_32_MATRIX = DataVariant(INT_32_MATRIX_ID, STROMX_RUNTIME_PACKAGE_NAME);
        const DataVariant DataVariant::UINT_32_MATRIX = DataVariant(UINT_32_MATRIX_ID, STROMX_RUNTIME_PACKAGE_NAME);
        const DataVariant DataVariant::FLOAT_MATRIX = DataVariant(FLOAT_MATRIX_ID, STROMX_RUNTIME_PACKAGE_NAME);
        const DataVariant DataVariant::FLOAT_32_MATRIX = DataVariant(FLOAT_32_MATRIX_ID, STROMX_RUNTIME_PACKAGE_NAME);
        const DataVariant DataVariant::FLOAT_64_MATRIX = DataVariant(FLOAT_64_MATRIX_ID, STROMX_RUNTIME_PACKAGE_NAME);
        const DataVariant DataVariant::STRING = DataVariant(STRING_ID, STROMX_RUNTIME_PACKAGE_NAME);
        const DataVariant DataVariant::IMAGE = DataVariant(IMAGE_ID, STROMX_RUNTIME_PACKAGE_NAME);
        const DataVariant DataVariant::MONO_IMAGE = DataVariant(MONO_IMAGE_ID, STROMX_RUNTIME_PACKAGE_NAME);
        const DataVariant DataVariant::RGB_IMAGE = DataVariant(RGB_IMAGE_ID, STROMX_RUNTIME_PACKAGE_NAME);
        const DataVariant DataVariant::MONO_8_IMAGE = DataVariant(MONO_8_IMAGE_ID, STROMX_RUNTIME_PACKAGE_NAME);
        const DataVariant DataVariant::RGB_24_IMAGE = DataVariant(RGB_24_IMAGE_ID, STROMX_RUNTIME_PACKAGE_NAME);
        const DataVariant DataVariant::BGR_24_IMAGE = DataVariant(BGR_24_IMAGE_ID, STROMX_RUNTIME_PACKAGE_NAME);
        const DataVariant DataVariant::BAYERBG_8_IMAGE = DataVariant(BAYERBG_8_IMAGE_ID, STROMX_RUNTIME_PACKAGE_NAME);
        const DataVariant DataVariant::BAYERGB_8_IMAGE = DataVariant(BAYERGB_8_IMAGE_ID, STROMX_RUNTIME_PACKAGE_NAME);
        const DataVariant DataVariant::BAYERBG_16_IMAGE = DataVariant(BAYERBG_16_IMAGE_ID, STROMX_RUNTIME_PACKAGE_NAME);
        const DataVariant DataVariant::BAYERGB_16_IMAGE = DataVariant(BAYERGB_16_IMAGE_ID, STROMX_RUNTIME_PACKAGE_NAME);
        const DataVariant DataVariant::MONO_16_IMAGE = DataVariant(MONO_16_IMAGE_ID, STROMX_RUNTIME_PACKAGE_NAME);
        const DataVariant DataVariant::RGB_48_IMAGE = DataVariant(RGB_48_IMAGE_ID, STROMX_RUNTIME_PACKAGE_NAME);
        const DataVariant DataVariant::BGR_48_IMAGE = DataVariant(BGR_48_IMAGE_ID, STROMX_RUNTIME_PACKAGE_NAME);
         
        const std::string DataVariant::title() const
        {
            if (package() != STROMX_RUNTIME_PACKAGE_NAME)
                return "";
            
            switch(id())
            {
            case NONE_ID:
                return  L_("None");
            case DATA_ID:
                return  L_("Data");
            case TRIGGER_ID:
                return  L_("Trigger");
            case PRIMITIVE_ID:
                return  L_("Primitive");
            case BOOL_ID:
                return  L_("Bool");
            case ENUM_ID:
                return  L_("Enum");
            case INT_ID:
                return  L_("Int");
            case UINT_ID:
                return  L_("UInt");
            case INT_8_ID:
                return  L_("Int8");
            case UINT_8_ID:
                return  L_("UInt8");
            case INT_16_ID:
                return  L_("Int16");
            case UINT_16_ID:
                return  L_("UInt16");
            case INT_32_ID:
                return  L_("Int32");
            case UINT_32_ID:
                return  L_("UInt32");
            case FLOAT_ID:
                return  L_("Float");
            case FLOAT_32_ID:
                return  L_("Float32");
            case FLOAT_64_ID:
                return  L_("Float64");
            case LIST_ID:
                return  L_("List");
            case MATRIX_ID:
                return  L_("Matrix");
            case INT_MATRIX_ID:
                return  L_("Int matrix");
            case UINT_MATRIX_ID:
                return  L_("UInt matrix");
            case INT_8_MATRIX_ID:
                return  L_("Int8 matrix");
            case UINT_8_MATRIX_ID:
                return  L_("UInt8 matrix");
            case INT_16_MATRIX_ID:
                return  L_("Int16 matrix");
            case UINT_16_MATRIX_ID:
                return  L_("UInt16 matrix");
            case INT_32_MATRIX_ID:
                return  L_("Int32 matrix");
            case UINT_32_MATRIX_ID:
                return  L_("UInt32 matrix");
            case FLOAT_MATRIX_ID:
                return  L_("Float matrix");
            case FLOAT_32_MATRIX_ID:
                return  L_("Float32 matrix");
            case FLOAT_64_MATRIX_ID:
                return  L_("Float64 matrix");
            case STRING_ID:
                return  L_("String");
            case IMAGE_ID:
                return  L_("Image");
            case MONO_IMAGE_ID:
                return  L_("Mono image");
            case RGB_IMAGE_ID:
                return  L_("RGB image");
            case MONO_8_IMAGE_ID:
                return  L_("Mono image 8-bit");
            case RGB_24_IMAGE_ID:
                return  L_("RGB image 24-bit");
            case BGR_24_IMAGE_ID:
                return  L_("BGR image 24-bit");
            case BAYERBG_8_IMAGE_ID:
                return  L_("Bayer BG image 8-bit");
            case BAYERGB_8_IMAGE_ID:
                return  L_("Bayer GB image 8-bit");
            case BAYERBG_16_IMAGE_ID:
                return  L_("Bayer BG image 16-bit");
            case BAYERGB_16_IMAGE_ID:
                return  L_("Bayer GB image 16-bit");
            case MONO_16_IMAGE_ID:
                return  L_("Mono image 16-bit");
            case RGB_48_IMAGE_ID:
                return  L_("RGB image 48-bit");
            case BGR_48_IMAGE_ID:
                return  L_("BGR image 48-bit");
            default:
                return "";
            }
        }

        bool DataVariant::isVariant(const runtime::DataVariant& variant) const
        {
            // per default each data type is derived from Data
            if(variant == DataVariant::DATA)
                return true;
            
            if(variant.package() != package())
                return false;
            
            if(variant.id() == DATA_ID)
                return true;
            
            if(variant.id() == id())
                return true;
            
            switch(variant.id())
            {
            case PRIMITIVE_ID:
                return isVariant(BOOL)
                    || isVariant(INT) 
                    || isVariant(UINT) 
                    || isVariant(FLOAT_32) 
                    || isVariant(FLOAT_64);
            case UINT_ID:
                return isVariant(UINT_8) 
                    || isVariant(UINT_16) 
                    || isVariant(UINT_32);
            case UINT_32_ID:
                return isVariant(ENUM);
            case INT_ID:
                return isVariant(UINT)
                    || isVariant(INT_8) 
                    || isVariant(INT_16) 
                    || isVariant(INT_32);
            case FLOAT_ID:
                return isVariant(FLOAT_32)
                    || isVariant(FLOAT_64);
            case MATRIX_ID:
                return isVariant(INT_MATRIX)
                    || isVariant(UINT_MATRIX)
                    || isVariant(FLOAT_MATRIX)
                    || isVariant(IMAGE);
            case INT_MATRIX_ID:
                return isVariant(UINT_MATRIX)
                    || isVariant(INT_8_MATRIX)
                    || isVariant(INT_16_MATRIX)
                    || isVariant(INT_32_MATRIX);
            case UINT_MATRIX_ID:
                return isVariant(UINT_8_MATRIX)
                    || isVariant(UINT_16_MATRIX)
                    || isVariant(UINT_32_MATRIX);
            case FLOAT_MATRIX_ID:
                return isVariant(FLOAT_32_MATRIX)
                    || isVariant(FLOAT_64_MATRIX);
            case IMAGE_ID:
                return isVariant(MONO_IMAGE)
                    || isVariant(RGB_IMAGE)
                    || isVariant(BAYERBG_8_IMAGE)
                    || isVariant(BAYERGB_8_IMAGE)
                    || isVariant(BAYERBG_16_IMAGE)
                    || isVariant(BAYERGB_16_IMAGE);;
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
        
        bool operator==(const stromx::runtime::DataVariant& lhs, const stromx::runtime::DataVariant& rhs)
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
