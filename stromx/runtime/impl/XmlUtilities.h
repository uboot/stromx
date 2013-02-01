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

#ifndef STROMX_RUNTIME_IMPL_XMLUTILITES_H
#define STROMX_RUNTIME_IMPL_XMLUTILITES_H

#include <xercesc/util/XMLString.hpp>
#include "stromx/runtime/Exception.h"

namespace stromx
{
    namespace runtime
    {
        class Version; 
        
        namespace impl
        {
            class XmlError : public Exception
            {
            public:
                XmlError(const std::string & message = "XmlError")
                : Exception(message)
                {}
            };
        
            class Str2Xml
            {
            public:
                explicit Str2Xml(const char* const str);
                ~Str2Xml();
                
                operator const XMLCh*();
                
            private:
                XMLCh* m_xmlStr;
            };
            
            class Xml2Str
            {
            public:
                explicit Xml2Str(const XMLCh* const str);
                ~Xml2Str();
                
                operator const char*();
                
            private:
                char* m_str;
            };
            
            class XmlUtilities
            {
            public:
                static const std::string computePath(const std::string & filepath);
                static const std::string computeName(const std::string & filepath);
                static const std::string stripExtension(const std::string & filename);
                static const std::string computeExtension(const std::string & filename);
                static Version convertToVersion(const std::string & text);
            };
        }
    }
}

#endif // STROMX_RUNTIME_IMPL_XMLUTILITES_H
