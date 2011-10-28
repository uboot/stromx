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

#ifndef STROM_PRIMITIVE_H
#define STROM_PRIMITIVE_H

#define __STDC_LIMIT_MACROS

#include <boost/lexical_cast.hpp>
#include <stdint.h>
#include <float.h>
#include "Data.h"
#include "Description.h"
#include "Version.h"

namespace stromx
{
    namespace core
    {
        template<typename data_t>
        class NumericParameter;
        
        /** \brief %Primitive data value */
        template <class repr_t, class val_t>
        class Primitive : public Data
        {
        public:
            explicit Primitive() : m_value(0) {}
            explicit Primitive(const repr_t value) : m_value(value) {}
            
            virtual const std::string & type() const { return TYPE; }
            virtual const Version & version() const { return VERSION; }
            virtual const std::string & package() const { return PACKAGE; }
            
            virtual const DataVariant & variant() const;
            
            virtual Data* const clone() const { return new Primitive<repr_t, val_t>(); }
            
            virtual const std::string serialize(const std::string & name, const std::string & path) const 
            {
                return boost::lexical_cast<std::string>(m_value);
            }
            
            virtual void deserialize(const std::string & data, const std::string & path)
            {
                m_value = boost::lexical_cast<val_t>(data);
            }
            
            operator repr_t() const { return m_value; }
            
            static const Primitive MIN;
            static const Primitive MAX;
            
        private:
            static const std::string TYPE;
            static const std::string PACKAGE;
            static const Version VERSION;
            
            val_t m_value;
        };
        
        typedef Primitive<bool, bool> Bool;
            
        typedef Primitive<int, int8_t> Int8;
        template <>
        const std::string Int8::serialize(const std::string & name, const std::string & path) const;
        template <>
        void Int8::deserialize(const std::string & data, const std::string & path);
        
        typedef Primitive<unsigned int, uint8_t> UInt8;
        template <>
        const std::string UInt8::serialize(const std::string & name, const std::string & path) const;
        template <>
        void UInt8::deserialize(const std::string & data, const std::string & path);
        
        typedef Primitive<int, int16_t> Int16;
        typedef Primitive<unsigned int, uint16_t> UInt16;
        
        typedef Primitive<int, int32_t> Int32;
        typedef Primitive<unsigned int, uint32_t> UInt32;
        
        typedef Primitive<double, float> Float;
        typedef Primitive<double, double> Double;
    }
}
    
#endif // STROM_PRIMITIVE_H
