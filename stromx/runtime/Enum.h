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

#ifndef STROMX_RUNTIME_ENUM_H
#define STROMX_RUNTIME_ENUM_H

#include "stromx/runtime/Primitive.h"

namespace stromx
{
    namespace runtime
    {
        /** \brief Value of an enumeration. */
        class STROMX_RUNTIME_API Enum : public UInt32
        {
        public:
            /** Constructs an enumeration value and initializes it to 0. */
            Enum() : UInt32() {}
            
            /** Constructs an enumeration value and initializes it to \c value. */
            explicit Enum(const unsigned int value) : UInt32(value) {}
            
            /** Assigns \c value to the enum. */
            const Enum operator=(const unsigned int value)
            { 
                *this = Enum(value);
                return *this;
            }
            
            virtual const VariantHandle & variant() const { return Variant::ENUM; }
            virtual const std::string & type() const { return TYPE; }
            virtual Data* clone() const { return new Enum(*this); }
            
        private:
            static const std::string TYPE;
        };
        
        /** \cond */
        template <>
        class data_traits<Enum>
        {
        public:
            static const VariantHandle & variant() { return Variant::ENUM; }
        };
        /** \endcond */
    }
}

#endif // STROMX_RUNTIME_ENUM_H
