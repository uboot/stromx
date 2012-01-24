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

#ifndef STROMX_CORE_PRIMITIVE_H
#define STROMX_CORE_PRIMITIVE_H

#define __STDC_LIMIT_MACROS

#include <float.h>
#include <stdint.h>
#include "Data.h"
#include "Description.h"
#include "Version.h"

namespace stromx
{
    namespace core
    {
        template<typename data_t>
        class NumericParameter;
        
        /** \brief %Primitive data value. */
        template <class repr_t, class val_t>
        class STROMX_CORE_API Primitive : public Data
        {
        public:
            /** Constructs a primitive and initializes it with 0. */
            explicit Primitive() : m_value(0) {}
            
            /** Constructs a primitive and initializes it with \c value. */
            explicit Primitive(const repr_t value) : m_value(val_t(value)) {}
            
            virtual const std::string & type() const { return TYPE; }
            virtual const Version & version() const { return VERSION; }
            virtual const std::string & package() const { return PACKAGE; }
            
            virtual const DataVariant & variant() const;
            
            virtual Data* const clone() const { return new Primitive<repr_t, val_t>(); }
            
            virtual void serialize(OutputProvider & out) const;
            
            virtual void deserialize(InputProvider & in);
            
            /** Returns the value of the primitive. */
            const repr_t get() const {  return m_value; }
            
            /** Returns the value of the primitive. */
            operator repr_t() const { return get(); }
            
            /** Assigns \c value to the primitive. */
            const Primitive operator=(const repr_t value)
            { 
                m_value = val_t(value);
                return *this;
            }
            
            /** The minimal value which can be stored by this primitive. */
            static const Primitive MIN;
            
            /** The maximal value which can be stored by this primitive. */
            static const Primitive MAX;
            
        private:
            static const std::string TYPE;
            static const std::string PACKAGE;
            static const Version VERSION;
            
            val_t m_value;
        };


        /** \brief Boolean value */
        typedef Primitive<bool, bool> Bool;
        template<>
        STROMX_CORE_API const DataVariant & Bool::variant() const;
        template <>
        STROMX_CORE_API void Bool::serialize(OutputProvider & out) const;
        template <>
        STROMX_CORE_API void Bool::deserialize(InputProvider & in);
        
        /** \brief Signed 8-bit integer */
        typedef Primitive<int, int8_t> Int8;
        template<>
        STROMX_CORE_API const DataVariant & Int8::variant() const;
        template <>
        STROMX_CORE_API void Int8::serialize(OutputProvider & out) const;
        template <>
        STROMX_CORE_API void Int8::deserialize(InputProvider & in);
        
        /** \brief Unsigned 8-bit integer */
        typedef Primitive<unsigned int, uint8_t> UInt8;
        template<>
        STROMX_CORE_API const DataVariant & UInt8::variant() const;
        template <>
        STROMX_CORE_API void UInt8::serialize(OutputProvider & out) const;
        template <>
        STROMX_CORE_API void UInt8::deserialize(InputProvider & in);
        
        /** \brief Signed 16-bit integer */
        typedef Primitive<int, int16_t> Int16;
        template<>
        STROMX_CORE_API const DataVariant & Int16::variant() const;
        template <>
        STROMX_CORE_API void Int16::serialize(OutputProvider & out) const;
        template <>
        STROMX_CORE_API void Int16::deserialize(InputProvider & in);

        /** \brief Unsigned 16-bit integer */
        typedef Primitive<unsigned int, uint16_t> UInt16;
        template<>
        STROMX_CORE_API const DataVariant & UInt16::variant() const;
        template <>
        STROMX_CORE_API void UInt16::serialize(OutputProvider & out) const;
        template <>
        STROMX_CORE_API void UInt16::deserialize(InputProvider & in);
        
        /** \brief Signed 32-bit integer */
        typedef Primitive<int, int32_t> Int32;
        template<>
        STROMX_CORE_API const DataVariant & Int32::variant() const;
        template <>
        STROMX_CORE_API void Int32::serialize(OutputProvider & out) const;
        template <>
        STROMX_CORE_API void Int32::deserialize(InputProvider & in);
        
        /** \brief Unsigned 32-bit integer */
        typedef Primitive<unsigned int, uint32_t> UInt32;
        template<>
        STROMX_CORE_API const DataVariant & UInt32::variant() const;
        template <>
        STROMX_CORE_API void UInt32::serialize(OutputProvider & out) const;
        template <>
        STROMX_CORE_API void UInt32::deserialize(InputProvider & in);
        
        /** \brief Single precision floating point number */
        typedef Primitive<double, float> Float;
        template<>
        STROMX_CORE_API const DataVariant & Float::variant() const;
        template <>
        STROMX_CORE_API void Float::serialize(OutputProvider & out) const;
        template <>
        STROMX_CORE_API void Float::deserialize(InputProvider & in);

        /** \brief Double precision floating point number */
        typedef Primitive<double, double> Double;
        template<>
        STROMX_CORE_API const DataVariant & Double::variant() const;
        template <>
        STROMX_CORE_API void Double::serialize(OutputProvider & out) const;
        template <>
        STROMX_CORE_API void Double::deserialize(InputProvider & in);

        /** \cond */
        template class STROMX_CORE_API Primitive<bool, bool>;
        template class STROMX_CORE_API Primitive<int, int8_t>;    
        template class STROMX_CORE_API Primitive<unsigned int, uint8_t>; 
        template class STROMX_CORE_API Primitive<int, int16_t>;   
        template class STROMX_CORE_API Primitive<unsigned int, uint16_t>;   
        template class STROMX_CORE_API Primitive<int, int32_t>;   
        template class STROMX_CORE_API Primitive<unsigned int, uint32_t>;   
        template class STROMX_CORE_API Primitive<double, float>;   
        template class STROMX_CORE_API Primitive<double, double>;   
        /** \endcond */
    }
}
    
#endif // STROMX_CORE_PRIMITIVE_H
