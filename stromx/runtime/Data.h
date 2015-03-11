/* 
 *  Copyright 2011 Matthias Fuchs
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

#ifndef STROMX_RUNTIME_DATA_H
#define STROMX_RUNTIME_DATA_H

#include <typeinfo>
#include "stromx/runtime/Config.h"
#include "stromx/runtime/DataInterface.h"
#include "stromx/runtime/Variant.h"
#include "stromx/runtime/Exception.h"

namespace stromx
{
    namespace runtime
    {
        class Data;
        class DataRef;
        class InputProvider;
        class OutputProvider;
        class Version;
        
        /** \cond */
        template<class data_t>
        class data_traits
        {
        public:
            static const VariantHandle & variant();
        };  
        
        template<>
        class data_traits<Data>
        {
        public:
            static const VariantHandle & variant() { return Variant::DATA; }
        };  
        /** \endcond */
        
        /** \brief Abstract data object. */
        class STROMX_RUNTIME_API Data : public DataInterface
        {
        public:            
            virtual ~Data() {}
            
            virtual void serialize(OutputProvider & out) const;
            virtual void deserialize(InputProvider & in, const Version & version);
        };
        
        /** 
         * Casts a reference to a data object to <tt>data_t &</tt>.
         * Throws BadCast if the cast failed.
         * \throws BadCast
         */
        template<typename data_t>
        data_t & data_cast(Data & data)
        {
            if(data.isVariant(data_traits<data_t>::variant()))
                return reinterpret_cast<data_t &>(data);
            else
                throw BadCast();
        }
        
        /** 
         * Casts a constant reference to a data object to <tt>const data_t &</tt>.
         * Throws BadCast if the cast failed 
         * \throws BadCast
         */
        template<typename data_t>
        const data_t & data_cast(const Data & data)
        {
            if(data.isVariant(data_traits<data_t>::variant()))
                return reinterpret_cast<const data_t &>(data);
            else
                throw BadCast();
        }
        
        /** 
         * Casts a pointer to a data object to \c data_t*.
         * Returns \c 0 if the cast failed
         */
        template<typename data_t>
        data_t* data_cast(Data * data)
        {
            if(data && data->isVariant(data_traits<data_t>::variant()))
                return reinterpret_cast<data_t*>(data);
            else
                return 0;
        }
        
        /** 
         * Casts a pointer to a constant data object to <tt>const data_t*</tt>.
         * Returns \c 0 if the cast failed
         */
        template<typename data_t>
        const data_t* data_cast(const Data * data)
        {
            if(data && data->isVariant(data_traits<data_t>::variant()))
                return reinterpret_cast<const data_t*>(data);
            else
                return 0;
        }
    }
}

#endif // STROMX_RUNTIME_DATA_H