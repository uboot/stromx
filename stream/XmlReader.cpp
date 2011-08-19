#include "XmlReader.h"

#include "Exception.h"
#include "Factory.h"
#include "Data.h"

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
    
    Stream*const XmlReader::read(const std::string & filename)
    {        
        boost::filesystem::path filepath(filename);
        boost::filesystem::path separator("/");
        
        m_currentPath = filepath.parent_path().directory_string();
        
        std::cout << m_currentPath << std::endl;
        
        try
        {
            XMLPlatformUtils::Initialize();  // Initialize Xerces infrastructure
        }
        catch (const XMLException& toCatch)
        {
            char* message = XMLString::transcode(toCatch.getMessage());
            std::cerr << "Error during initialization! :\n"
                 << message << "\n";
            XMLString::release(&message);
            return 0;
        }
    
        XercesDOMParser* parser = new XercesDOMParser();
        parser->setValidationScheme(XercesDOMParser::Val_Always);
        parser->setDoNamespaces(true);    // optional

        ErrorHandler* errHandler = (ErrorHandler*) new HandlerBase();
        parser->setErrorHandler(errHandler);

        const char* xmlFile = filename.c_str();

        try {
            parser->parse(xmlFile);
        }
        catch (const XMLException& toCatch) {
            char* message = XMLString::transcode(toCatch.getMessage());
            std::cerr << "Exception message is: \n"
                << message << "\n";
            XMLString::release(&message);
            return 0;
        }
        catch (const DOMException& toCatch) {
            char* message = XMLString::transcode(toCatch.msg);
            std::cerr << "Exception message is: \n"
                << message << "\n";
            XMLString::release(&message);
            return 0;
        }
        catch (...) {
            std::cerr << "Unexpected Exception \n" ;
            return 0;
        }
        
        try
        {
            char tempStr[100];
            XMLCh tempXmlStr[100];
            
            DOMDocument* doc = parser->getDocument();
            
            DOMElement* stream = doc->getDocumentElement();
            
            XMLString::transcode("name", tempXmlStr, 99);
            const XMLCh* name = stream->getAttribute(tempXmlStr);
            XMLString::transcode(name, tempStr, 99);
            std::cout << tempStr << std::endl;
            
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
            }
                
        }
        catch(XMLException&)
        {
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
    }  
           
    const XmlReader::OperatorPair XmlReader::readOperator(xercesc_3_0::DOMElement* const opElement)
    {
    }
    
    const XmlReader::ParameterPair XmlReader::readParameter(xercesc_3_0::DOMElement* const paramElement)
    {
        const XMLCh* element = 0;
        const XMLCh* text = 0;
        ParameterPair pair;
        
        XMLString::transcode("id", m_tempXmlStr, BUFFER_LENGTH - 1);
        element = paramElement->getAttribute(m_tempXmlStr);
        XMLString::transcode(element, m_tempStr, BUFFER_LENGTH - 1);
        pair.id = boost::lexical_cast<unsigned int>(m_tempStr);
        
        XMLString::transcode("Data", m_tempXmlStr, 99);
        DOMNodeList* dataList = paramElement->getElementsByTagName(m_tempXmlStr);
        
        if(dataList->getLength() > 0)
        {
            DOMElement* data = dynamic_cast<DOMElement*>(dataList->item(0));
            std::string package, type, dataString;
            
            XMLString::transcode("type", m_tempXmlStr, BUFFER_LENGTH - 1);
            element = paramElement->getAttribute(m_tempXmlStr);
            XMLString::transcode(element, m_tempStr, BUFFER_LENGTH - 1);
            type = m_tempStr;
            
            XMLString::transcode("package", m_tempXmlStr, BUFFER_LENGTH - 1);
            element = paramElement->getAttribute(m_tempXmlStr);
            XMLString::transcode(element, m_tempStr, BUFFER_LENGTH - 1);
            package = m_tempStr;
            
            text = data->getNodeValue();
            XMLString::transcode(text, m_tempStr, BUFFER_LENGTH - 1);
            dataString = m_tempStr;
            
            pair.data = m_factory->newData(type, package);
            pair.data->deserialize(dataString, m_currentPath);
        }
    }
    
    void XmlReader::readThread(xercesc_3_0::DOMElement* const threadElement, Thread* const thread)
    {
    }
    
    const XmlReader::InputNode XmlReader::readInputNode(xercesc_3_0::DOMElement* const threadElement)
    {
    }
}
