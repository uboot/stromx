#include "Primitive.h"

namespace stream
{
    template<>
    const DataType Bool::m_type(DataType::BOOL);
    
    template<>
    const DataType Int32::m_type(DataType::INT_32);
    
    template<>
    const DataType UInt32::m_type(DataType::UINT_32);
    
    template<>
    const UInt32 UInt32::MIN = UInt32(0);
    
    template<>
    const UInt32 UInt32::MAX = UInt32(UINT32_MAX);
}
