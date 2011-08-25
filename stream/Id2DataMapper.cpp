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

#include "Id2DataMapper.h"

namespace stream
{
    bool trySet(const Id2DataMapper& mapper, const impl::Id2DataMap& id2DataMap)
    {
        return mapper.trySet(id2DataMap);
    }  
    
    bool tryGet(const Id2DataMapper& mapper, const impl::Id2DataMap& id2DataMap)
    {
        return mapper.tryGet(id2DataMap);
    }
    
    void get(const Id2DataMapper& mapper, impl::Id2DataMap& id2DataMap)
    {
        return mapper.get(id2DataMap);    
    }
    
    void set(const Id2DataMapper& mapper, impl::Id2DataMap& id2DataMap)
    {
        return mapper.set(id2DataMap);
    }
}
