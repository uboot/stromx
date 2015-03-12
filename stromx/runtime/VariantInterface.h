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

#ifndef STROMX_RUNTIME_VARIANTINTERFACE_H
#define STROMX_RUNTIME_VARIANTINTERFACE_H

#include "stromx/runtime/Config.h"

#include <ostream>

namespace stromx
{
    namespace runtime
    {
        class VariantHandle;
        
        /** \brief Abstract description of a data variant.
         *
         * A variant describes type of a Data object, i.e. which subclass of Data the object
         * is an instance of. Some variants give further details about an instance of a data
         * data class e.g the type of the entries of a Matrix object (their type is not
         * characterized by the class type but by a class member).
         *
         * A variant is uniquely characterized by its ID and the package it
         * belongs to.
         */
        class VariantInterface
        {            
        public:
            enum CompositeType
            {
                NO_COMPOSITE,
                AND_COMPOSITE,
                OR_COMPOSITE
            };
            
            /** Returns the integer ID of this data variant. */
            virtual unsigned int id() const = 0;
            
            /** Returns the package this data variant belongs to. */
            virtual const std::string & package() const = 0;
            
            /** Returns the title of the data variant. */
            virtual const std::string title() const = 0;
            
            /**
             * Returns true if \c this is a subtype of \c type. E.g.
             * <tt>INT_16.isVariant(INT) == true</tt>.
             */
            virtual bool isVariant(const VariantInterface& variant) const = 0;
            
            /** 
             * The composite type if this variant is a logical composition of two 
             * other variants.
             */
            virtual CompositeType compositeType() const { return NO_COMPOSITE; }
            
            /** The left hand side of the composition. */
            virtual const VariantHandle lhs() const;
            
            /** The right hand side of the composition. */
            virtual const VariantHandle rhs() const;
        };
        
        STROMX_RUNTIME_API bool operator==(const VariantInterface & lhs, const VariantInterface & rhs);
        STROMX_RUNTIME_API std::ostream& operator<< (std::ostream& out, const VariantInterface & container);
    }
}

#endif // STROMX_RUNTIME_VARIANTINTERFACE_H
