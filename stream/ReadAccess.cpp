#include "ReadAccess.h"

#include "DataContainer.h"

namespace stream
{
    ReadAccess::ReadAccess(DataContainer& data)
    : m_data(data)
    {
        m_data.m_impl->getReadAccess();
    }

    const Data* const ReadAccess::operator()()
    {
        return m_data.m_impl->data();
    }
    
    ReadAccess::~ReadAccess()
    {
        m_data.m_impl->getReadAccess();
    }
} 
