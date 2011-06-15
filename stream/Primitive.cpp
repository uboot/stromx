#include "Primitive.h"

namespace stream
{
    template<>
    const DataType Bool::TYPE(DataType::BOOL);
    
    template<>
    const DataType Int16::TYPE(DataType::INT_16);
    
    template<>
    const DataType UInt16::TYPE(DataType::UINT_16);
    
    template<>
    const DataType Int32::TYPE(DataType::INT_32);
    
    template<>
    const DataType UInt32::TYPE(DataType::UINT_32);
    template<>
    const UInt32 UInt32::MIN = UInt32(0);
    template<>
    const UInt32 UInt32::MAX = UInt32(UINT32_MAX);
    
    template<>
    const DataType Double::TYPE(DataType::DOUBLE);
}
