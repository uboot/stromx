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

#ifndef STROMX_CORE_ENUMPARAMETER_H
#define STROMX_CORE_ENUMPARAMETER_H

#include <vector>
#include "Config.h"
#include "Enum.h"
#include "Parameter.h"

namespace stromx
{
    namespace core
    {
        /** \brief %Description of an enumeration value */
        class EnumDescription
        {
        public:
            EnumDescription(const Enum value, const std::string & description)
            : m_value(value),
                m_description(description)
            {}
            
            const Enum value() const { return m_value; }
            const std::string & description() const { return m_description; }
            
        private:
            Enum m_value;
            std::string m_description;
        };
        
        /** \brief %Description of an enumeration parameter */
        class STROMX_CORE_API EnumParameter : public Parameter
        {
        public:
            EnumParameter(const unsigned int id)
            : Parameter(id, DataVariant::ENUM)
            {}
            
            virtual const std::vector<EnumDescription> & descriptions() const { return m_descriptions; }
            
            virtual void add(const EnumDescription & description);
            virtual void remove(const EnumDescription & description);     
            
        private:
            std::vector<EnumDescription> m_descriptions;
        };
    }
}

#endif // STROMX_CORE_ENUMPARAMETER_H
