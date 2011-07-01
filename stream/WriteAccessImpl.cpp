#include "WriteAccessImpl.h"

#include "DataContainer.h"

namespace stream
{   
    WriteAccessImpl::WriteAccessImpl(DataContainer& data)
      : m_data(data)
    {
        m_data.m_impl->getWriteAccess();
    }

    Data* const WriteAccessImpl::operator()()
    {
        return m_data.m_impl->data();
    }
    
    WriteAccessImpl::~WriteAccessImpl()
    {
        m_data.m_impl->returnWriteAccess();
    }
} 
