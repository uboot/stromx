#include "DataType.h"

namespace stream
{
    const bool DataType::is(const stream::DataType& type) const
    {
        return type.m_type == m_type;
    }
}
