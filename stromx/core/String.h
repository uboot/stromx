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

#ifndef STROMX_CORE_STRING_H
#define STROMX_CORE_STRING_H

#include <string>

#include "Data.h"

namespace stromx
{
    namespace core
    {
        /** \brief %String data value. */
        class STROMX_CORE_API String : public Data
        {
            friend STROMX_CORE_API const bool operator==(const String & lhs, const String & rhs);
            friend STROMX_CORE_API const bool operator!=(const String & lhs, const String & rhs);
            
        public:
            static const DataVariant & classVariant();
            
            /** Constructs an empty string. */
            explicit String() {}
            
            /** Constructs a string and initializes it with \c value. */
            explicit String(const std::string & value) : m_value(value) {}
            
            /** Constructs a string and initializes it with \c value. */
            explicit String(const char * value) : m_value(value) {}
            
            virtual const std::string & type() const { return TYPE; }
            virtual const Version & version() const { return VERSION; }
            virtual const std::string & package() const { return PACKAGE; }
            
            virtual const DataVariant & variant() const;
            
            virtual Data* const clone() const { return new String(m_value); }
            
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
        STROMX_CORE_API const bool operator==(const String & lhs, const String & rhs);
        
        /** Returns true if the contents of \c lhs and \c rhs are different. */
        STROMX_CORE_API const bool operator!=(const String & lhs, const String & rhs);
    }
}
    
#endif // STROMX_CORE_STRING_H
