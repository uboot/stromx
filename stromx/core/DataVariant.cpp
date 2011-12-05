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

namespace stromx
{
    namespace core
    {
        const DataVariant DataVariant::NONE = DataVariant(NONE_ID, STROMX_CORE_PACKAGE_NAME, "None");
        const DataVariant DataVariant::DATA = DataVariant(DATA_ID, STROMX_CORE_PACKAGE_NAME, "Data");
        const DataVariant DataVariant::TRIGGER = DataVariant(TRIGGER_ID, STROMX_CORE_PACKAGE_NAME, "Trigger");
        const DataVariant DataVariant::BOOL = DataVariant(BOOL_ID, STROMX_CORE_PACKAGE_NAME, "Bool");
        const DataVariant DataVariant::ENUM = DataVariant(ENUM_ID, STROMX_CORE_PACKAGE_NAME, "Enum");
        const DataVariant DataVariant::INT = DataVariant(INT_ID, STROMX_CORE_PACKAGE_NAME, "Int");
        const DataVariant DataVariant::UINT = DataVariant(UINT_ID, STROMX_CORE_PACKAGE_NAME, "UInt");
        const DataVariant DataVariant::INT_8 = DataVariant(INT_8_ID, STROMX_CORE_PACKAGE_NAME, "Int8");
        const DataVariant DataVariant::UINT_8 = DataVariant(UINT_8_ID, STROMX_CORE_PACKAGE_NAME, "UInt8");
        const DataVariant DataVariant::INT_16 = DataVariant(INT_16_ID, STROMX_CORE_PACKAGE_NAME, "Int16");
        const DataVariant DataVariant::UINT_16 = DataVariant(UINT_16_ID, STROMX_CORE_PACKAGE_NAME, "UInt16");
        const DataVariant DataVariant::INT_32 = DataVariant(INT_32_ID, STROMX_CORE_PACKAGE_NAME, "Int32");
        const DataVariant DataVariant::UINT_32 = DataVariant(UINT_32_ID, STROMX_CORE_PACKAGE_NAME, "UInt32");
        const DataVariant DataVariant::FLOAT = DataVariant(FLOAT_ID, STROMX_CORE_PACKAGE_NAME, "Float");
        const DataVariant DataVariant::DOUBLE = DataVariant(DOUBLE_ID, STROMX_CORE_PACKAGE_NAME, "Double");
        const DataVariant DataVariant::IMAGE = DataVariant(IMAGE_ID, STROMX_CORE_PACKAGE_NAME, "Image");
        const DataVariant DataVariant::MONO_IMAGE = DataVariant(MONO_IMAGE_ID, STROMX_CORE_PACKAGE_NAME, "Mono image");
        const DataVariant DataVariant::RGB_IMAGE = DataVariant(RGB_IMAGE_ID, STROMX_CORE_PACKAGE_NAME, "RGB image");
        const DataVariant DataVariant::MONO_8_IMAGE = DataVariant(MONO_8_IMAGE_ID, STROMX_CORE_PACKAGE_NAME, "Mono image 8-bit");
        const DataVariant DataVariant::RGB_24_IMAGE = DataVariant(RGB_24_IMAGE_ID, STROMX_CORE_PACKAGE_NAME, "RGB image 24-bit");
        const DataVariant DataVariant::BGR_24_IMAGE = DataVariant(BGR_24_IMAGE_ID, STROMX_CORE_PACKAGE_NAME, "BGR image 24-bit");
        const DataVariant DataVariant::BAYERBG_8_IMAGE = DataVariant(BAYERBG_8_IMAGE_ID, STROMX_CORE_PACKAGE_NAME, "Bayer BG image 8-bit");
        const DataVariant DataVariant::BAYERGB_8_IMAGE = DataVariant(BAYERGB_8_IMAGE_ID, STROMX_CORE_PACKAGE_NAME, "Bayer GB image 8-bit");
        const DataVariant DataVariant::MONO_16_IMAGE = DataVariant(MONO_16_IMAGE_ID, STROMX_CORE_PACKAGE_NAME, "Mono image 16-bit");
        const DataVariant DataVariant::RGB_48_IMAGE = DataVariant(RGB_48_IMAGE_ID, STROMX_CORE_PACKAGE_NAME, "RGB image 48-bit");
        const DataVariant DataVariant::BGR_48_IMAGE = DataVariant(BGR_48_IMAGE_ID, STROMX_CORE_PACKAGE_NAME, "BGR image 48-bit");
            
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
            case IMAGE_ID:
                return isVariant(MONO_IMAGE)
                    || isVariant(RGB_IMAGE);
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
    }
}
