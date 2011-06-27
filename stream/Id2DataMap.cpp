#include "Id2DataMap.h"
#include "Exception.h"
#include "DataContainer.h"

namespace stream
{
    Id2DataMap::Id2DataMap(const std::vector<Description*> & descriptions)
    {
        for(std::vector<Description*>::const_iterator iter = descriptions.begin();
            iter != descriptions.end();
            ++iter)
        {
            if(m_map.count((*iter)->id()))
                throw ArgumentException("Two descriptors with the same ID");
            
            m_map[(*iter)->id()] = 0;
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
    
    void Id2DataMap::clear()
    {
        for(std::map<unsigned int, DataContainer*>::iterator iter = m_map.begin();
            iter != m_map.end();
            ++iter)
        {
            if(iter->second)
                iter->second->dereference();
            
            iter->second = 0;
        }       
    }
    
    const bool Id2DataMap::isEmpty() const
    {
        bool value = true;
        for(std::map<unsigned int, DataContainer*>::const_iterator iter = m_map.begin();
            iter != m_map.end();
            ++iter)
        {
            if(iter->second)
            {
                value = false;
                break;
            }
        }   
        
        return value;
    }
}
