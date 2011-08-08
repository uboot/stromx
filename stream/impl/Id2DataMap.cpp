#include "Id2DataMap.h"
#include "../Exception.h"
#include "../DataContainer.h"

namespace stream
{
    namespace impl
    {
        Id2DataMap::Id2DataMap(const std::vector<const Description*> & descriptions)
        {
            for(std::vector<const Description*>::const_iterator iter = descriptions.begin();
                iter != descriptions.end();
                ++iter)
            {
                if(m_map.count((*iter)->id()))
                    throw WrongArgument("Two descriptors with the same ID");
                
                m_map[(*iter)->id()] = DataContainer();
            }
        }
        
        Id2DataMap::Id2DataMap()
        {
        }
        
        stream::DataContainer Id2DataMap::operator[](const unsigned int id) const
        {
            std::map<unsigned int, DataContainer>::const_iterator iter = m_map.find(id);
            if(iter == m_map.end())
                throw WrongId("No data with ID " + id);
            
            return iter->second;
        }
        
        DataContainer & Id2DataMap::operator[](const unsigned int id)
        {
            std::map<unsigned int, DataContainer>::iterator iter = m_map.find(id);
            if(iter == m_map.end())
                throw WrongId("No data with ID " + id);
            
            return iter->second;
        }
        
        void Id2DataMap::clear()
        {
            for(std::map<unsigned int, DataContainer>::iterator iter = m_map.begin();
                iter != m_map.end();
                ++iter)
            {
                iter->second = DataContainer();
            }       
        }
        
        const bool Id2DataMap::isEmpty() const
        {
            bool value = true;
            for(std::map<unsigned int, DataContainer>::const_iterator iter = m_map.begin();
                iter != m_map.end();
                ++iter)
            {
                if(! iter->second.empty())
                {
                    value = false;
                    break;
                }
            }   
            
            return value;
        }
    }
}
