#include "DataContainer.h"

#include "Data.h"

namespace stream
{
    DataContainer::DataContainer ( const unsigned int id, const stream::Operator*const owner, stream::Data* const data )
      : m_id(id),
        m_owner(owner),
        m_data(data),
        m_refCount(1)
    {
    }
    
    void DataContainer::reference()
    {
        m_refCount++;
    }

    void DataContainer::dereference()
    {
        m_refCount--;
        
        if(m_refCount == 0)
        {
            delete m_data;
            m_data = 0;
        }
    }

}
