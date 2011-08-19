#include "DataType.h"

#include "Config.h"

namespace stream
{
    const DataType DataType::NONE = DataType(NONE_ID, "None");
    const DataType DataType::DATA = DataType(DATA_ID, "Data");
    const DataType DataType::TRIGGER = DataType(TRIGGER_ID, "Trigger");
    const DataType DataType::BOOL = DataType(BOOL_ID, "Bool");
    const DataType DataType::ENUM = DataType(ENUM_ID, "Enum");
    const DataType DataType::INT = DataType(INT_ID, "Int");
    const DataType DataType::UINT = DataType(UINT_ID, "UInt");
    const DataType DataType::INT_8 = DataType(INT_8_ID, "Int8");
    const DataType DataType::UINT_8 = DataType(UINT_8_ID, "UInt8");
    const DataType DataType::INT_16 = DataType(INT_16_ID, "Int16");
    const DataType DataType::UINT_16 = DataType(UINT_16_ID, "UInt16");
    const DataType DataType::INT_32 = DataType(INT_32_ID, "Int32");
    const DataType DataType::UINT_32 = DataType(UINT_32_ID, "UInt32");
    const DataType DataType::FLOAT = DataType(FLOAT_ID, "Float");
    const DataType DataType::DOUBLE = DataType(DOUBLE_ID, "Double");
    const DataType DataType::IMAGE = DataType(IMAGE_ID, "Image");
    const DataType DataType::MONO_IMAGE = DataType(MONO_IMAGE_ID, "Mono image");
    const DataType DataType::RGB_IMAGE = DataType(RGB_IMAGE_ID, "RGB image");
    const DataType DataType::MONO_8_IMAGE = DataType(MONO_8_IMAGE_ID, "Mono image 8-bit");
    const DataType DataType::RGB_24_IMAGE = DataType(RGB_24_IMAGE_ID, "RGB image 24-bit");
    const DataType DataType::BGR_24_IMAGE = DataType(BGR_24_IMAGE_ID, "BGR image 24-bit");
    const DataType DataType::BAYERBG_8_IMAGE = DataType(BAYERBG_8_IMAGE_ID, "Bayer BG image 8-bit");
    const DataType DataType::BAYERGB_8_IMAGE = DataType(BAYERGB_8_IMAGE_ID, "Bayer GB image 8-bit");
    const DataType DataType::MONO_16_IMAGE = DataType(MONO_16_IMAGE_ID, "Mono image 16-bit");
    const DataType DataType::RGB_48_IMAGE = DataType(RGB_48_IMAGE_ID, "RGB image 48-bit");
    const DataType DataType::BGR_48_IMAGE = DataType(BGR_48_IMAGE_ID, "BGR image 48-bit");
        
    const bool DataType::is(const stream::DataType& type) const
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
