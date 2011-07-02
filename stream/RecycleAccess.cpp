#include "RecycleAccess.h"

namespace stream
{
    RecycleAccess::RecycleAccess(DataContainer data)
      : m_impl(new RecycleAccessImpl(data))
    {
    }

    Data*const RecycleAccess::operator()()
    {
        if(! m_impl.get())
            return 0;
        
        return (*m_impl)();
    }
    
    void RecycleAccess::add(DataContainer data)
    {
        if(! m_impl.get())
            m_impl = boost::shared_ptr<RecycleAccessImpl>(new RecycleAccessImpl(data));
        else
            m_impl->add(data);
    }

} 
