#include "ReadAccessImpl.h"

#include "../DataContainer.h"
#include "../Exception.h"

namespace stream
{
    namespace impl
    {
        ReadAccessImpl::ReadAccessImpl(DataContainer& data)
        : m_data(data)
        {
            if(data.empty())
                throw ArgumentException("Data container is empty.");
            
            m_data.m_impl->getReadAccess();
        }

        const Data & ReadAccessImpl::operator()()
        {
            return *m_data.m_impl->data();
        }
        
        ReadAccessImpl::~ReadAccessImpl()
        {
            m_data.m_impl->returnReadAccess();
        }
    }
} 
