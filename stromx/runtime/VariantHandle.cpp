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

#include "stromx/runtime/VariantHandle.h"

namespace stromx
{
    namespace runtime
    {
        unsigned int VariantHandle::id() const
        { 
            if (! isValid())
                throw WrongState("This variant handle is not valid.");
            
            return m_variant->id(); 
        }
        
        const std::string & VariantHandle::package() const
        { 
            if (! isValid())
                throw WrongState("This variant handle is not valid.");
            
            return m_variant->package();
        }
        
        const std::string VariantHandle::title() const
        { 
            if (! isValid())
                throw WrongState("This variant handle is not valid.");
            
            return m_variant->title();
        }
        
        bool VariantHandle::isVariant(const VariantInterface& variant) const
        { 
            if (! isValid())
                throw WrongState("This variant handle is not valid.");
            
            return m_variant->isVariant(variant);
            
        }
        
        VariantInterface::CompositeType VariantHandle::compositeType() const
        { 
            if (! isValid())
                throw WrongState("This variant handle is not valid.");
            
            return m_variant->compositeType(); 
        }
        
        const VariantHandle VariantHandle::lhs() const
        { 
            if (! isValid())
                throw WrongState("This variant handle is not valid.");
            
            return m_variant->lhs();
            
        }
        
        const VariantHandle VariantHandle::rhs() const
        { 
            if (! isValid())
                throw WrongState("This variant handle is not valid.");
            
            return m_variant->rhs();
        }
    }
}
