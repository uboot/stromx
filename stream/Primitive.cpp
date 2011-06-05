#include "Primitive.h"

namespace stream
{
    template<>
    const DataType Bool::m_type(DataType::BOOL);
    
    template<>
    const DataType Int32::m_type(DataType::INT_32);
    
    template<>
    const DataType UInt32::m_type(DataType::UINT_32);
}
