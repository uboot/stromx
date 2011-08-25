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

#include "XmlReader.h"

#include "Exception.h"
#include "Factory.h"
#include "Data.h"
#include "Stream.h"
#include "Operator.h"
#include "Thread.h"

#include "impl/XmlUtilities.h"

#include <xercesc/parsers/XercesDOMParser.hpp>
#include <xercesc/sax/HandlerBase.hpp>
#include <xercesc/util/XMLString.hpp>
#include <xercesc/util/PlatformUtils.hpp>

#include <iostream>
#include <boost/lexical_cast.hpp>
#include <boost/filesystem.hpp>

namespace stream
{
    using namespace xercesc;
    using namespace impl;

    Stream*const XmlReader::read(const std::string & filename)
    {    
        cleanUp();
        
        m_currentPath = computePath(filename);
        
        try
        {
            XMLPlatformUtils::Initialize();  // Initialize Xerces infrastructure
        }
        catch (const XMLException& toCatch)
        {
            char* message = XMLString::transcode(toCatch.getMessage());
            
            FileAccessFailed ex(filename, "Failed to initialize xerces-c: " + std::string(message));
            XMLString::release(&message);
            throw ex;
        }
    
        XercesDOMParser* parser = new XercesDOMParser();
        parser->setValidationScheme(XercesDOMParser::Val_Always);
        parser->setDoNamespaces(true);    // optional

        ErrorHandler* errHandler = (ErrorHandler*) new HandlerBase();
        parser->setErrorHandler(errHandler);

        const char* xmlFile = filename.c_str();

        try
        {
            parser->parse(xmlFile);
        }
        catch (const XMLException& toCatch)
        {
            char* message = XMLString::transcode(toCatch.getMessage());
            
            FileAccessFailed ex(filename, "XML exception: " + std::string(message));
            XMLString::release(&message);
            throw ex;
        }
        catch (const DOMException& toCatch)
        {
            char* message = XMLString::transcode(toCatch.msg);
            
            FileAccessFailed ex(filename, "DOM exception: " + std::string(message));
            XMLString::release(&message);
            throw ex;
        }
        catch (...)
        {
            throw FileAccessFailed(filename, "Unexpected exception.");
        }
        
        try
        {
            m_stream = new Stream();
            
            DOMDocument* doc = parser->getDocument();
            DOMElement* stream = doc->getDocumentElement();
            
            Xml2Str name(stream->getAttribute(Str2Xml("name")));
            m_stream->setName(std::string(name));
            
            DOMNodeList* operators = stream->getElementsByTagName(Str2Xml("Operator"));
            XMLSize_t numOperators = operators->getLength();
            
            // read the operators
            for(unsigned int i = 0; i < numOperators; ++i)
            {
                DOMElement* op = dynamic_cast<DOMElement*>(operators->item(i));
                readOperator(op);
            }
            
            // read the inputs of each operator
            for(unsigned int i = 0; i < numOperators; ++i)
            {
                DOMElement* op = dynamic_cast<DOMElement*>(operators->item(i));
                readOperatorInputs(op);
            }
            
            DOMNodeList* threads = stream->getElementsByTagName(Str2Xml("Thread"));
            XMLSize_t numThreads = threads->getLength();
            
            for(unsigned int i = 0; i < numThreads; ++i)
            {
                DOMElement* threadElement = dynamic_cast<DOMElement*>(threads->item(i));
                Thread* thread = m_stream->addThread();
                readThread(threadElement, thread);
            }
        }
        catch(xercesc::XMLException&)
        {
            delete m_stream;
            throw FileAccessFailed(filename, "Failed to read XML file.");
        }
        catch(stream::Exception& e)
        {
            delete m_stream;
            throw FileAccessFailed(filename, e.what());
        }
        catch(boost::bad_lexical_cast e)
        {
            delete m_stream;
            throw FileAccessFailed(filename, e.what());
        }

        delete parser;
        delete errHandler;
            
        try
        {
            XMLPlatformUtils::Terminate();  // Terminate after release of memory
        }
        catch(XMLException&)
        {
        }
        
        Stream* retValue = m_stream;
        
        cleanUp();
        
        return retValue;
    }  
    
    XmlReader::~XmlReader()
    {
        cleanUp();
    }
    
    void XmlReader::readOperator(DOMElement*const opElement)
    {
        Xml2Str name(opElement->getAttribute(Str2Xml("name")));
        Xml2Str idStr(opElement->getAttribute(Str2Xml("id")));
        Xml2Str type(opElement->getAttribute(Str2Xml("type")));
        Xml2Str package(opElement->getAttribute(Str2Xml("package")));
        
        unsigned int id = boost::lexical_cast<unsigned int>((const char*)(idStr));
        
        if(m_id2OperatorMap.count(id))
            throw XmlError("Multiple operators with the same ID.");
        
        Operator* op = m_factory->newOperator(std::string(package), std::string(type));
        op->setName(std::string(name));
        
        m_id2OperatorMap[id] = op;
        
        DOMNodeList* parameters = opElement->getElementsByTagName(Str2Xml("Parameter"));
        XMLSize_t numParameters = parameters->getLength();
        
        // read the parameters
        for(unsigned int i = 0; i < numParameters; ++i)
        {
            DOMElement* paramElement = dynamic_cast<DOMElement*>(parameters->item(i));
            readParameter(paramElement);
        }
    
        // set parameters before initialization
        for(std::vector<const Parameter*>::const_iterator iter = op->info()->parameters().begin();
            iter != op->info()->parameters().end();
            ++iter)
        {
            if((*iter)->accessMode() != Parameter::NONE_WRITE)
                continue;
                
            std::map<unsigned int, Data*>::iterator idDataPair = m_id2DataMap.find((*iter)->id());
            
            if(idDataPair == m_id2DataMap.end())
                continue;
            
            op->setParameter(idDataPair->first, *idDataPair->second);
            
            delete idDataPair->second;
            m_id2DataMap.erase(idDataPair);
        }
        
        // initialize
        op->initialize();
    
        // set parameters after initialization
        for(std::vector<const Parameter*>::const_iterator iter = op->info()->parameters().begin();
            iter != op->info()->parameters().end();
            ++iter)
        {
            if((*iter)->accessMode() != Parameter::INITIALIZED_WRITE
                &&  (*iter)->accessMode() != Parameter::ACTIVATED_WRITE)
            {
                continue;
            }
                
            std::map<unsigned int, Data*>::iterator idDataPair = m_id2DataMap.find((*iter)->id());
            
            if(idDataPair == m_id2DataMap.end())
                continue;
            
            op->setParameter(idDataPair->first, *idDataPair->second);
            
            delete idDataPair->second;
            m_id2DataMap.erase(idDataPair);
        }
        
        if(! m_id2DataMap.empty())
            throw XmlError("Not all parameters of operator '" + op->name() + "' could be set.");
        
        m_stream->addOperator(op);
    }
    
    void XmlReader::readOperatorInputs(DOMElement*const opElement)
    {
        Xml2Str idStr(opElement->getAttribute(Str2Xml("id")));
        unsigned int id = boost::lexical_cast<unsigned int>((const char*)(idStr));
        
        BOOST_ASSERT(m_id2OperatorMap.count(id));
        
        DOMNodeList* inputs = opElement->getElementsByTagName(Str2Xml("Input"));
        XMLSize_t numInputs = inputs->getLength();
        
        // read the parameters
        for(unsigned int i = 0; i < numInputs; ++i)
        {
            DOMElement* inputElement = dynamic_cast<DOMElement*>(inputs->item(i));
            readInput(inputElement, m_id2OperatorMap[id]);
        }
    }
    
    void XmlReader::readParameter(DOMElement*const paramElement)
    {
        Xml2Str idStr(paramElement->getAttribute(Str2Xml("id")));
        
        unsigned int id = boost::lexical_cast<unsigned int>((const char*)(idStr));
        
        DOMNodeList* dataElements = paramElement->getElementsByTagName(Str2Xml("Data"));
        XMLSize_t numDataElements = dataElements->getLength();
        
        if(! numDataElements)
            return;
        
        if(numDataElements != 1)
            throw XmlError("More than one <Data/> elements for parameter.");
        
        DOMElement* dataElement = dynamic_cast<DOMElement*>(dataElements->item(0));
        Data* data = readData(dataElement);
        
        if(m_id2DataMap.count(id))
            throw XmlError("Multiple parameters with the same ID " + boost::lexical_cast<std::string>(id) + ".");
        
        m_id2DataMap[id] = data;
    }
    
    Data* XmlReader::readData(DOMElement*const dataElement)
    {
        Xml2Str type(dataElement->getAttribute(Str2Xml("type")));
        Xml2Str package(dataElement->getAttribute(Str2Xml("package")));
        
        DOMNodeList* dataTextElements = dataElement->getChildNodes();
        XMLSize_t numDataTextElements = dataTextElements->getLength();
        std::string dataString;
        
        if(numDataTextElements > 1)
            throw XmlError("More than one children of <Data/>.");
        
        if(numDataTextElements)
        {
            DOMNode* node = dataTextElements->item(0);
            if(node->getNodeType() != DOMNode::TEXT_NODE)
                throw XmlError("Child of <Data/> must be a text node.");
            
            dataString = std::string(Xml2Str(node->getNodeValue()));
        }
        
        Data* data = m_factory->newData(std::string(package), std::string(type));
        
        try
        {
            data->deserialize(dataString, m_currentPath);
        }
        catch(Exception& e)
        {
            delete data;
            throw e;
        }
        
        return data;
    }

    const std::string XmlReader::computePath(const std::string& filename)
    {
        boost::filesystem::path filepath(filename);
        boost::filesystem::path parentpath = filepath.parent_path();
        std::string pathSeparator;
        
        if(! parentpath.empty() && parentpath != boost::filesystem::path("/"))
            pathSeparator = boost::filesystem::path("/").file_string();
    
        return parentpath.file_string() + pathSeparator;
    }
    
    void XmlReader::readThread(DOMElement*const threadElement, Thread*const thread)
    {
        Xml2Str type(threadElement->getAttribute(Str2Xml("name")));
        thread->setName(std::string(type));
        
        DOMNodeList* inputs = threadElement->getElementsByTagName(Str2Xml("InputNode"));
        XMLSize_t numInputs = inputs->getLength();
        
        for(unsigned int i = 0; i < numInputs; ++i)
        {
            DOMElement* inputElement = dynamic_cast<DOMElement*>(inputs->item(i));
            readInputNode(inputElement, thread);
        }
    }
    
    void XmlReader::readInputNode(DOMElement*const inputNodeElement, Thread*const thread)
    {
        Xml2Str opIdStr(inputNodeElement->getAttribute(Str2Xml("operator")));
        Xml2Str inputIdStr(inputNodeElement->getAttribute(Str2Xml("input")));
        
        unsigned int opId = boost::lexical_cast<unsigned int>(std::string(opIdStr));
        unsigned int inputId = boost::lexical_cast<unsigned int>(std::string(inputIdStr));
        
        std::map<unsigned int, Operator*>::iterator idOpPair = m_id2OperatorMap.find(opId);
        
        if(idOpPair == m_id2OperatorMap.end())
            throw XmlError("No operator with ID " + std::string(opIdStr) + ".");
        
        thread->addNode(idOpPair->second, inputId);
    }
    
    void XmlReader::readInput(DOMElement*const inputElement, Operator*const op)
    {
        Xml2Str opIdStr(inputElement->getAttribute(Str2Xml("operator")));
        Xml2Str inputIdStr(inputElement->getAttribute(Str2Xml("id")));
        Xml2Str outputIdStr(inputElement->getAttribute(Str2Xml("output")));
        
        unsigned int opId = boost::lexical_cast<unsigned int>(std::string(opIdStr));
        unsigned int inputId = boost::lexical_cast<unsigned int>(std::string(inputIdStr));
        unsigned int outputId = boost::lexical_cast<unsigned int>(std::string(outputIdStr));
        
        std::map<unsigned int, Operator*>::iterator idOpPair = m_id2OperatorMap.find(opId);
        
        if(idOpPair == m_id2OperatorMap.end())
            throw XmlError("No source operator with ID " + std::string(opIdStr) + ".");
        
        Operator* source = idOpPair->second;
        
        m_stream->connect(op, inputId, source, outputId);
    }
    
    void XmlReader::cleanUp()
    {
        m_stream = 0;
        m_currentPath = "";
        
        m_id2OperatorMap.clear();
        
        for(std::map<unsigned int, Data*>::iterator iter = m_id2DataMap.begin();
            iter != m_id2DataMap.end();
            ++iter)
        {
            delete iter->second;
        }
                
        m_id2DataMap.clear();
    }
}
