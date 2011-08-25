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

#include "XmlUtilities.h"

    
namespace stream
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
    }
}