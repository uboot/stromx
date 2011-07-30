#include "Id2DataPair.h"

#include "Exception.h"
#include "DataContainer.h"
#include "impl/Id2DataMap.h"

namespace stream
{      
    const bool Id2DataPair::trySet(const impl::Id2DataMap& id2DataMap) const
    {
        return id2DataMap[m_id].empty();   
    }
    
    const bool Id2DataPair::tryGet(const impl::Id2DataMap& id2DataMap) const
    {
        return ! id2DataMap[m_id].empty();
    }
    
    void Id2DataPair::get(stream::impl::Id2DataMap& id2DataMap) const
    {
        if(! m_data.empty())
            throw InvalidStateException("Data has already been assigned to this ID-data pair");
            
        if(id2DataMap[m_id].empty())
            throw InvalidStateException("The requested output is empty");
        
        m_data = id2DataMap[m_id];
        id2DataMap[m_id] = DataContainer();
    }

    void Id2DataPair::set(stream::impl::Id2DataMap& id2DataMap) const
    {
        if(m_data.empty())
            throw InvalidStateException("This ID-data pair contains no data");
        
        if(! id2DataMap[m_id].empty())
            throw InvalidStateException("Data has already been assigned to this input ID");
        
        id2DataMap[m_id] = m_data;
        m_data = DataContainer();
    }
}
