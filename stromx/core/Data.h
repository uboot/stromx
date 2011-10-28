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

#ifndef STROM_DATA_H
#define STROM_DATA_H

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
            
            virtual const Version & version() const = 0;
            virtual const std::string & type() const = 0;
            virtual const std::string & package() const = 0;
            virtual const DataVariant & variant() const = 0;
            
            virtual Data* const clone() const = 0;
            
            virtual const std::string serialize(const std::string & name, const std::string & path) const;
            virtual void deserialize(const std::string & data, const std::string & path);
            
            const bool is(const DataVariant & t) const { return variant().is(t); }
            
        protected:
            Data() {} 
        };
        
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
        
        template<typename data_t>
        data_t data_cast(Data * data)
        {
            return dynamic_cast<data_t>(data);
        }
    }
}

#endif // STROM_DATA_H