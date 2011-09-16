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

#ifndef STREAM_ENUMPARAMETER_H
#define STREAM_ENUMPARAMETER_H

#include <vector>
#include "Enum.h"
#include "Parameter.h"

namespace strom
{
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
    
    class EnumParameter : public Parameter
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

#endif // STREAM_ENUMPARAMETER_H
