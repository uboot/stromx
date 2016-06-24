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

#ifndef STROMX_RUNTIME_IMPL_XMLWRITERIMPL_H
#define STROMX_RUNTIME_IMPL_XMLWRITERIMPL_H

#include <xercesc/dom/DOM.hpp>
#include <string>
#include "stromx/runtime/Operator.h"
#include "stromx/runtime/Parameter.h"
#include "stromx/runtime/Stream.h"

namespace stromx
{
    namespace runtime
    {
        class FileOutput;
        class Stream;
        
        namespace impl
        {
            class XmlWriterImpl
            {
        public:
                XmlWriterImpl();
                ~XmlWriterImpl() {};
                void writeStream(FileOutput & output, const std::string & basename, const Stream& stream);
                void writeParameters(FileOutput& output, 
                                     const std::string& basename, 
                                     const std::vector< const stromx::runtime::Operator* >& operators);
                
            private:
                void createDoc();
                void createComm();
                void createStromx();
                unsigned int translateOperatorPointerToID(const Operator* const op) const;
                void createThreads(const std::vector<Thread*> & threads);
                void createInputConnectors(const Thread* const currThr, xercesc::DOMElement* const thrElement);
                void createOperators(const std::vector<const Operator*> & operators, xercesc::DOMElement* const parentElement);
                void createParameters(const Operator* const currOp, xercesc::DOMElement* const opElement);
                void createData(const Parameter* const currPar, const Operator* const currOp, xercesc::DOMElement* const parElement);
                void createInputs(const Operator* const currOp, xercesc::DOMElement* const opElement);
                
                static const unsigned int XML_FORMAT_VERSION_MAJOR;
                static const unsigned int XML_FORMAT_VERSION_MINOR;
                static const unsigned int XML_FORMAT_VERSION_REVISION;
                
                const Stream* m_stream;
                std::vector<const Operator*> m_opList;
                FileOutput* m_output;
                std::string m_filename;
                xercesc::DOMImplementation* m_impl;
                xercesc::DOMDocument* m_doc;
                xercesc::DOMElement* m_stromxElement;
                xercesc::DOMElement* m_strElement;
                xercesc::DOMElement* m_parsElement;
            };
        }
    }
}

#endif // STROMX_RUNTIME_IMPL_XMLWRITERIMPL_H
