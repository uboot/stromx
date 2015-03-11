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

#ifndef STROMX_RUNTIME_STRING_H
#define STROMX_RUNTIME_STRING_H

#include <string>
#include "stromx/runtime/Data.h"

namespace stromx
{
    namespace runtime
    {
        /** \brief %String data value. */
        class STROMX_RUNTIME_API String : public Data
        {
            friend STROMX_RUNTIME_API bool operator==(const String & lhs, const String & rhs);
            friend STROMX_RUNTIME_API bool operator!=(const String & lhs, const String & rhs);
            friend STROMX_RUNTIME_API std::ostream& operator<<(std::ostream& out, const String& string);
            
        public:
            /** Constructs an empty string. */
            explicit String() {}
            
            /** Constructs a string and initializes it with \c value. */
            explicit String(const std::string & value) : m_value(value) {}
            
            /** Constructs a string and initializes it with \c value. */
            explicit String(const char * value) : m_value(value) {}
            
            virtual const std::string & type() const { return TYPE; }
            virtual const Version & version() const { return VERSION; }
            virtual const std::string & package() const { return PACKAGE; }
            
            virtual const VariantHandle & variant() const;
            
            virtual Data* clone() const { return new String(m_value); }
            
            virtual void serialize(OutputProvider & out) const;
            
            virtual void deserialize(InputProvider & in, const Version & version);
            
            /** Returns the value of the string. */
            const std::string & get() const {  return m_value; }
            
            /** Returns the value of the string. */
            operator std::string() const { return get(); }
            
        private:
            static const std::string TYPE;
            static const std::string PACKAGE;
            static const Version VERSION;
            
            std::string m_value;
        };
        
        /** Returns true if the contents of \c lhs and \c rhs are the same. */
        STROMX_RUNTIME_API bool operator==(const String & lhs, const String & rhs);
        
        /** Returns true if the contents of \c lhs and \c rhs are different. */
        STROMX_RUNTIME_API bool operator!=(const String & lhs, const String & rhs);   
        
        STROMX_RUNTIME_API std::ostream& operator<<(std::ostream& out, const String& string);    
        
        /** \cond */
        template <>
        class data_traits<String>
        {
        public:
            static const VariantHandle & variant() { return Variant::STRING; }
        };  
        /** \endcond */
    }
}


    
#endif // STROMX_RUNTIME_STRING_H
