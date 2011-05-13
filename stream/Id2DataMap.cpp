#include "Id2DataMap.h"
#include "Exception.h"

namespace stream
{
    Id2DataMap::Id2DataMap(const std::vector<Descriptor> & descriptions)
    {
        for(std::vector<Descriptor>::const_iterator iter = descriptions.begin();
            iter != descriptions.end();
            ++iter)
        {
            if(m_map.count(iter->id()))
                throw ArgumentException("Two descriptors with the same ID");
            
            m_map[iter->id()] = 0;
        }
    }
    
    stream::DataContainer*const Id2DataMap::operator[](const unsigned int id) const
    {
        std::map<unsigned int, DataContainer*>::const_iterator iter = m_map.find(id);
        if(iter == m_map.end())
            throw WrongIdException("No data with ID " + id);
        
        return iter->second;
    }
    
    DataContainer*& Id2DataMap::operator[](const unsigned int id)
    {
        std::map<unsigned int, DataContainer*>::iterator iter = m_map.find(id);
        if(iter == m_map.end())
            throw WrongIdException("No data with ID " + id);
        
        return iter->second;
    }

}
