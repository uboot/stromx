#include "ReadAccessImpl.h"

#include "DataContainer.h"

namespace stream
{
    ReadAccessImpl::ReadAccessImpl(DataContainer& data)
      : m_data(data)
    {
        m_data.m_impl->getReadAccess();
    }

    const Data* const ReadAccessImpl::operator()()
    {
        return m_data.m_impl->data();
    }
    
    ReadAccessImpl::~ReadAccessImpl()
    {
        m_data.m_impl->returnReadAccess();
    }
} 
