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

#include <xercesc/dom/DOM.hpp>
#include <string>
#include "../Node.h"
#include "../Operator.h"
#include "../Parameter.h"
#include "../Stream.h"
#include "XmlUtilities.h"
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
            void createThreads(const std::vector<Thread*> thr);
            void createInputNodes(const std::vector<Node> inNodes, xercesc::DOMElement* const thr);
            void createOperators(const std::vector<Operator*> operators);
            void createParameters(const std::vector<const Parameter*> parameters, const Operator* const currOp, xercesc::DOMElement* const op);
            void createData(const Parameter* const parameter, const Operator* const currOp, xercesc::DOMElement* const param);
            
            Stream* m_stream;
            std::string m_filename;
            xercesc::DOMImplementation* m_impl;
            xercesc::DOMDocument* m_doc;
            xercesc::DOMElement* m_Stream;
        };
    }
}

#endif // STREAM_XMLWRITERIMPL_H
