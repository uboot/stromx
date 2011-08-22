/* 
*  Copyright 2011 XXX
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

#ifndef STREAM_XMLREADER_H
#define STREAM_XMLREADER_H

#include <string>
#include <map>

#include <xercesc/util/XercesDefs.hpp>

namespace xercesc_3_0
{
    class DOMElement;
}

namespace stream
{
    class Factory;
    class Stream;
    class Data;
    class Operator;
    class Thread;
    class Data;
    
    class XmlReader
    {
        friend class XmlReaderTest;
        
    public:
        XmlReader(const Factory* const factory)
          : m_factory(factory), m_stream(0) {}
          
        ~XmlReader();
          
        Stream* const read(const std::string & filename);
        
    private:
        static const std::string computePath(const std::string & filename);
        
        std::string m_currentPath;
        
        void readOperator(xercesc_3_0::DOMElement* const opElement);
        void readParameter(xercesc_3_0::DOMElement* const paramElement);
        void readThread(xercesc_3_0::DOMElement* const threadElement, Thread* const thread);
        void readInputNode(xercesc_3_0::DOMElement* const threadElement, Thread* const thread);
        Data* readData(xercesc_3_0::DOMElement* const dataElement);
        void cleanUp();
        
        const Factory* m_factory;
        Stream* m_stream;
        std::map<unsigned int, Operator*> m_id2OperatorMap;
        std::map<unsigned int, Data*> m_id2DataMap;
    };
}

#endif // STREAM_XMLREADER_H
