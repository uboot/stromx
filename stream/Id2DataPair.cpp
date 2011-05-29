#include "Id2DataPair.h"

#include "Exception.h"
#include "Id2DataMap.h"

namespace stream
{      
    const bool Id2DataPair::trySet(const Id2DataMap& id2DataMap) const
    {
        return id2DataMap[m_id] == 0;   
    }
    
    const bool Id2DataPair::tryGet(const Id2DataMap& id2DataMap) const
    {
        return id2DataMap[m_id] != 0;
    }
    
    void Id2DataPair::get(stream::Id2DataMap& id2DataMap) const
    {
        if(*m_data != 0)
            throw InvalidStateException("Data has already been assigned to this ID-data pair");
            
        if(id2DataMap[m_id] == 0)
            throw InvalidStateException("The requested output contains 0");
        
        *m_data = id2DataMap[m_id];
        id2DataMap[m_id] = 0;
    }

    void Id2DataPair::set(stream::Id2DataMap& id2DataMap) const
    {
        if(*m_data == 0)
            throw InvalidStateException("This ID-data pair contains no data");
        
        if(id2DataMap[m_id] != 0)
            throw InvalidStateException("Data has already been assigned to this input ID");
        
        id2DataMap[m_id] = *m_data;
       *m_data = 0;
    }
}
