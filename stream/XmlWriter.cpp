#include "XmlWriter.h"

#include "Exception.h"
#include "boost/lexical_cast.hpp"

#include <xercesc/dom/DOM.hpp>

#include <iostream>
#include "Stream.h"
#include "Operator.h"

using namespace xercesc;

namespace stream
{
    void XmlWriter::write(const std::string& filename, Stream& stream)
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
            return;
        }
        
        try
        {
            XMLCh tempStr[100];
            XMLString::transcode("", tempStr, 99);
            DOMImplementation* impl = DOMImplementationRegistry::getDOMImplementation(tempStr);

            XMLString::transcode("Stream", tempStr, 99); 
            DOMDocument* doc = impl->createDocument(0, tempStr, 0);
            DOMElement* Stream = doc->getDocumentElement();
            XMLString::transcode("name", tempStr, 99);
            DOMAttr* name = doc->createAttribute(tempStr);
            XMLString::transcode(stream.name().c_str(), tempStr, 99);
            name->setValue(tempStr);
            Stream->setAttributeNode(name);
            
            //Add operator branches (tree structure: stream:operator)
            //Processed for each operator belonging to the stream object (multiple entries for stream possible)
            for(std::vector<Operator*>::const_iterator iter_op = stream.operators().begin();
                iter_op != stream.operators().end();
                ++iter_op)
            {
                //Create current operator entry op being child of stream (one for each operator possible)
                XMLString::transcode("Operator", tempStr, 99);
                DOMElement* op = doc->createElement(tempStr);
                Stream->appendChild(op);
                
                //Create attribute id of current operator op (one for each operator possible)
                XMLString::transcode("id", tempStr, 99);
                DOMAttr* id = doc->createAttribute(tempStr);
                
                //Create attribute package of current operator op (one for each operator possible)
                XMLString::transcode("package", tempStr, 99);
                DOMAttr* package = doc->createAttribute(tempStr);
                XMLString::transcode((*iter_op)->info()->package().c_str(), tempStr, 99);
                package->setValue(tempStr);
                op->setAttributeNode(package);
                
                //Create attribute type of current operator op (one for each operator possible)
                XMLString::transcode("type", tempStr, 99);
                DOMAttr* type_op = doc->createAttribute(tempStr);
                XMLString::transcode((*iter_op)->info()->type().c_str(), tempStr, 99);
                type_op->setValue(tempStr);
                op->setAttributeNode(type_op);
                
                //Create attribute name
                XMLString::transcode("name", tempStr, 99);
                DOMAttr* name_op = doc->createAttribute(tempStr);
                XMLString::transcode((*iter_op)->name().c_str(), tempStr, 99);
                name_op->setValue(tempStr);
                op->setAttributeNode(name_op);
                
                //Add parameter branches (tree structure: stream:operator:parameter)
                //Processed for each parameter belonging to current operator op (multiple entries for each operator possible)
                for(std::vector<const Parameter*>::const_iterator iter_param = (*iter_op)->info()->parameters().begin();
                    iter_param != (*iter_op)->info()->parameters().end();
                    ++iter_param)
                {
                    //Create current parameter entry param being child of current operator op (one for each parameter possible)
                    XMLString::transcode("Parameter", tempStr, 99);
                    DOMElement* param = doc->createElement(tempStr);
                    op->appendChild(param);
                    
                    //Create attribute id of current parameter param (one for each parameter possible)
                    XMLString::transcode("id", tempStr, 99);
                    DOMAttr* id = doc->createAttribute(tempStr);
                    XMLString::transcode(boost::lexical_cast<std::string>((*iter_param)->id()).c_str(), tempStr, 99);
                    id->setValue(tempStr);
                    param->setAttributeNode(id);
                   
//                     //Create type entry dataType being child of current param (one for each parameter possible)
//                     XMLString::transcode(boost::lexical_cast<std::string>((*iter_param)->type()).c_str(), tempStr, 99);
//                     DOMElement* dataType = doc->createElement(tempStr);
//                     param->appendChild(dataType);

//                     //Set value of dataType (one for each dataType possible)
//                     XMLString::transcode((*iter_param)->name().c_str(), tempStr, 99);
//                     DOMText* value = doc->createTextNode(tempStr);
//                     dataType->appendChild(value);
                }
                    
                
            }
                
//             XMLString::transcode("id", tempStr, 99);
//             DOMAttr* id = doc->createAttribute(tempStr);
//             XMLString::transcode("0", tempStr, 99);
//             id->setValue(tempStr);
//             op->setAttributeNode(id);
//             
//             
//             
//             
//             XMLString::transcode("type", tempStr, 99);
//             DOMAttr* type = doc->createAttribute(tempStr);
//             XMLString::transcode("TestOperator", tempStr, 99);
//             type->setValue(tempStr);
//             op->setAttributeNode(type);
// 
//             
// 
//             XMLString::transcode("UInt32", tempStr, 99);
//             DOMElement* dataType = doc->createElement(tempStr);
//             param->appendChild(dataType);
//             
//             XMLString::transcode("5000", tempStr, 99);
//             DOMText* value = doc->createTextNode(tempStr);
//             dataType->appendChild(value);
            
            DOMLSSerializer* serializer = impl->createLSSerializer();
            serializer->getDomConfig()->setParameter(XMLUni::fgDOMWRTFormatPrettyPrint, true);
            char* content = XMLString::transcode(serializer->writeToString(doc));
            std::cout << content << std::endl;
            XMLString::release(&content);
            serializer->release();

            // done with the document, must call release() to release the entire document resources
            doc->release();
        }
        catch(DOMException& e)
        {
            throw InternalError("Error in Xerces-C.");
        }
        catch(XMLException& e)
        {
            throw InternalError("Error in Xerces-C.");
        }
                    
        try
        {
            XMLPlatformUtils::Terminate();  // Terminate after release of memory
        }
        catch(XMLException&)
        {
        }
    }
} 
