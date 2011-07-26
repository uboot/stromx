#include "ReadAccess.h"

#include "DataContainer.h"

namespace stream
{
    ReadAccess::ReadAccess(DataContainer data)
      : m_impl(new ReadAccessImpl(data))
    {
    }

    const Data & ReadAccess::operator()()
    {
        return (*m_impl)();
    }
} 
