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

#include "DataVariant.h"
#include "Exception.h"

#include <typeinfo>

namespace stromx
{
    namespace core
    {
        class Version;
        
        /** \brief Abstract data object */
        class Data
        {
        public:
            virtual ~Data() {}
            
            /** Returns the version of the class of this data object */
            virtual const Version & version() const = 0;
            
            /** Returns the name of the class of this data object */
            virtual const std::string & type() const = 0;
            
            /** Returns the package of the class of this data object */
            virtual const std::string & package() const = 0;
            
            /** Returns the data variant of this data object */
            virtual const DataVariant & variant() const = 0;
            
            /**
             * Returns a copy of the data object, i.e. a object of the same type is
             * allocated and returned. The current value of the data is \em not
             * reproduced.
             */
            virtual Data* const clone() const = 0;
            
            /**
             * Serializs this data object to a string. Additionally, some extra data can be stored
             * in the file <code>path/name.[ext]</code> where the file extension \c ext can be chosen.
             */
            virtual const std::string serialize(const std::string & name, const std::string & path) const;
            
            /**
             * Deserializes a data object from the string \c data. If necessary extra data should
             * be read from the file <code>path/name.[ext]</code> where \c ext is
             * the same as in serialize().
             */
            virtual void deserialize(const std::string & data, const std::string & path);
            
            /** Returns \c true if the variant of this data object is a subtype of \c v. */
            const bool isVariant(const DataVariant & v) const { return variant().isVariant(v); }
        };
        
        /** 
         * Casts the type of a reference to a data object to \c data_t.
         * Throws BadCast if the cast failed.
         * \throws BadCast
         */
        template<typename data_t>
        data_t data_cast(Data & data)
        {
            try
            {
                return dynamic_cast<data_t>(data);
            }
            catch(std::bad_cast &)
            {
                throw BadCast();
            }
        }
        
        /** 
         * Casts the type of a constant reference to a data object to \c data_t.
         * Throws BadCast if the cast failed 
         * \throws BadCast
         */
        template<typename data_t>
        data_t data_cast(const Data & data)
        {
            try
            {
                return dynamic_cast<const data_t>(data);
            }
            catch(std::bad_cast &)
            {
                throw BadCast();
            }
        }
        
        /** 
         * Casts the type of a pointer to a data object to \c data_t.
         * Returns \c 0 if the cast failed
         */
        template<typename data_t>
        data_t data_cast(Data * data)
        {
            return dynamic_cast<data_t>(data);
        }
    }
}

#endif // STROMX_CORE_DATA_H