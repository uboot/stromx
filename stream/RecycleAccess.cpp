#include "RecycleAccess.h"

namespace stream
{
    RecycleAccess::RecycleAccess(DataContainer& data)
      : m_impl(new RecycleAccessImpl(data))
    {
    }

    Data*const RecycleAccess::operator()()
    {
        if(! m_impl.get())
            return 0;
        
        return (*m_impl)();
    }

} 
