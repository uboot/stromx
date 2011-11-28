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

#ifndef STROMX_CORE_IMPL_XMLREADERIMPL_H
#define STROMX_CORE_IMPL_XMLREADERIMPL_H

#include <string>
#include <map>

#include <xercesc/util/XercesDefs.hpp>
#include <xercesc/dom/DOM.hpp>

namespace stromx
{
        namespace core
    {
        class Factory;
        class Stream;
        class Data;
        class Operator;
        class Thread;
        class Data;
        
        namespace impl
        {
            class XmlReaderImpl
            {
            public:
                XmlReaderImpl(const Factory& factory);
                
                ~XmlReaderImpl();
                
                Stream* const read(const std::string & filename);
                
            private:
                void readOperator(xercesc::DOMElement* const opElement);
                void readOperatorInputs(xercesc::DOMElement* const opElement);
                void readParameter(xercesc::DOMElement* const paramElement);
                void readThread(xercesc::DOMElement* const threadElement, Thread* const thread);
                void readInputNode(xercesc::DOMElement* const inputNodeElement, Thread* const thread);
                void readInput(xercesc::DOMElement* const inputElement, Operator* const op);
                Data* readData(xercesc::DOMElement* const dataElement);
                
                std::string m_currentPath;
                const Factory& m_factory;
                Stream* m_stream;
                std::map<unsigned int, Operator*> m_id2OperatorMap;
                std::map<unsigned int, Data*> m_id2DataMap;
            };
        }
    }
}

#endif // STROMX_CORE_IMPL_XMLREADERIMPL_H
