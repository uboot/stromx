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

#include "stromx/runtime/Locale.h"
#include "stromx/runtime/VariantComposite.h"

#include <boost/assert.hpp>

namespace
{
    static const std::string EMPTY_STRING = "";
}
        
namespace stromx
{
    namespace runtime
    {
        VariantComposite::VariantComposite(const CompositeType type, const VariantHandle & lhs, 
            const VariantHandle & rhs) : m_lhs(lhs), m_rhs(rhs), m_type(type)
        {
            if (!lhs.isValid() || !rhs.isValid())
            {
                throw WrongArgument("Both components of a variant composite must be valid.");
            }
        }
        
        bool VariantComposite::isVariant(const VariantInterface& variant) const
        { 
            switch (compositeType())
            {
            case VariantInterface::AND_COMPOSITE:
                return lhs().isVariant(variant) || rhs().isVariant(variant);
            case VariantInterface::OR_COMPOSITE:
                return lhs().isVariant(variant) && rhs().isVariant(variant);
            default:
                BOOST_ASSERT(false);
            }
            
            return false;
        }
        
        const std::string & VariantComposite::package() const
        {
            return EMPTY_STRING;
        }
        
        const std::string VariantComposite::title() const
        {
            switch (compositeType())
            {
            case VariantInterface::AND_COMPOSITE:
                return lhs().title() + L_(" and ") + rhs().title();
            case VariantInterface::OR_COMPOSITE:
                return lhs().title() + L_(" or ") + rhs().title();
            default:
                BOOST_ASSERT(false);
            }
            
            return "";
        }
            
        const VariantHandle operator&&(const VariantHandle & lhs, const VariantHandle & rhs)
        {
            return VariantHandle(new VariantComposite(VariantInterface::AND_COMPOSITE, lhs, rhs));
        }
        
        const VariantHandle operator||(const VariantHandle & lhs, const VariantHandle & rhs)
        {
            return VariantHandle(new VariantComposite(VariantInterface::OR_COMPOSITE, lhs, rhs));
        }
    }
}
