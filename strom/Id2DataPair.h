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

#ifndef STREAM_ID2DATAPAIR_H
#define STREAM_ID2DATAPAIR_H

#include "DataContainer.h"
#include "Id2DataMapper.h"

namespace strom
{  
    namespace impl
    {
        class Id2DataMap;
    }
    
    class Id2DataPair : public Id2DataMapper
    {
    public:
        Id2DataPair(const unsigned int id, DataContainer data)
          : m_id(id),
            m_data(DataContainer())
        { m_data = data; }
        
        Id2DataPair(const unsigned int id)
          : m_id(id),
            m_data(DataContainer())
        {}
        
        ~Id2DataPair() {}
        
        const unsigned int id() const { return m_id; }
        const DataContainer & data() const { return m_data; }
        
        virtual const bool trySet(const impl::Id2DataMap& id2DataMap) const;
        virtual const bool tryGet(const impl::Id2DataMap& id2DataMap) const;
        virtual void get(impl::Id2DataMap& id2DataMap) const; 
        virtual void set(impl::Id2DataMap& id2DataMap) const;     
    
    private:
        unsigned int m_id;
        mutable DataContainer m_data;
    }; 
}

#endif // STREAM_ID2DATAPAIR_H
