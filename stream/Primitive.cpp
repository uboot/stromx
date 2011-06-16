#include "Primitive.h"

namespace stream
{
    template<>
    const DataType & Bool::type() const { return DataType::BOOL; }
    
    template<>
    const DataType & Int16::type() const { return DataType::INT_16; }
    
    template<>
    const DataType & UInt16::type() const { return DataType::UINT_16; }
    
    template<>
    const DataType & Int32::type() const { return DataType::INT_32; }
    
    template<>
    const DataType & UInt32::type() const { return DataType::UINT_32; }
    template<>
    const UInt32 UInt32::MIN = UInt32(0);
    template<>
    const UInt32 UInt32::MAX = UInt32(UINT32_MAX);
    
    template<>
    const DataType & Double::type() const { return DataType::DOUBLE; }
}
