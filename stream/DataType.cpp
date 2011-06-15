#include "DataType.h"

namespace stream
{
    const bool DataType::is(const stream::DataType& type) const
    {
        if(type.m_type == m_type)
            return true;
        
        switch(type.m_type)
        {
        case UINT:
            return is(UINT_8) 
                || is(UINT_16) 
                || is(UINT_32);
        case INT:
            return is(UINT)
                || is(INT_8) 
                || is(INT_16) 
                || is(INT_32);
        case IMAGE:
            return is(MONO_IMAGE)
                || is(RGB_IMAGE);
        case MONO_IMAGE:
            return is(MONO_8_IMAGE)
                || is(MONO_16_IMAGE);
        case RGB_IMAGE:
            return is(RGB_24_IMAGE)
                || is(RGB_48_IMAGE);
        default:
            return false;
        }
    }
}
