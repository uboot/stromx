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

#ifndef STROMX_RUNTIME_CONSTDATAREF_H
#define STROMX_RUNTIME_CONSTDATAREF_H

#include "stromx/runtime/DataInterface.h"

#ifdef __GNUG__
    #include <tr1/memory>
#else
    #include <memory>
#endif

namespace stromx
{
    namespace runtime
    {
        class Data;
        class DataRef;
        
        /** 
         * \brief Reference to a constant data object.
         * 
         * %Data references hold a smart pointer to a constant data object. They can
         * be copied and passed by value without copying the data object. Instances of
         * this class are automatically casted to <tt>Data &</tt> references. If all copies
         * of a data reference are out of scope the encapsulated data object is automatically
         * deleted.
         */
        class STROMX_RUNTIME_API ConstDataRef : public DataInterface
        {
            friend class DataRefTest;
            
        public:
            /** 
             * Constructs an null data reference. For a null reference the behavior of all member
             * functions with exceptoin of isNull() is undefined.
             */
            ConstDataRef() {}
            
            /**
             * Constructs a data reference from a pointer to a data object. The reference
             * takes ownership of \c data.
             */
            explicit ConstDataRef(const Data* data);
            
            /** 
             * Constructs a constant data reference from the input (non-constant) data
             * reference. They data held by the reference is not copied.
             */
            explicit ConstDataRef(const DataRef & dataRef);

            /** Casts a data reference to <tt>const Data &</tt>. */
            operator const Data&() { return *m_data; }
            
            /** 
             * Returns true if the data reference is a null reference, i.e. it was not initialized
             * by a pointer to an object. Null references can not 
             */
            bool isNull() const { return bool(m_data); }
            
            virtual const Version & version() const;
            virtual const std::string & type() const;
            virtual const std::string & package() const;
            virtual const VariantHandle & variant() const;
            
            bool isVariant(const VariantInterface & v) const;
            virtual Data* clone() const;
            
            virtual void serialize(OutputProvider & out) const;
            virtual void deserialize(InputProvider & in, const Version & version);
            
        private:
            std::tr1::shared_ptr<const Data> m_data;
        };
        
        /** 
         * Casts a constant data reference to <tt>const data_t &</tt>.
         * Throws BadCast if the cast failed.
         * \throws BadCast
         */
        template<typename data_t>
        const data_t & data_cast(ConstDataRef & data)
        {
            return data_cast<data_t>((const Data &)(data));
        }
    }
}

#endif // STROMX_RUNTIME_CONSTDATAREF_H