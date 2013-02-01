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

#ifndef STROMX_RUNTIME_PARAMETERGROUP_H
#define STROMX_RUNTIME_PARAMETERGROUP_H

#include "stromx/runtime/Config.h"
#include "stromx/runtime/Parameter.h"

namespace stromx
{
    namespace runtime
    {
        /** \brief A group of parameters */
        class STROMX_RUNTIME_API ParameterGroup : public Parameter
        {
            friend class Parameter;
            
        public:
            /** Constructs an enumeration parameter. */
            ParameterGroup(const unsigned int id, ParameterGroup* const group = 0);
            
            virtual const std::vector<const Parameter*> & members() const { return m_members; }
            
        private:
            void addMember(const Parameter* const member);
            
            std::vector<const Parameter*> m_members;
        };
    }
}

#endif // STROMX_RUNTIME_PARAMETERGROUP_H
