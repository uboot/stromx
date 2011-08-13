#include "XmlReader.h"

#include "Exception.h"

#include <xercesc/parsers/XercesDOMParser.hpp>
#include <xercesc/dom/DOM.hpp>
#include <xercesc/sax/HandlerBase.hpp>
#include <xercesc/util/XMLString.hpp>
#include <xercesc/util/PlatformUtils.hpp>

#include <iostream>

namespace stream
{
    using namespace xercesc;
    
    Stream*const XmlReader::read(const std::string & filename, const OperatorFactory* const factory)
    {        
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
}
