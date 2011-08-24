#include "XmlWriter.h"

#include "Exception.h"
#include "boost/lexical_cast.hpp"

#include <xercesc/dom/DOM.hpp>

#include <iostream>
#include "Stream.h"
#include "Operator.h"
#include "Data.h"
#include <boost/filesystem.hpp>

using namespace xercesc;

namespace stream
{
    const std::string XmlWriter::computePath(const std::string& filename)
    {
        boost::filesystem::path filepath(filename);
        boost::filesystem::path parentpath = filepath.parent_path();
        std::string pathSeparator;
        
        if(! parentpath.empty() && parentpath != boost::filesystem::path("/"))
            pathSeparator = boost::filesystem::path("/").file_string();
    
        return parentpath.file_string() + pathSeparator;
    }
    
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
            
            //Counter for tag id of each operator
            //In addition, neccessary for unique name in section "Create value of current parameter param" (see below)
            unsigned int count_op = 0;
            //Add operator branches (tree structure: stream:operator)
            //Processed for each operator belonging to the stream object (multiple entries for stream possible)
            for(std::vector<Operator*>::const_iterator iter_op = stream.operators().begin();
                iter_op != stream.operators().end();
                ++iter_op, ++count_op)
            {
                //Create current operator entry op being child of stream (one for each operator possible)
                XMLString::transcode("Operator", tempStr, 99);
                DOMElement* op = doc->createElement(tempStr);
                Stream->appendChild(op);
                
                //Create attribute id of current operator op (one for each operator possible)
                XMLString::transcode("id", tempStr, 99);
                DOMAttr* id = doc->createAttribute(tempStr);
                XMLString::transcode(boost::lexical_cast<std::string>(count_op).c_str(), tempStr, 99);
                id->setValue(tempStr);
                op->setAttributeNode(id);
                
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
                   
                    //Add Data branch
                    //Create Data entry data being child of current param (one for each parameter possible)
                    XMLString::transcode("Data", tempStr, 99);
                    DOMElement* data = doc->createElement(tempStr);
                    param->appendChild(data);
                    
                    //Create attribute type of current parameter param (one for each parameter possible)
                    XMLString::transcode("type", tempStr, 99);
                    DOMAttr* type_data = doc->createAttribute(tempStr);
                    XMLString::transcode((*iter_op)->getParameter((*iter_param)->id()).name().c_str(), tempStr, 99);
                    type_data->setValue(tempStr);
                    data->setAttributeNode(type_data);
                    
                    //Create attribute package of current parameter param (one for each parameter possible)
                    XMLString::transcode("package", tempStr, 99);
                    DOMAttr* package_data = doc->createAttribute(tempStr);
                    XMLString::transcode((*iter_op)->getParameter((*iter_param)->id()).package().c_str(), tempStr, 99);
                    package_data->setValue(tempStr);
                    data->setAttributeNode(package_data);
                    
                    //Create value of current parameter param (one for each parameter possible)
                    //First, create unique input parameter name for function Data::serialize()
                    std::string str = stream.name() + 
                                        "_" + "op" + boost::lexical_cast<std::string>(count_op) + 
                                        "_" + "parameter" + boost::lexical_cast<std::string>((*iter_param)->id());
                    XMLString::transcode((*iter_op)->getParameter((*iter_param)->id()).serialize(str, computePath(filename)).c_str(), tempStr, 99);
                    DOMText* value = doc->createTextNode(tempStr);
                    data->appendChild(value);
                }
                    
                
            }
            
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
