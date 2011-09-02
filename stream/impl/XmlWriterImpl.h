/* 
 *  Copyright 2011 Thomas Fidler
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

#ifndef STREAM_XMLWRITERIMPL_H
#define STREAM_XMLWRITERIMPL_H

#include <string>
#include <xercesc/dom/DOM.hpp>
#include "XmlUtilities.h"
#include "../Operator.h"
#include "../Stream.h"

namespace stream
{
    class Stream;
    
    namespace impl
    {
        class XmlWriterImpl
        {
        public:
            XmlWriterImpl();
            ~XmlWriterImpl() {};
            void write(const std::string & filename, Stream& stream);
            
        private:
            const unsigned int translateOperatorPointerToID(const Operator* const op) const;
            void createThreadBranches(const std::vector<Thread*> thr);
            
            Stream* m_stream;
            XMLCh m_tempStr[100];
            xercesc::DOMImplementation* m_impl;
            xercesc::DOMDocument* m_doc;
            xercesc::DOMElement* m_Stream;
        };
    }
}

#endif // STREAM_XMLWRITERIMPL_H
