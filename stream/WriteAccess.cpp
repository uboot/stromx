#include "WriteAccess.h"

#include "DataContainer.h"

namespace stream
{
    WriteAccess::WriteAccess(DataContainer& data)
      : m_impl(new WriteAccessImpl(data))
    {
    }

    Data* const WriteAccess::operator()()
    {
        return (*m_impl)();
    }
} 
