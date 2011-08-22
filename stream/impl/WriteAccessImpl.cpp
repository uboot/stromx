#include "WriteAccessImpl.h"

#include "../DataContainer.h"
#include "../Exception.h"

namespace stream
{   
    namespace impl
    {
        WriteAccessImpl::WriteAccessImpl(DataContainer& data)
        : m_data(data)
        {
            if(data.empty())
                throw WrongArgument("Data container is empty.");
            
            m_data.m_impl->getWriteAccess();
        }

        Data& WriteAccessImpl::operator()() const
        {
            return *m_data.m_impl->data();
        }
        
        WriteAccessImpl::~WriteAccessImpl()
        {
            m_data.m_impl->returnWriteAccess();
        }
    }
} 
