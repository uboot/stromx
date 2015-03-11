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
 *  See the License for the specific language governing permisVariantsions and
 *  limitations under the License.
 */

#include "stromx/runtime/VariantInterface.h"
#include "stromx/runtime/VariantHandle.h"

namespace stromx
{
    namespace runtime
    {
        const VariantHandle VariantInterface::lhs() const
        {
            return VariantHandle();
        }
        
        const VariantHandle VariantInterface::rhs() const
        {
            return VariantHandle();
        }
            
        bool operator==(const VariantInterface & lhs, const VariantInterface & rhs)
        {
            return lhs.id() == rhs.id() && lhs.package() == rhs.package();
        }
        
        std::ostream& operator<<(std::ostream& out, const VariantInterface& variant)
        {
            out << variant.package() << ", " << variant.id();
            return out;
        }
    }
}
