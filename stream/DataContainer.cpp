#include "DataContainer.h"

#include "Recycler.h"

namespace stream
{
    DataContainer::DataContainer(Data* const data)
    : m_impl(new DataContainerImpl(data))
    {
    }
    
    const bool operator==(const stream::DataContainer& lhs, const stream::DataContainer& rhs)
    {
        if(! lhs.m_impl.get() && ! rhs.m_impl.get())
            return true;
        
        if(! lhs.m_impl.get() || ! rhs.m_impl.get())
            return false;
        
        return lhs.m_impl->data() == rhs.m_impl->data();
    }
    
    const bool operator!=(const stream::DataContainer& lhs, const stream::DataContainer& rhs)
    {
        return ! (lhs == rhs);
    }
    
    std::ostream& operator<<(std::ostream& out, const stream::DataContainer& container)
    {
        if(! container.m_impl.get())
            out << 0;
        else
            out << container.m_impl->data();
    }


}
 