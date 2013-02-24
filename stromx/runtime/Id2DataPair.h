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

#ifndef STROMX_RUNTIME_ID2DATAPAIR_H
#define STROMX_RUNTIME_ID2DATAPAIR_H

#include "stromx/runtime/Config.h"
#include "stromx/runtime/DataContainer.h"
#include "stromx/runtime/Id2DataMapper.h"

namespace stromx
{
    namespace runtime
    {  
        namespace impl
        {
            class Id2DataMap;
        }
        
        /** \brief A direct map of one ID to one data object. */
        class STROMX_RUNTIME_API Id2DataPair : public Id2DataMapper
        {
        public:
            Id2DataPair(const unsigned int id, DataContainer data)
             : m_id(id),
               m_data(data)
            {}
            
            explicit Id2DataPair(const unsigned int id)
              : m_id(id),
                m_data(DataContainer())
            {}
            
            ~Id2DataPair() {}
            
            unsigned int id() const { return m_id; }
            const DataContainer & data() const { return m_data; }
            
            virtual bool trySet(const impl::Id2DataMap& id2DataMap) const;
            virtual bool tryGet(const impl::Id2DataMap& id2DataMap) const;
            virtual void get(impl::Id2DataMap& id2DataMap) const; 
            virtual void set(impl::Id2DataMap& id2DataMap) const;     
        
        private:
            unsigned int m_id;
            mutable DataContainer m_data;
        }; 
    }
}

#endif // STROMX_RUNTIME_ID2DATAPAIR_H
