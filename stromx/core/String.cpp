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

#include "String.h"
#include "InputProvider.h"
#include "OutputProvider.h"

namespace stromx
{
    namespace core
    {
        const std::string String::TYPE = "String";
        const std::string String::PACKAGE = STROMX_CORE_PACKAGE_NAME;
        const Version String::VERSION = Version(STROMX_VERSION_MAJOR, STROMX_VERSION_MINOR, STROMX_VERSION_PATCH);
        
        const DataVariant & String::classVariant()
        { 
            return DataVariant::BOOL;
        }
        
        const DataVariant & String::variant() const 
        {
            return DataVariant::STRING;
        }
        
        void String::serialize(OutputProvider & out) const
        {
            out.text() << m_value;
        }
        
        void String::deserialize(InputProvider & in, const Version & version)
        {
            in.text() >> m_value;
        }
        
        const bool operator==(const stromx::core::String& lhs, const stromx::core::String& rhs)
        {
            return lhs.m_value == rhs.m_value;
        }
    }
}
