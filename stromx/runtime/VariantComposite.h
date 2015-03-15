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

#ifndef STROMX_RUNTIME_VARIANTCOMPOSITE_H
#define STROMX_RUNTIME_VARIANTCOMPOSITE_H

#include "stromx/runtime/VariantHandle.h"

namespace stromx
{
    namespace runtime
    {
        /** \brief A composite expression of two variants. */
        class STROMX_RUNTIME_API VariantComposite : public VariantInterface
        {
        public:
            STROMX_RUNTIME_API friend const VariantHandle operator&&(const VariantHandle & lhs, const VariantHandle & rhs);
            STROMX_RUNTIME_API friend const VariantHandle operator||(const VariantHandle & lhs, const VariantHandle & rhs);
            
            virtual unsigned int id() const { return 0; }
            virtual const std::string & package() const;
            virtual const std::string title() const;
            virtual bool isVariant(const VariantInterface& variant) const;
            virtual CompositeType compositeType() const { return m_type; }
            virtual const VariantHandle lhs() const { return m_lhs; }
            virtual const VariantHandle rhs() const { return m_rhs; }
            
        private:  
            VariantComposite(const CompositeType type, const VariantHandle & lhs, const VariantHandle & rhs);
            
            const VariantHandle m_lhs;
            const VariantHandle m_rhs;
            const CompositeType m_type;
        };
        
        /** 
         * Constructs a variant which is a valid super-type of subtypes which are 
         * \c lhs or \c rhs. 
         * 
         * \throws WrongArgument If \c lhs or \c rhs is not valid.
         */
        STROMX_RUNTIME_API const VariantHandle operator||(const VariantHandle & lhs, const VariantHandle & rhs);
    }
}

#endif // STROMX_RUNTIME_VARIANTCOMPOSITE_H