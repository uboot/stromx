#include "RecycleAccess.h"

namespace stream
{
    RecycleAccess::RecycleAccess(DataContainer& data)
      : m_impl(new RecycleAccessImpl(data))
    {
    }

    Data*const RecycleAccess::operator()()
    {
        return (*m_impl)();
    }

} 
