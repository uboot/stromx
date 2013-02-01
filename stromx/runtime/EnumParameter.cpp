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

#include "stromx/runtime/EnumParameter.h"
#include "stromx/runtime/Exception.h"

namespace stromx
{
    namespace runtime
    {   
        void EnumParameter::add(const runtime::EnumDescription& description)
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

        void EnumParameter::remove(const Enum & value)
        {
            for(std::vector<EnumDescription>::iterator iter = m_descriptions.begin();
                iter != m_descriptions.end();
                ++iter)
            {
                if(iter->value() == value)
                    m_descriptions.erase(iter);
                
                return;
            }
            
            throw WrongArgument("No description with this value exists.");
        }
    }
}
