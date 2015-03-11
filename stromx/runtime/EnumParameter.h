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

#ifndef STROMX_RUNTIME_ENUMPARAMETER_H
#define STROMX_RUNTIME_ENUMPARAMETER_H

#include "stromx/runtime/Config.h"
#include "stromx/runtime/Parameter.h"

namespace stromx
{
    namespace runtime
    {
        /** \brief %Description of an enumeration parameter */
        class STROMX_RUNTIME_API EnumParameter : public Parameter
        {
        public:
            /** Constructs an enumeration parameter. */
            EnumParameter(const unsigned int id, ParameterGroup* const group = 0)
              : Parameter(id, Variant::ENUM, group)
            {}
            
            virtual const std::vector<EnumDescription> & descriptions() const { return m_descriptions; }
            
            /** 
             * Adds a description of a possible enumeration value.
             * \throws WrongArgument If description with the same value has already been added.
             */
            virtual void add(const EnumDescription & description);
            
            /** 
             * Removes a value from the possible enumeration values.
             * \throws WrongArgument If no description with the same value exists.
             */
            virtual void remove(const Enum & description);     
            
        private:
            std::vector<EnumDescription> m_descriptions;
        };
    }
}

#endif // STROMX_RUNTIME_ENUMPARAMETER_H
