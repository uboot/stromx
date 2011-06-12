#include "DataContainer.h"

#include "Data.h"
#include "Exception.h"
#include "DataOwner.h"

namespace stream
{
    DataContainer::DataContainer (stream::Data* const data, DataOwner* const owner)
      : m_data(data),
        m_owner(owner),
        m_refCount(1),
        m_hasWriteAccess(false)
    {
        if(! data)
            throw ArgumentException();
    }
    
    DataContainer::~DataContainer()
    {
        delete m_data;
    }
 
    void DataContainer::reference()
    {
        unique_lock_t lock(m_mutex);
        
        try
        {
            while(m_hasWriteAccess)
                m_cond.wait(lock);
        }
        catch(boost::thread_interrupted&)
        {
            throw InterruptException();
        }  
        
        m_refCount++;
    }

    void DataContainer::dereference()
    {
        bool destroy = false;
        
        {
            lock_t lock(m_mutex);
            
            BOOST_ASSERT(m_refCount != 0);
            
            m_refCount--;
            
            if(m_refCount == 0)
                destroy = true;
            else
                m_cond.notify_all();
            
            if(m_hasWriteAccess)
            {
                BOOST_ASSERT(m_refCount == 0);
                m_hasWriteAccess = false;
            }        
        }
        
        // the following code possibly leads to the deletion of 
        // the object
        // it can not be executed while the mutex is locked
        if(destroy)
        {
            if(m_owner)
                m_owner->release(this);
            else
                delete this;
        }
    }
    
    const Data*const DataContainer::getReadAccess()
    {
        lock_t lock(m_mutex);
        
        BOOST_ASSERT(m_refCount != 0);
        
        return m_data;    
    }
    
    Data*const DataContainer::getWriteAccess()
    {
        unique_lock_t lock(m_mutex);
        
        BOOST_ASSERT(m_refCount != 0);
        
        try
        {
            while(m_refCount != 1)
                m_cond.wait(lock);
        }
        catch(boost::thread_interrupted&)
        {
            throw InterruptException();
        } 
           
        m_hasWriteAccess = true;
        
        return m_data;
    }
    
    void DataContainer::clearWriteAccess()
    {
        lock_t lock(m_mutex);
        
        m_cond.notify_all();
        
        m_hasWriteAccess = false;             
    }
}
