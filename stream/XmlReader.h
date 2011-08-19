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
    
    class XmlReader
    {
    public:
        XmlReader(const Factory* const factory) : m_factory(factory) {}
        Stream* const read(const std::string & filename);
        
    private:
        
        struct ParameterPair
        {
            ParameterPair() : id(0), data(0) {}
            
            unsigned int id;
            Data* data;
        };
        
        struct OperatorPair
        {
            OperatorPair() : id(0), data(0) {}
            
            unsigned int id;
            Operator* data;
        };
        
        struct InputNode
        {
            InputNode() : id(0), operatorId(0), outputId(0) {}
            unsigned int id;
            unsigned int operatorId;
            unsigned int outputId;
        };
        
        static const unsigned int BUFFER_LENGTH = 100;
        
        char m_tempStr[BUFFER_LENGTH];
        XMLCh m_tempXmlStr[BUFFER_LENGTH];
        std::string m_currentPath;
        
        const OperatorPair readOperator(xercesc_3_0::DOMElement* const opElement);
        const ParameterPair readParameter(xercesc_3_0::DOMElement* const paramElement);
        void readThread(xercesc_3_0::DOMElement* const threadElement, Thread* const thread);
        const InputNode readInputNode(xercesc_3_0::DOMElement* const threadElement);
        
        const Factory* m_factory;
    };
}

#endif // STREAM_XMLREADER_H
