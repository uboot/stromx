#include "XmlReader.h"

#include "Exception.h"
#include "Factory.h"
#include "Data.h"
#include "Stream.h"
#include "Operator.h"

#include "impl/XmlUtilities.h"

#include <xercesc/parsers/XercesDOMParser.hpp>
#include <xercesc/dom/DOM.hpp>
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
            
            for(unsigned int i = 0; i < numOperators; ++i)
            {
                DOMElement* op = dynamic_cast<DOMElement*>(operators->item(i));
                readOperator(op);
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
        
        for(unsigned int i = 0; i < numParameters; ++i)
        {
            DOMElement* paramElement = dynamic_cast<DOMElement*>(parameters->item(i));
            readParameter(paramElement, op);
        }
    }
    
    void XmlReader::readParameter(DOMElement*const paramElement, Operator* op)
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
        
        op->setParameter(id, *data);
        
        delete data;
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
        
        if(! parentpath.empty())
            pathSeparator = boost::filesystem::path("/").file_string();
    
        return parentpath.file_string() + pathSeparator;
    }
    
    void XmlReader::cleanUp()
    {
        m_stream = 0;
        m_currentPath = "";
        m_id2OperatorMap.clear();
    }
}
