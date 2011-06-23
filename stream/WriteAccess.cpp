#include "WriteAccess.h"

#include "DataContainer.h"

namespace stream
{
    WriteAccess::WriteAccess(DataContainer& data)
    : m_data(data)
    {
        m_data.m_impl->getWriteAccess();
    }

    Data* const WriteAccess::operator()()
    {
        return m_data.m_impl->data();
    }
    
    WriteAccess::~WriteAccess()
    {
        m_data.m_impl->returnWriteAccess();
    }
} 
