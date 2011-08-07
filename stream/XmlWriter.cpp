#include "XmlWriter.h"

#include "Exception.h"

#include <xercesc/dom/DOM.hpp>
#include <iostream>

using namespace xercesc;

namespace stream
{
    void XmlWriter::write(const std::string& filename, Stream& stream)
    {
        try
        {
            XMLPlatformUtils::Initialize();  // Initialize Xerces infrastructure
            
            XMLCh tempStr[100];
            XMLString::transcode("", tempStr, 99);
            DOMImplementation* impl = DOMImplementationRegistry::getDOMImplementation(tempStr);

            XMLString::transcode("Stream", tempStr, 99);
            DOMDocument* doc = impl->createDocument(0, tempStr, 0);
            DOMElement* stream = doc->getDocumentElement();

            XMLString::transcode("Operator", tempStr, 99);
            DOMElement* op = doc->createElement(tempStr);
            stream->appendChild(op);
            
            XMLString::transcode("id", tempStr, 99);
            DOMAttr* id = doc->createAttribute(tempStr);
            XMLString::transcode("0", tempStr, 99);
            id->setValue(tempStr);
            op->setAttributeNode(id);
            
            XMLString::transcode("type", tempStr, 99);
            DOMAttr* type = doc->createAttribute(tempStr);
            XMLString::transcode("TestOperator", tempStr, 99);
            type->setValue(tempStr);
            op->setAttributeNode(type);

            XMLString::transcode("Parameter", tempStr, 99);
            DOMElement* param = doc->createElement(tempStr);
            op->appendChild(param);

            XMLString::transcode("UInt32", tempStr, 99);
            DOMElement* dataType = doc->createElement(tempStr);
            param->appendChild(dataType);
            
            XMLString::transcode("5000", tempStr, 99);
            DOMText* value = doc->createTextNode(tempStr);
            dataType->appendChild(value);
            
            DOMLSSerializer* serializer = impl->createLSSerializer();
            serializer->getDomConfig()->setParameter(XMLUni::fgDOMWRTFormatPrettyPrint, true);
            char* content = XMLString::transcode(serializer->writeToString(doc));
            std::cout << content << std::endl;
            XMLString::release(&content);
            serializer->release();

            // done with the document, must call release() to release the entire document resources
            doc->release();
   
            XMLPlatformUtils::Terminate();  // Terminate after release of memory
        }
        catch(DOMException& e)
        {
            throw InternalError("Error in Xerces-C.");
        }
        catch(XMLException& e)
        {
            throw InternalError("Error in Xerces-C.");
        }
    }
} 
