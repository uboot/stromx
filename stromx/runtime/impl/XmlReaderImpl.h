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

#ifndef STROMX_RUNTIME_IMPL_XMLREADERIMPL_H
#define STROMX_RUNTIME_IMPL_XMLREADERIMPL_H

#include <xercesc/dom/DOM.hpp>
#include <xercesc/util/XercesDefs.hpp>
#include <map>
#include <string>
#include <vector>
#include "stromx/runtime/Version.h"

namespace stromx
{
        namespace runtime
    {
        class AbstractFactory;
        class Data;
        class FileInput;
        class Stream;
        class Operator;
        class Thread;
        
        namespace impl
        {
            class XmlReaderImpl
            {
        public:
                explicit XmlReaderImpl(const AbstractFactory* factory);
                
                ~XmlReaderImpl();
                
                Stream* readStream(FileInput & input, const std::string & filename);
                
                void readParameters(FileInput & input, const std::string & filename, 
                                    const std::vector<stromx::runtime::Operator*> & operators);
                
            private:
                void readOperator(xercesc::DOMElement* const opElement);
                void readOperatorInputs(xercesc::DOMElement* const opElement);
                void readParameter(xercesc::DOMElement* const paramElement);
                void readThread(xercesc::DOMElement* const threadElement, Thread* const thread);
                void readInputConnector(xercesc::DOMElement* const inputNodeElement, Thread* const thread);
                void readInput(xercesc::DOMElement* const inputElement, Operator* const op);
                Data* readData(xercesc::DOMElement* const dataElement);
                
                const AbstractFactory* m_factory;
                Stream* m_stream;
                FileInput* m_input;
                std::map<unsigned int, Operator*> m_id2OperatorMap;
                std::map<unsigned int, Data*> m_id2DataMap;
            };
        }
    }
}

#endif // STROMX_RUNTIME_IMPL_XMLREADERIMPL_H
