/* 
 *  Copyright 2013 Matthias Fuchs
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

#include "stromx/runtime/Version.h"

#include <boost/lexical_cast.hpp>
#include <boost/graph/graph_concepts.hpp>

#include "stromx/runtime/Exception.h"

namespace stromx
{
    namespace runtime
    {
        std::ostream& operator<<(std::ostream& out, const Version& version)
        {
            out << version.m_major << "." << version.m_minor << "." << version.m_revision;
            return out;
        }

        std::istream& operator>>(std::istream& in, Version& version)
        {
            std::string text;
            in >> text;
            
            std::size_t first = text.find_first_of(".");
            std::size_t last = text.find_last_of(".");
            
            if(first == std::string::npos)
                throw WrongArgument("Input must be of the type '[x].[y].[z]'.");
            
            if(last == std::string::npos || last == first)
                throw WrongArgument("Input must be of the type '[x].[y].[z]'.");
            
            unsigned int major = 0;
            unsigned int minor = 0;
            unsigned int revision = 0;
            
            try
            {
                major = boost::lexical_cast<unsigned int>(std::string(text, 0, first));
                minor = boost::lexical_cast<unsigned int>(std::string(text, first + 1, last - first - 1));
                revision = boost::lexical_cast<unsigned int>(std::string(text, last + 1));
                
                version.m_major = major;
                version.m_minor = minor;
                version.m_revision = revision;
            }
            catch(boost::bad_lexical_cast&)
            {
                throw WrongArgument("Input must be of the type '[x].[y].[z]'.");
            }
            
            return in;
           
        }
        
        bool operator==(const Version& lhs, const Version& rhs)
        {
            return lhs.m_major == rhs.m_major && lhs.m_minor == rhs.m_minor && lhs.m_revision == rhs.m_revision;
        }
        
        bool operator!=(const Version& lhs, const Version& rhs)
        {
            return ! (lhs == rhs);
        }
        
        bool operator<(const Version& lhs, const Version& rhs)
        {
            if (lhs.m_major == rhs.m_major)
            {
                if (lhs.m_minor == rhs.m_minor)
                {
                    return lhs.m_revision < rhs.m_revision;
                }
                else
                {
                    return lhs.m_minor < rhs.m_minor;
                }
            }
            else
            {
                return lhs.m_major < rhs.m_major;
            }
        }
        
        bool operator<=(const Version& lhs, const Version& rhs)
        {
            return (lhs < rhs) || (lhs == rhs);
        }
        
        bool operator>=(const Version& lhs, const Version& rhs)
        {
            return rhs <= lhs;
        }
        
        
        bool operator>(const Version& lhs, const Version& rhs)
        {
            return rhs < lhs;
        }
    }
}


