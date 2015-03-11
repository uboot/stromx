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

#include "stromx/runtime/Parameter.h"
#include "stromx/runtime/ParameterGroup.h"

namespace stromx
{
    namespace runtime
    {
        const std::vector<const Parameter*> Parameter::NO_MEMBERS = std::vector<const Parameter*>();
        
        Parameter::Parameter(const unsigned int id, const stromx::runtime::VariantHandle& variant,
                             ParameterGroup* const group)
          : Description(id, variant),
            m_access(NO_ACCESS),
            m_group(group)
        {
            if(group)
                group->addMember(this);
        }
    }
}