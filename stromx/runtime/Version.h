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

#ifndef STROMX_RUNTIME_VERSION_H
#define STROMX_RUNTIME_VERSION_H

#include "stromx/runtime/Config.h"

#include <iostream>

#undef major
#undef minor

namespace stromx
{
    namespace runtime
    {
       class Version;
    }
}

namespace boost
{
    namespace serialization
    {
        template<class Archive>
        void serialize(Archive & ar, stromx::runtime::Version & v, const unsigned int classVersion);
    }
}

namespace stromx
{
    namespace runtime
    {
        /** \brief A version of an operator or data type. */
        class Version
        {
            friend STROMX_RUNTIME_API bool operator== (const Version & lhs, const Version & rhs);
            friend STROMX_RUNTIME_API bool operator< (const Version & lhs, const Version & rhs);
            friend STROMX_RUNTIME_API std::ostream& operator<< (std::ostream& out, const Version & version);
            friend STROMX_RUNTIME_API std::istream& operator>> (std::istream& in, Version & version);
            
            template<class Archive>
            friend void boost::serialization::serialize(Archive & ar, stromx::runtime::Version & v, const unsigned int classVersion);
            
        public:
            /** Constructs a version object 0.0.0. */
            Version()
              : m_major(0),
                m_minor(0),
                m_revision(0)
            {}
            
            /** Constructs a version object. */
            Version(const unsigned int major, const unsigned int minor, const unsigned int revision)
              : m_major(major),
                m_minor(minor),
                m_revision(revision)
            {}
            
            /** Returns the major version number. */
            unsigned int major() const { return m_major; }
            
            /** Returns the minor version number. */
            unsigned int minor() const { return m_minor; }
            
            /** Returns the revision number. */
            unsigned int revision() const { return m_revision; }
            
        private:
            unsigned int m_major;
            unsigned int m_minor;
            unsigned int m_revision;
        };
        
        STROMX_RUNTIME_API bool operator== (const Version & lhs, const Version & rhs);
        STROMX_RUNTIME_API bool operator!= (const Version & lhs, const Version & rhs);
        STROMX_RUNTIME_API std::ostream& operator<< (std::ostream& out, const Version & version);
        STROMX_RUNTIME_API std::istream& operator>> (std::istream& in, Version & version);
        STROMX_RUNTIME_API bool operator< (const Version & lhs, const Version & rhs);
        STROMX_RUNTIME_API bool operator> (const Version & lhs, const Version & rhs);
        STROMX_RUNTIME_API bool operator>= (const Version & lhs, const Version & rhs);
        STROMX_RUNTIME_API bool operator<= (const Version & lhs, const Version & rhs);
    }
}



#endif // STROMX_RUNTIME_VERSION_H
