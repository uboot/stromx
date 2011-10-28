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

#include "DataVariant.h"

#include "Config.h"

namespace core
{
    const DataVariant DataVariant::NONE = DataVariant(NONE_ID, PACKAGE_NAME, "None");
    const DataVariant DataVariant::DATA = DataVariant(DATA_ID, PACKAGE_NAME, "Data");
    const DataVariant DataVariant::TRIGGER = DataVariant(TRIGGER_ID, PACKAGE_NAME, "Trigger");
    const DataVariant DataVariant::BOOL = DataVariant(BOOL_ID, PACKAGE_NAME, "Bool");
    const DataVariant DataVariant::ENUM = DataVariant(ENUM_ID, PACKAGE_NAME, "Enum");
    const DataVariant DataVariant::INT = DataVariant(INT_ID, PACKAGE_NAME, "Int");
    const DataVariant DataVariant::UINT = DataVariant(UINT_ID, PACKAGE_NAME, "UInt");
    const DataVariant DataVariant::INT_8 = DataVariant(INT_8_ID, PACKAGE_NAME, "Int8");
    const DataVariant DataVariant::UINT_8 = DataVariant(UINT_8_ID, PACKAGE_NAME, "UInt8");
    const DataVariant DataVariant::INT_16 = DataVariant(INT_16_ID, PACKAGE_NAME, "Int16");
    const DataVariant DataVariant::UINT_16 = DataVariant(UINT_16_ID, PACKAGE_NAME, "UInt16");
    const DataVariant DataVariant::INT_32 = DataVariant(INT_32_ID, PACKAGE_NAME, "Int32");
    const DataVariant DataVariant::UINT_32 = DataVariant(UINT_32_ID, PACKAGE_NAME, "UInt32");
    const DataVariant DataVariant::FLOAT = DataVariant(FLOAT_ID, PACKAGE_NAME, "Float");
    const DataVariant DataVariant::DOUBLE = DataVariant(DOUBLE_ID, PACKAGE_NAME, "Double");
    const DataVariant DataVariant::IMAGE = DataVariant(IMAGE_ID, PACKAGE_NAME, "Image");
    const DataVariant DataVariant::MONO_IMAGE = DataVariant(MONO_IMAGE_ID, PACKAGE_NAME, "Mono image");
    const DataVariant DataVariant::RGB_IMAGE = DataVariant(RGB_IMAGE_ID, PACKAGE_NAME, "RGB image");
    const DataVariant DataVariant::MONO_8_IMAGE = DataVariant(MONO_8_IMAGE_ID, PACKAGE_NAME, "Mono image 8-bit");
    const DataVariant DataVariant::RGB_24_IMAGE = DataVariant(RGB_24_IMAGE_ID, PACKAGE_NAME, "RGB image 24-bit");
    const DataVariant DataVariant::BGR_24_IMAGE = DataVariant(BGR_24_IMAGE_ID, PACKAGE_NAME, "BGR image 24-bit");
    const DataVariant DataVariant::BAYERBG_8_IMAGE = DataVariant(BAYERBG_8_IMAGE_ID, PACKAGE_NAME, "Bayer BG image 8-bit");
    const DataVariant DataVariant::BAYERGB_8_IMAGE = DataVariant(BAYERGB_8_IMAGE_ID, PACKAGE_NAME, "Bayer GB image 8-bit");
    const DataVariant DataVariant::MONO_16_IMAGE = DataVariant(MONO_16_IMAGE_ID, PACKAGE_NAME, "Mono image 16-bit");
    const DataVariant DataVariant::RGB_48_IMAGE = DataVariant(RGB_48_IMAGE_ID, PACKAGE_NAME, "RGB image 48-bit");
    const DataVariant DataVariant::BGR_48_IMAGE = DataVariant(BGR_48_IMAGE_ID, PACKAGE_NAME, "BGR image 48-bit");
        
    const bool DataVariant::is(const core::DataVariant& type) const
    {
        if(type.package() != package())
            return false;
        
        if(type.id() == DATA_ID)
            return true;
        
        if(type.id() == id())
            return true;
        
        switch(type.id())
        {
        case UINT_ID:
            return is(UINT_8) 
                || is(UINT_16) 
                || is(UINT_32);
        case INT_ID:
            return is(UINT)
                || is(INT_8) 
                || is(INT_16) 
                || is(INT_32);
        case IMAGE_ID:
            return is(MONO_IMAGE)
                || is(RGB_IMAGE);
        case MONO_IMAGE_ID:
            return is(MONO_8_IMAGE)
                || is(MONO_16_IMAGE);
        case RGB_IMAGE_ID:
            return is(RGB_24_IMAGE)
                || is(BGR_24_IMAGE)
                || is(RGB_48_IMAGE)
                || is(BGR_48_IMAGE);
        default:
            return false;
        }
    }
}
