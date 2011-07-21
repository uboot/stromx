#include "Primitive.h"

#include <float.h>

namespace stream
{
    template<>
    const DataType & Bool::type() const { return DataType::BOOL; }
    
    template<>
    const DataType & Int8::type() const { return DataType::INT_8; }
    
    template<>
    const DataType & UInt8::type() const { return DataType::UINT_8; }
    
    template<>
    const DataType & Int16::type() const { return DataType::INT_16; }
    
    template<>
    const DataType & UInt16::type() const { return DataType::UINT_16; }
    
    template<>
    const DataType & Int32::type() const { return DataType::INT_32; }
    template<>
    const Int32 Int32::MIN = Int32(INT32_MIN);
    template<>
    const Int32 Int32::MAX = Int32(INT32_MAX);
    
    template<>
    const DataType & UInt32::type() const { return DataType::UINT_32; }
    template<>
    const UInt32 UInt32::MIN = UInt32(0);
    template<>
    const UInt32 UInt32::MAX = UInt32(UINT32_MAX);
    
    template<>
    const DataType & Float::type() const { return DataType::FLOAT; }
    template<>
    const Float Float::MIN = Float(- FLT_MAX);
    template<>
    const Float Float::MAX = Float(FLT_MAX);
    
    template<>
    const DataType & Double::type() const { return DataType::DOUBLE; }
    template<>
    const Double Double::MIN = Double(- DBL_MAX);
    template<>
    const Double Double::MAX = Double(DBL_MAX);
}
