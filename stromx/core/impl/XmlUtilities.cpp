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

#include <boost/filesystem.hpp>
#include <boost/lexical_cast.hpp>
#include "XmlUtilities.h"
#include "../Version.h"

namespace stromx
{
    namespace core
    {
        namespace impl
        {
            using namespace xercesc;
            
            Xml2Str::Xml2Str(const XMLCh* const str)
            {
                m_str = XMLString::transcode(str);
            }

            Xml2Str::~Xml2Str()
            {
                XMLString::release(&m_str);
            }

            Xml2Str::operator const char*()
            {
                return m_str;
            }
            
            Str2Xml::Str2Xml(const char* const str)
            {
                m_xmlStr = XMLString::transcode(str);
            }

            Str2Xml::~Str2Xml()
            {
                XMLString::release(&m_xmlStr);
            }

            Str2Xml::operator const XMLCh*()
            {
                return m_xmlStr;
            }
            
            const std::string XmlUtilities::computePath(const std::string& filepath)
            {
                std::string path = boost::filesystem::path(filepath).parent_path().string();
                
                return path.empty() ? "." : path;
            }
            
            const std::string XmlUtilities::computeName(const std::string& filepath)
            {
                boost::filesystem::path path = boost::filesystem::path(filepath).filename();
                return path.string();
            }
            
            const std::string XmlUtilities::stripExtension(const std::string& filename)
            {
                std::size_t i = filename.find_first_of(".");
                
                if(i == std::string::npos)
                    return filename;
                
                return std::string (filename, 0, i);
            }
            
            const std::string XmlUtilities::computeExtension(const std::string& filename)
            {
                std::size_t i = filename.find_first_of(".");
                
                if(i == std::string::npos)
                    return "";
                
                return std::string (filename, i + 1);
            }
            
            Version XmlUtilities::convertToVersion(const std::string& text)
            {
                unsigned int first = text.find_first_of(".");
                unsigned int last = text.find_last_of(".");
                
                if(first == std::string::npos)
                    throw WrongArgument("Input must be of the type '[x].[y].[z]'.");
                
                if(last == std::string::npos || last == first)
                    throw WrongArgument("Input must be of the type '[x].[y].[z]'.");
                
                unsigned int major = 0;
                unsigned int minor = 0;
                unsigned int patch = 0;
                
                try
                {
                    major = boost::lexical_cast<unsigned int>(std::string(text, 0, first));
                    minor = boost::lexical_cast<unsigned int>(std::string(text, first + 1, last - first - 1));
                    patch = boost::lexical_cast<unsigned int>(std::string(text, last + 1));
                }
                catch(boost::bad_lexical_cast&)
                {
                    throw WrongArgument("Input must be of the type '[x].[y].[z]'.");
                }
                
                return Version(major, minor, patch);
            }
        }
    }
}
