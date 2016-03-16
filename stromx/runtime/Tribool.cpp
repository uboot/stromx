/* 
 *  Copyright 2015 Matthias Fuchs
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
 
#include "stromx/runtime/Tribool.h"

#include "stromx/runtime/InputProvider.h"
#include "stromx/runtime/OutputProvider.h"
#include "stromx/runtime/Version.h"
#include "stromx/runtime/Variant.h"

namespace stromx
{
    namespace runtime
    {        
        const std::string Tribool::TYPE = "Tribool";
        const std::string Tribool::PACKAGE = STROMX_RUNTIME_PACKAGE_NAME;
        const Version Tribool::VERSION = Version(0, 1, 0);
        
        const VariantHandle & data_traits<Tribool>::variant()
        { 
            return Variant::TRIBOOL;
        }
        
        const VariantHandle & Tribool::variant() const 
        {
            return Variant::TRIBOOL;
        }
        
        void Tribool::serialize(OutputProvider & out) const
        {
            out.text() << static_cast<int>(m_value);
        }
        
        void Tribool::deserialize(InputProvider & in, const Version &)
        {
            int temp = 0;
            in.text() >> temp;
            m_value = static_cast<int8_t>(temp);
        }
        
        bool operator==(const stromx::runtime::Tribool& lhs, const stromx::runtime::Tribool& rhs)
        {
            return lhs.m_value == rhs.m_value;
        }
        
        bool operator!=(const stromx::runtime::Tribool& lhs, const stromx::runtime::Tribool& rhs)
        {
            return lhs.m_value != rhs.m_value;
        }
        
        std::ostream& operator<<(std::ostream& out, const Tribool& value)
        {
            return out << value.m_value;
        }
    }
}

