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

#ifndef STROMX_RUNTIME_VARIANTHANDLE_H
#define STROMX_RUNTIME_VARIANTHANDLE_H

#include "stromx/runtime/VariantInterface.h"
#include "stromx/runtime/Exception.h"

#ifdef __GNUG__
    #include <tr1/memory>
#else
    #include <memory>
#endif

namespace stromx
{
    namespace runtime
    {
        /** Handle for VariantInterface objects which can be passed by value. */
        class VariantHandle : public VariantInterface
        {            
            friend class VariantInterface;
            
        public:
            /** 
             * Creates a variant handle for \c variant. The handle gains ownership
             * of \c variant.
             *
             * \throws WrongArgument If \c variant is null.
             */
            VariantHandle(VariantInterface* const variant)
              : m_variant(variant)
            {
                if (variant == 0)
                {
                    throw WrongArgument("Variant must not be 0.");
                }
            }
            
            unsigned int id() const { return m_variant->id(); }
            const std::string & package() const { return m_variant->package(); }
            virtual const std::string title() const { return m_variant->title(); }
            virtual bool isVariant(const VariantInterface& variant) const { return m_variant->isVariant(variant); }
            virtual CompositeType compositeType() const { return m_variant->compositeType(); }
            virtual const VariantHandle lhs() const { return m_variant->lhs(); }
            virtual const VariantHandle rhs() const { return m_variant->rhs(); }
            
            /** Returns whether the handle holds a reference to an actual variant. */
            bool isValid() const { return m_variant.get() != 0; }
            
        private:
            VariantHandle() {}
            
            std::tr1::shared_ptr<VariantInterface> m_variant;
        };
    }
}

#endif // STROMX_RUNTIME_VARIANTHANDLE_H
