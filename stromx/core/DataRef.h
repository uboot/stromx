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

#ifndef STROMX_CORE_DATAREF_H
#define STROMX_CORE_DATAREF_H

#include "DataInterface.h"

#ifdef __GNUG__
    #include <tr1/memory>
#else
    #include <memory>
#endif

namespace stromx
{
    namespace core
    {
        class Data;
        
        namespace impl
        {
            class SynchronizedOperatorKernel;
        }
        
        /** \brief Reference to a constant data object. */
        class STROMX_CORE_API DataRef
        {
            friend class Data;
            friend class ConstDataRef;
            friend class DataRefTest;
            friend class impl::SynchronizedOperatorKernel;
            
        public:
            DataRef() {}
            operator Data&() { return *m_data; }
            
            virtual const Version & version() const;
            virtual const std::string & type() const;
            virtual const std::string & package() const;
            virtual const DataVariant & variant() const;
            
            const bool isVariant(const DataVariant & v) const;
            virtual Data* const clone() const;
            
            virtual void serialize(OutputProvider & out) const;
            virtual void deserialize(InputProvider & in, const Version & version);
            
        private:
            DataRef(Data* data) : m_data(data) {}
            
            std::tr1::shared_ptr<Data> m_data;
        };
        
        /** \brief Reference to a data object. */
        class STROMX_CORE_API ConstDataRef : public DataInterface
        {
            friend class DataRefTest;
            friend class impl::SynchronizedOperatorKernel;
            
        public:
            ConstDataRef() {}
            ConstDataRef(const DataRef & dataRef) : m_data(dataRef.m_data) {}
            operator const Data&() { return *m_data; }
            
            virtual const Version & version() const;
            virtual const std::string & type() const;
            virtual const std::string & package() const;
            virtual const DataVariant & variant() const;
            
            const bool isVariant(const DataVariant & v) const;
            virtual Data* const clone() const;
            
            virtual void serialize(OutputProvider & out) const;
            virtual void deserialize(InputProvider & in, const Version & version);
            
        private:
            ConstDataRef(const Data* data) : m_data(data) {}
            
            std::tr1::shared_ptr<const Data> m_data;
        };
        
        /** 
         * Casts a data reference to <tt>data_t &</tt>.
         * Throws BadCast if the cast failed.
         * \throws BadCast
         */
        template<typename data_t>
        data_t & data_cast(DataRef & data)
        {
            return data_cast<data_t>((Data &)(data));
        }
        
        /** 
         * Casts a constant data reference to <tt>data_t &</tt>.
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

#endif // STROMX_CORE_DATAREF_H