/* 
*  Copyright 2015 Matthias Fuchs
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

#ifndef STROMX_RUNTIME_TRIBOOL_H
#define STROMX_RUNTIME_TRIBOOL_H

#ifdef __GNUG__
    #include <tr1/cstdint>
#else
    #include <cstdint>
#endif

#include "stromx/runtime/Config.h"
#include "stromx/runtime/Data.h"

namespace stromx
{
    namespace runtime
    {
        /** \brief %String data value. */
        class STROMX_RUNTIME_API Tribool : public Data
        {
            friend STROMX_RUNTIME_API bool operator==(const Tribool & lhs, const Tribool & rhs);
            friend STROMX_RUNTIME_API bool operator!=(const Tribool & lhs, const Tribool & rhs);
            friend STROMX_RUNTIME_API std::ostream& operator<<(std::ostream& out, const Tribool& value);
            
        public:
            
            /** Constructs an undefined tribool. */
            explicit Tribool() : m_value(-1) {}
            
            /** Constructs a tribool from a Boolean value. */
            explicit Tribool(const bool value) : m_value(value) {}
            
            bool undefined() const { return m_value == UNDEFINED; }
            operator bool() const { return m_value == TRUE; }
            
            virtual const std::string & type() const { return TYPE; }
            virtual const Version & version() const { return VERSION; }
            virtual const std::string & package() const { return PACKAGE; }
            
            virtual const VariantHandle & variant() const;
            
            virtual Data* clone() const { return new Tribool(*this); }
            
            virtual void serialize(OutputProvider & out) const;
            virtual void deserialize(InputProvider & in, const Version & version);
            
        private:
            enum Value
            {
                UNDEFINED = -1,
                TRUE = 1,
                FALSE = 0
            };
            
            static const std::string TYPE;
            static const std::string PACKAGE;
            static const Version VERSION;
            
            int8_t m_value;
        };
        
        /** Returns true if the contents of \c lhs and \c rhs are the same. */
        STROMX_RUNTIME_API bool operator==(const Tribool & lhs, const Tribool & rhs);
        
        /** Returns true if the contents of \c lhs and \c rhs are different. */
        STROMX_RUNTIME_API bool operator!=(const Tribool & lhs, const Tribool & rhs);   
        
        STROMX_RUNTIME_API std::ostream& operator<<(std::ostream& out, const Tribool& string);    
        
        /** \cond */
        template <>
        class data_traits<Tribool>
        {
        public:
            static const VariantHandle & variant();
        };  
        /** \endcond */
    }
}


    
#endif // STROMX_RUNTIME_STRING_H
