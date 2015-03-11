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

#ifndef STROMX_RUNTIME_DATAINTERFACE_H
#define STROMX_RUNTIME_DATAINTERFACE_H

#include "stromx/runtime/VariantHandle.h"

namespace stromx
{
    namespace runtime
    {
        class Data;
        class InputProvider;
        class OutputProvider;
        class Version;
        
        /** \brief Common interface of data objects or references to data objects.  */
        class DataInterface
        {
        public:
            virtual ~DataInterface() {}
            
            /** Returns the version of the class of this data object. */
            virtual const Version & version() const = 0;
            
            /** Returns the name of the class of this data object. */
            virtual const std::string & type() const = 0;
            
            /** Returns the package of the class of this data object. */
            virtual const std::string & package() const = 0;
            
            /** Returns the data variant of this data object. */
            virtual const VariantHandle & variant() const = 0;
            
            /**
             * Returns a copy of the data object, i.e. a object of the same type is
             * allocated and the data of the original object is copied to the new object.
             * A pointer to the new object is returned.
             */
            virtual Data* clone() const = 0;
            
            /**
             * Serializes this data object.
             * 
             * \param out The output which the data is sent to.
             */
            virtual void serialize(OutputProvider & out) const = 0;
            
            /**
             * Deserializes a data object.
             * 
             * \param out The input which the data obtained from.
             * \param version The version of the data class which serialized the input.
             */
            virtual void deserialize(InputProvider & in, const Version & version) = 0;
            
            /** Returns \c true if the variant of this data object is a subtype of \c v. */
            bool isVariant(const VariantInterface & v) const { return variant().isVariant(v); }
        };
    }
}

#endif // STROMX_RUNTIME_DATAINTERFACE_H