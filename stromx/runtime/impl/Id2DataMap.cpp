/* 
 *  Copyright 2011 Matthias Fuchs
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

#include "stromx/runtime/DataContainer.h"
#include "stromx/runtime/Exception.h"
#include "stromx/runtime/Input.h"
#include "stromx/runtime/Output.h"
#include "stromx/runtime/Parameter.h"
#include "stromx/runtime/impl/Id2DataMap.h"

namespace stromx
{
    namespace runtime
    {
        namespace 
        {
            template <class description_t>
            bool isConnector(const Parameter* parameter);
            
            template <>
            bool isConnector<Input>(const Parameter* parameter)
            {
                return parameter->originalType() == Description::INPUT;
            } 
            
            template <>
            bool isConnector<Output>(const Parameter* parameter)
            {
                return parameter->originalType() == Description::OUTPUT;
            }            
            
            template <class description_t>
            void populateId2DataMap(const std::vector<const description_t*> & descriptions,
                                    const std::vector<const Parameter*> & parameters, 
                                    std::map<unsigned int, DataContainer> & map,
                                    std::set<unsigned int> & persistentParameters)
            {
                map.clear();
                persistentParameters.clear();
                
                for(typename std::vector<const description_t*>::const_iterator iter = descriptions.begin();
                    iter != descriptions.end();
                    ++iter)
                {
                    if(map.count((*iter)->id()))
                        throw WrongArgument("Two descriptors with the same ID");
                    
                    map[(*iter)->id()] = DataContainer();
                }
                
                for(typename std::vector<const Parameter*>::const_iterator iter = parameters.begin();
                    iter != parameters.end();
                    ++iter)
                {
                    if (! isConnector<description_t>(*iter))
                        continue;
                        
                    if(map.count((*iter)->id()))
                        throw WrongArgument("Two descriptors with the same ID");
                    
                    map[(*iter)->id()] = DataContainer();
                    
                    if ((*iter)->updateBehavior() == Description::PERSISTENT)
                        persistentParameters.insert((*iter)->id());
                }
            }
        }

        namespace impl
        {
            Id2DataMap::Id2DataMap()
              : m_observer(0)
            {
            }
            
            void Id2DataMap::initialize(const std::vector<const Input*> & descriptions, const std::vector<const Parameter*> & parameters)
            {
                populateId2DataMap(descriptions, parameters, m_map, m_persistentParameters);
            }
            
            void Id2DataMap::initialize(const std::vector<const Output*> & descriptions, const std::vector<const Parameter*> & parameters)
            {
                populateId2DataMap(descriptions, parameters, m_map, m_persistentParameters);
            }

            void Id2DataMap::setObserver(const Id2DataMapObserver* const observer)
            {
                m_observer = observer;
            }
            
            const DataContainer & Id2DataMap::get(const unsigned int id) const
            {
                std::map<unsigned int, DataContainer>::const_iterator iter = m_map.find(id);
                if(iter == m_map.end())
                    throw WrongId("No data with ID " + id);
                
                return iter->second;
            }
            
            void Id2DataMap::set(const unsigned int id, const DataContainer & data)
            {
                std::map<unsigned int, DataContainer>::iterator iter = m_map.find(id);
                if(iter == m_map.end())
                    throw WrongId("No data with ID " + id);
                
                DataContainer oldData = iter->second;
                iter->second = data;
                
                if(m_observer)
                    m_observer->observe(id, oldData, data);
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
            
            bool Id2DataMap::empty() const
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
            
            bool Id2DataMap::canBeSet(const unsigned int id) const
            {
                if (m_persistentParameters.count(id))
                    return true;
                    
                return get(id).empty();
            }
            
            bool Id2DataMap::mustBeReset(const unsigned int id) const
            {
                if (m_persistentParameters.count(id))
                    return false;
                    
                return true;
            }
        }
    }
}
