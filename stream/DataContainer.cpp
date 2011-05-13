#include "DataContainer.h"

#include "Data.h"
#include "Exception.h"
#include "DataManager.h"

namespace stream
{
    DataContainer::DataContainer (stream::Data* const data, DataManager* const manager)
      : m_data(data),
        m_manager(manager),
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
        boost::lock_guard<boost::mutex> lock(m_mutex);
        
        BOOST_ASSERT(! m_hasWriteAccess);
        
        m_refCount++;
    }

    void DataContainer::dereference()
    {
        bool destroy = false;
        
        {
            boost::lock_guard<boost::mutex> lock(m_mutex);
            
            BOOST_ASSERT(m_refCount != 0);
            
            m_refCount--;
            
            if(m_refCount == 0)
                destroy = true;
        }
        
        // the following code possibly leads to the deletion of 
        // the object
        // it can not be executed while the mutex is locked
        if(destroy)
        {
            if(m_manager)
                m_manager->release(this);
            else
                delete this;
        }
    }
    
    const Data*const DataContainer::getReadAccess()
    {
        BOOST_ASSERT(m_refCount != 0);
        
        return m_data;    
    }
    
    Data*const DataContainer::getWriteAccess()
    {
        boost::lock_guard<boost::mutex> lock(m_mutex);
        
        BOOST_ASSERT(m_refCount != 0);
        
        if(m_refCount != 1)
            throw ReferenceCountException("Data container is referenced by more than one clients");
        
        return m_data;
    }
    
    void DataContainer::clearWriteAccess()
    {
        boost::lock_guard<boost::mutex> lock(m_mutex);
        
        BOOST_ASSERT(m_refCount == 1);
        
        m_hasWriteAccess = false;             
    }
}
