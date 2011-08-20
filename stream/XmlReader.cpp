#include "XmlReader.h"

#include "Exception.h"
#include "Factory.h"
#include "Data.h"
#include "Stream.h"

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
            
            /*m_xmlStr = XMLString::transcode("name");
            value = stream->getAttribute(m_xmlStr);
            char* m_str = XMLString::transcode(value);
            m_stream->setName(m_str);
            releaseStrings();
            
            XMLString::transcode("Operator", tempXmlStr, 99);
            DOMNodeList* operators = stream->getElementsByTagName(tempXmlStr);
            
            XMLSize_t numOperators = operators->getLength();
            
            for(unsigned int i = 0; i < numOperators; ++i)
            {
                DOMElement* op = dynamic_cast<DOMElement*>(operators->item(i));
                XMLString::transcode("name", tempXmlStr, 99);
                const XMLCh* name = op->getAttribute(tempXmlStr);
                XMLString::transcode(name, tempStr, 99);
                std::cout << tempStr << std::endl;
                
                XMLString::transcode("Parameter", tempXmlStr, 99);
                DOMNodeList* parameters = op->getElementsByTagName(tempXmlStr);
            }*/
                
        }
        catch(XMLException&)
        {
            delete m_stream;
            throw FileAccessFailed(filename, "Failed to read XML file.");
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
