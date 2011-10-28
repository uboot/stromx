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

#include "EnumParameter.h"
#include "Exception.h"

namespace core
{   
    void EnumParameter::add(const core::EnumDescription& description)
    {
        for(std::vector<EnumDescription>::const_iterator iter = m_descriptions.begin();
            iter != m_descriptions.end();
            ++iter)
        {
            if(iter->value() == description.value())
                throw WrongArgument("A description with this value has already been added.");
        }
        
        m_descriptions.push_back(description);
    }

    void EnumParameter::remove(const core::EnumDescription& description)
    {
        for(std::vector<EnumDescription>::iterator iter = m_descriptions.begin();
            iter != m_descriptions.end();
            ++iter)
        {
            if(iter->value() == description.value())
                m_descriptions.erase(iter);
            
            return;
        }
        
        throw WrongArgument("No description with this value exists.");
    }
}
