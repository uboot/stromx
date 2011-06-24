#include "RecycleAccessImpl.h"

#include "Data.h"
#include "DataContainer.h"
#include "Exception.h"

namespace stream
{
    RecycleAccessImpl::RecycleAccessImpl(DataContainer& data)
      : m_isExpired(false),
        m_data(0),
        m_dataContainer(data.m_impl.get())
    {
        m_dataContainer->getRecycleAccess(this);
    }

    void RecycleAccessImpl::recycle(Data* const data)
    {
        lock_t lock(m_mutex);
        
        m_dataContainer = 0;
        m_data = data;
        m_cond.notify_all();
    }

    RecycleAccessImpl::~RecycleAccessImpl()
    {
        if(m_dataContainer)
        {
            BOOST_ASSERT(! m_data);
            m_dataContainer->returnRecycleAccess();
        }
        else
        {
            delete m_data;
        }
    }
    
    Data*const RecycleAccessImpl::operator()()
    {
        unique_lock_t lock(m_mutex);
        
        if(m_isExpired)
            return 0;
        
        try
        {
            while(! m_data)
                m_cond.wait(lock);
        }
        catch(boost::thread_interrupted&)
        {
            throw InterruptException();
        }
        
        Data* value = m_data;
        m_data = 0;
        m_isExpired = true;
        return value;
    }
} 
