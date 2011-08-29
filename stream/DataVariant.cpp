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

namespace stream
{
    const DataVariant DataVariant::NONE = DataVariant(NONE_ID, "None");
    const DataVariant DataVariant::DATA = DataVariant(DATA_ID, "Data");
    const DataVariant DataVariant::TRIGGER = DataVariant(TRIGGER_ID, "Trigger");
    const DataVariant DataVariant::BOOL = DataVariant(BOOL_ID, "Bool");
    const DataVariant DataVariant::ENUM = DataVariant(ENUM_ID, "Enum");
    const DataVariant DataVariant::INT = DataVariant(INT_ID, "Int");
    const DataVariant DataVariant::UINT = DataVariant(UINT_ID, "UInt");
    const DataVariant DataVariant::INT_8 = DataVariant(INT_8_ID, "Int8");
    const DataVariant DataVariant::UINT_8 = DataVariant(UINT_8_ID, "UInt8");
    const DataVariant DataVariant::INT_16 = DataVariant(INT_16_ID, "Int16");
    const DataVariant DataVariant::UINT_16 = DataVariant(UINT_16_ID, "UInt16");
    const DataVariant DataVariant::INT_32 = DataVariant(INT_32_ID, "Int32");
    const DataVariant DataVariant::UINT_32 = DataVariant(UINT_32_ID, "UInt32");
    const DataVariant DataVariant::FLOAT = DataVariant(FLOAT_ID, "Float");
    const DataVariant DataVariant::DOUBLE = DataVariant(DOUBLE_ID, "Double");
    const DataVariant DataVariant::IMAGE = DataVariant(IMAGE_ID, "Image");
    const DataVariant DataVariant::MONO_IMAGE = DataVariant(MONO_IMAGE_ID, "Mono image");
    const DataVariant DataVariant::RGB_IMAGE = DataVariant(RGB_IMAGE_ID, "RGB image");
    const DataVariant DataVariant::MONO_8_IMAGE = DataVariant(MONO_8_IMAGE_ID, "Mono image 8-bit");
    const DataVariant DataVariant::RGB_24_IMAGE = DataVariant(RGB_24_IMAGE_ID, "RGB image 24-bit");
    const DataVariant DataVariant::BGR_24_IMAGE = DataVariant(BGR_24_IMAGE_ID, "BGR image 24-bit");
    const DataVariant DataVariant::BAYERBG_8_IMAGE = DataVariant(BAYERBG_8_IMAGE_ID, "Bayer BG image 8-bit");
    const DataVariant DataVariant::BAYERGB_8_IMAGE = DataVariant(BAYERGB_8_IMAGE_ID, "Bayer GB image 8-bit");
    const DataVariant DataVariant::MONO_16_IMAGE = DataVariant(MONO_16_IMAGE_ID, "Mono image 16-bit");
    const DataVariant DataVariant::RGB_48_IMAGE = DataVariant(RGB_48_IMAGE_ID, "RGB image 48-bit");
    const DataVariant DataVariant::BGR_48_IMAGE = DataVariant(BGR_48_IMAGE_ID, "BGR image 48-bit");
        
    const bool DataVariant::is(const stream::DataVariant& type) const
    {
        if(type.m_typeId == DATA_ID)
            return true;
        
        if(type.m_typeId == m_typeId)
            return true;
        
        switch(type.m_typeId)
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
