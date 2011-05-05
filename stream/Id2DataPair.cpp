#include "Id2DataPair.h"

#include "Exception.h"
#include <boost/assert.hpp>

namespace stream
{  
    Id2DataPair::Id2DataPair(const unsigned int id, DataContainer* const data)
        : m_id(id),
        m_data(data)
    {
        if(! m_data)
            throw ArgumentException("Passed 0 as input data");
    }
        
    const bool Id2DataPair::trySet(const Id2DataMap& id2DataMap) const
    {
        Id2DataMap::const_iterator iter = id2DataMap.find(m_id);
        if(iter == id2DataMap.end())
            throw WrongIdException("No input with ID " + m_id);
        
        return iter->second == 0;
    }
    
    const bool Id2DataPair::tryGet(const Id2DataMap& id2DataMap) const
    {
        if(m_data == 0)
            throw InvalidStateException("This ID-data pair contains a 0 data pointer");
        
        Id2DataMap::const_iterator iter = id2DataMap.find(m_id);
        if(iter == id2DataMap.end())
            throw WrongIdException("No output with ID " + m_id);
        
        return iter->second != 0;
    }
    
    void Id2DataPair::get(const stream::Id2DataMap& id2DataMap)
    {
        if(m_data != 0)
            throw InvalidStateException("Data has already been assigned to this ID-data pair");
            
        Id2DataMap::const_iterator iter = id2DataMap.find(m_id);
        if(iter == id2DataMap.end())
            throw WrongIdException("No output with ID " + m_id);
        
        if(iter->second == 0)
            throw InternalException("The requested output contains 0");
        
        m_data = iter->second;
    }

    void Id2DataPair::set(stream::Id2DataMap& id2DataMap) const
    {
        Id2DataMap::iterator iter = id2DataMap.find(m_id);
        if(iter == id2DataMap.end())
            throw WrongIdException("No output with ID " + m_id);
        
        if(iter->second != 0)
            throw InternalException("Data has already been assigned to this input ID");
        
        id2DataMap[m_id] = m_data;
    }
}
