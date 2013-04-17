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

#ifndef STROMX_RUNTIME_PRIMITIVE_H
#define STROMX_RUNTIME_PRIMITIVE_H

#ifdef __GNUG__
    #include <tr1/cstdint>
#else
    #include <cstdint>
#endif

#include "stromx/runtime/Data.h"
#include "stromx/runtime/Description.h"
#include "stromx/runtime/Version.h"

namespace stromx
{
    namespace runtime
    {
        template<typename data_t>
        class NumericParameter;
        
        /** \brief %Primitive data value. */
        template <class repr_t, class val_t>
        class STROMX_RUNTIME_API Primitive : public Data
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
            
            virtual Data* clone() const { return new Primitive<repr_t, val_t>(m_value); }
            
            virtual void serialize(OutputProvider & out) const;
            
            virtual void deserialize(InputProvider & in, const Version & version);
            
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
        STROMX_RUNTIME_API const DataVariant & Bool::variant() const;
        template <>
        STROMX_RUNTIME_API void Bool::serialize(OutputProvider & out) const;
        template <>
        STROMX_RUNTIME_API void Bool::deserialize(InputProvider & in, const Version & version);
        
        /** \cond */
        template <>
        class data_traits<Bool>
        {
        public:
            static const DataVariant & variant() { return DataVariant::BOOL; }
        };
        /** \endcond */
        
        /** \brief Signed 8-bit integer */
        typedef Primitive<int, int8_t> Int8;
        template<>
        STROMX_RUNTIME_API const DataVariant & Int8::variant() const;
        template <>
        STROMX_RUNTIME_API void Int8::serialize(OutputProvider & out) const;
        template <>
        STROMX_RUNTIME_API void Int8::deserialize(InputProvider & in, const Version & version);
        
        /** \cond */
        template <>
        class data_traits<Int8>
        {
        public:
            static const DataVariant & variant() { return DataVariant::INT_8; }
        };  
        /** \endcond */
        
        /** \brief Unsigned 8-bit integer */
        typedef Primitive<unsigned int, uint8_t> UInt8;
        template<>
        STROMX_RUNTIME_API const DataVariant & UInt8::variant() const;
        template <>
        STROMX_RUNTIME_API void UInt8::serialize(OutputProvider & out) const;
        template <>
        STROMX_RUNTIME_API void UInt8::deserialize(InputProvider & in, const Version & version);
        
        /** \cond */
        template <>
        class data_traits<UInt8>
        {
        public:
            static const DataVariant & variant() { return DataVariant::UINT_8; }
        };  
        /** \endcond */
        
        /** \brief Signed 16-bit integer */
        typedef Primitive<int, int16_t> Int16;
        template<>
        STROMX_RUNTIME_API const DataVariant & Int16::variant() const;
        template <>
        STROMX_RUNTIME_API void Int16::serialize(OutputProvider & out) const;
        template <>
        STROMX_RUNTIME_API void Int16::deserialize(InputProvider & in, const Version & version);
        
        /** \cond */
        template <>
        class data_traits<Int16>
        {
        public:
            static const DataVariant & variant() { return DataVariant::INT_16; }
        };  
        /** \endcond */

        /** \brief Unsigned 16-bit integer */
        typedef Primitive<unsigned int, uint16_t> UInt16;
        template<>
        STROMX_RUNTIME_API const DataVariant & UInt16::variant() const;
        template <>
        STROMX_RUNTIME_API void UInt16::serialize(OutputProvider & out) const;
        template <>
        STROMX_RUNTIME_API void UInt16::deserialize(InputProvider & in, const Version & version);
        
        /** \cond */
        template <>
        class data_traits<UInt16>
        {
        public:
            static const DataVariant & variant() { return DataVariant::UINT_16; }
        };  
        /** \endcond */
        
        /** \brief Signed 32-bit integer */
        typedef Primitive<int, int32_t> Int32;
        template<>
        STROMX_RUNTIME_API const DataVariant & Int32::variant() const;
        template <>
        STROMX_RUNTIME_API void Int32::serialize(OutputProvider & out) const;
        template <>
        STROMX_RUNTIME_API void Int32::deserialize(InputProvider & in, const Version & version);
        
        /** \cond */
        template <>
        class data_traits<Int32>
        {
        public:
            static const DataVariant & variant() { return DataVariant::INT_32; }
        };  
        /** \endcond */
        
        /** \brief Unsigned 32-bit integer */
        typedef Primitive<unsigned int, uint32_t> UInt32;
        template<>
        STROMX_RUNTIME_API const DataVariant & UInt32::variant() const;
        template <>
        STROMX_RUNTIME_API void UInt32::serialize(OutputProvider & out) const;
        template <>
        STROMX_RUNTIME_API void UInt32::deserialize(InputProvider & in, const Version & version);
        
        /** \cond */
        template <>
        class data_traits<UInt32>
        {
        public:
            static const DataVariant & variant() { return DataVariant::UINT_32; }
        };  
        /** \endcond */
        
        /** \brief Single precision floating point number */
        typedef Primitive<double, float> Float;
        template<>
        STROMX_RUNTIME_API const DataVariant & Float::variant() const;
        template <>
        STROMX_RUNTIME_API void Float::serialize(OutputProvider & out) const;
        template <>
        STROMX_RUNTIME_API void Float::deserialize(InputProvider & in, const Version & version);
        
        /** \cond */
        template <>
        class data_traits<Float>
        {
        public:
            static const DataVariant & variant() { return DataVariant::FLOAT; }
        };  
        /** \endcond */

        /** \brief Double precision floating point number */
        typedef Primitive<double, double> Double;
        template<>
        STROMX_RUNTIME_API const DataVariant & Double::variant() const;
        template <>
        STROMX_RUNTIME_API void Double::serialize(OutputProvider & out) const;
        template <>
        STROMX_RUNTIME_API void Double::deserialize(InputProvider & in, const Version & version);
        
        /** \cond */
        template <>
        class data_traits<Double>
        {
        public:
            static const DataVariant & variant() { return DataVariant::DOUBLE; }
        };  
        /** \endcond */

#ifdef WIN32
        /** \cond */
        template class STROMX_RUNTIME_API Primitive<bool, bool>;
        template class STROMX_RUNTIME_API Primitive<int, int8_t>;    
        template class STROMX_RUNTIME_API Primitive<unsigned int, uint8_t>; 
        template class STROMX_RUNTIME_API Primitive<int, int16_t>;   
        template class STROMX_RUNTIME_API Primitive<unsigned int, uint16_t>;   
        template class STROMX_RUNTIME_API Primitive<int, int32_t>;   
        template class STROMX_RUNTIME_API Primitive<unsigned int, uint32_t>;   
        template class STROMX_RUNTIME_API Primitive<double, float>;   
        template class STROMX_RUNTIME_API Primitive<double, double>;   
        /** \endcond */
#endif // WIN32
    }
}
    
#endif // STROMX_RUNTIME_PRIMITIVE_H