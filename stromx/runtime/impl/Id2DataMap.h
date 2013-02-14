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

#ifndef STROMX_RUNTIME_IMPL_ID2DATAMAP_H
#define STROMX_RUNTIME_IMPL_ID2DATAMAP_H

#include <map>
#include <vector>
#include "stromx/runtime/Description.h"

namespace stromx
{
    namespace runtime
    {
        class DataContainer;
        
        namespace impl
        {
            class Id2DataMapObserver
            {
            public:
                virtual void observe(const unsigned int id, const DataContainer & data) const = 0;
            };
           
            class Id2DataMap
            {
            public:
                Id2DataMap();
                explicit Id2DataMap(const std::vector<const Description*> & descriptions);                
                
                const DataContainer & get(const unsigned int id) const;
                void set(const unsigned int id, const DataContainer & data);
                void clear();
                bool empty() const;
                void setObserver(const Id2DataMapObserver* const observer);
                
            private:
                std::map<unsigned int, DataContainer> m_map;
                const Id2DataMapObserver* m_observer;
            };
        }
    }
}

#endif // STROMX_RUNTIME_IMPL_ID2DATAMAP_H
