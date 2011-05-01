#include "DataContainer.h"

#include "Data.h"
#include "Exception.h"

namespace stream
{
    DataContainer::DataContainer ( const unsigned int id, const stream::Operator*const owner, stream::Data* const data )
      : m_id(id),
        m_owner(owner),
        m_data(data),
        m_refCount(1)
    {
        if(! data)
            throw ArgumentException();
    }
    
    DataContainer::DataContainer ( const unsigned int id, stream::Data* const data )
      : m_id(id),
        m_owner(0),
        m_data(data),
        m_refCount(1)
    {
        if(! data)
            throw ArgumentException("Data pointer is 0");
    }
    
    void DataContainer::reference()
    {
        boost::lock_guard<boost::mutex> lock(m_mutex);
        
        m_refCount++;
    }

    void DataContainer::dereference()
    {
        boost::lock_guard<boost::mutex> lock(m_mutex);
        
        if(m_refCount == 0)
            throw ReferenceCountException("Reference count is 0");
        
        m_refCount--;
        
        if(m_refCount == 0)
        {
            delete m_data;
            m_data = 0;
        }
    }
    
    const Data*const DataContainer::getReadAccess()
    {
        boost::lock_guard<boost::mutex> lock(m_mutex);
        
        if(m_refCount == 0)
            throw ReferenceCountException("Data container is not referenced");
            
        return m_data;    
    }
    
    Data*const DataContainer::obtainOwnership(const Operator* const owner)
    {
        boost::lock_guard<boost::mutex> lock(m_mutex);
        
        if(m_refCount != 1)
            throw ReferenceCountException("Data container is referenced by more than one clients");
        
        if(!m_owner && m_owner != owner)
            throw ReferenceCountException("Data container is owned by a different client");
        
        m_owner = owner;
    }
    
    void DataContainer::releaseOwnership(const Operator* const owner)
    {
        boost::lock_guard<boost::mutex> lock(m_mutex);
        
        if(m_owner != owner)
            throw ArgumentException("Data container is owned by a different client");
        
        m_owner = 0;        
    }
}
