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
            XMLString::transcode("Range", tempStr, 99);
            DOMImplementation* impl = DOMImplementationRegistry::getDOMImplementation(tempStr);

            XMLString::transcode("root", tempStr, 99);
            DOMDocument*   doc = impl->createDocument(0, tempStr, 0);
            DOMElement*   root = doc->getDocumentElement();

            XMLString::transcode("FirstElement", tempStr, 99);
            DOMElement*   e1 = doc->createElement(tempStr);
            root->appendChild(e1);

            XMLString::transcode("SecondElement", tempStr, 99);
            DOMElement*   e2 = doc->createElement(tempStr);
            root->appendChild(e2);

            XMLString::transcode("aTextNode", tempStr, 99);
            DOMText*       textNode = doc->createTextNode(tempStr);
            e1->appendChild(textNode);

            // optionally, call release() to release the resource associated with the range after done
            DOMRange* range = doc->createRange();
            range->release();

            // no need to release this returned object which is owned by implementation
            XMLString::transcode("*", tempStr, 99);
            DOMNodeList*    nodeList = doc->getElementsByTagName(tempStr);
            
            DOMLSSerializer* serializer = impl->createLSSerializer();
            char* content = XMLString::transcode(serializer->writeToString(doc));
            // std::cout << content;
            XMLString::release(&content);
            serializer->release();

            // done with the document, must call release() to release the entire document resources
            doc->release();
   
            XMLPlatformUtils::Terminate();  // Terminate after release of memory
        }
        catch(XMLException&)
        {
            throw InternalError("Error in Xerces-C.");
        }
    }
} 
