/* 
 *  Copyright 2012 Matthias Fuchs
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

#include "stromx/runtime/Exception.h"
#include "stromx/runtime/ParameterGroup.h"
#include <algorithm>
#include <boost/assert.hpp>

namespace stromx
{
    namespace runtime
    {   
        ParameterGroup::ParameterGroup(const unsigned int id, ParameterGroup*const group)
          : Parameter(id, Variant::NONE, group)
        {
            setAccessMode(NO_ACCESS);
        }
            
        void ParameterGroup::addMember(const stromx::runtime::Parameter*const member)
        {              
            std::vector<const Parameter*>::const_iterator iter = 
                std::find(m_members.begin(), m_members.end(), member);
            
            BOOST_ASSERT(iter == m_members.end());
            
            m_members.push_back(member);
        }
    }
}
