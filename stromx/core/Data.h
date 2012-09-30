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

#ifndef STROMX_CORE_DATA_H
#define STROMX_CORE_DATA_H

#include <typeinfo>
#include "Config.h"
#include "DataInterface.h"
#include "DataVariant.h"
#include "Exception.h"

namespace stromx
{
    namespace core
    {
        class DataRef;
        class InputProvider;
        class OutputProvider;
        class Version;
        
        /** \brief Abstract data object. */
        class STROMX_CORE_API Data : public DataInterface
        {
        public:
            /** 
             * The most general variant of this data class. 
             * For an object \c data of type \c SomeData the expression
             * <tt>data.isVariant(SomeData.classVariant())</tt> is always true.
             */
            static const DataVariant & classVariant();
            
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
            try
            {
                return dynamic_cast<data_t &>(data);
            }
            catch(std::bad_cast &)
            {
                throw BadCast();
            }
        }
        
        /** 
         * Casts a constant reference to a data object to <tt>const data_t &</tt>.
         * Throws BadCast if the cast failed 
         * \throws BadCast
         */
        template<typename data_t>
        const data_t & data_cast(const Data & data)
        {
            try
            {
                return dynamic_cast<const data_t &>(data);
            }
            catch(std::bad_cast &)
            {
                throw BadCast();
            }
        }
        
        /** 
         * Casts a pointer to a data object to \c data_t*.
         * Returns \c 0 if the cast failed
         */
        template<typename data_t>
        data_t* data_cast(Data * data)
        {
            return dynamic_cast<data_t*>(data);
        }
        
        /** 
         * Casts a pointer to a constant data object to <tt>const data_t*</tt>.
         * Returns \c 0 if the cast failed
         */
        template<typename data_t>
        const data_t* data_cast(const Data * data)
        {
            return dynamic_cast<const data_t*>(data);
        }
    }
}

#endif // STROMX_CORE_DATA_H