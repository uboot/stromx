#include "DataContainer.h"

#include "Data.h"
#include "Exception.h"

namespace stream
{
    DataContainer::DataContainer (const stream::Operator*const owner, stream::Data* const data )
      : m_owner(owner),
        m_data(data),
        m_refCount(1)
    {
        if(! data)
            throw ArgumentException();
    }
    
    DataContainer::DataContainer ( stream::Data* const data )
      : m_owner(0),
        m_data(data),
        m_refCount(1)
    {
        if(! data)
            throw ArgumentException("Data pointer is 0");
    }
    
    DataContainer::~DataContainer()
    {
        delete m_data;
    }

    
    void DataContainer::reference()
    {
        boost::lock_guard<boost::mutex> lock(m_mutex);
        
        if(m_owner != 0)
            throw OwnershipException("This data can not be referenced because it is owned");
        
        m_refCount++;
    }

    void DataContainer::dereference()
    {
        boost::lock_guard<boost::mutex> lock(m_mutex);
        
        BOOST_ASSERT(m_refCount != 0);
        
        if(m_owner != 0)
            throw OwnershipException("This data can not be dereferenced because it is owned");
        
        m_refCount--;
        
        if(m_refCount == 0)
            delete this;
    }
    
    const Data*const DataContainer::getReadAccess()
    {
        boost::lock_guard<boost::mutex> lock(m_mutex);
        
        BOOST_ASSERT(m_refCount != 0);
            
        return m_data;    
    }
    
    Data*const DataContainer::obtainOwnership(const Operator* const owner)
    {
        boost::lock_guard<boost::mutex> lock(m_mutex);
        
        BOOST_ASSERT(m_refCount != 0);
        
        if(! m_owner)
            throw ArgumentException("Owner pointer is 0");
        
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
