/* 
 *  Copyright 2011 Thomas Fidler
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

#include <boost/lexical_cast.hpp>
#include <iostream>

#include "Data.h"
#include "Exception.h"
#include "Node.h"
#include "Operator.h"
#include "Thread.h"
#include "XmlWriter.h"

using namespace xercesc;

namespace stream
{
    XmlWriter::XmlWriter() : m_stream(0)
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
            XMLString::transcode("", m_tempStr, 99);
            m_impl = DOMImplementationRegistry::getDOMImplementation(m_tempStr);

            XMLString::transcode("Stream", m_tempStr, 99); 
            m_doc = m_impl->createDocument(0, m_tempStr, 0);
            
            m_Stream = m_doc->getDocumentElement();
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
    
    const unsigned int XmlWriter::translateOperatorPointerToID(const Operator* const op) const
    {
        unsigned int count_op = 0;
        for(std::vector<Operator*>::const_iterator iter_op = m_stream->operators().begin();
                iter_op != m_stream->operators().end();
                ++iter_op, ++count_op)
        {
            if ((*iter_op) == op)
            {
                return count_op;
            }
        }
        
        throw InternalError("Operator does not exist.");
    }
    
    void XmlWriter::createThreadBranches(const std::vector<Thread*> thr)
    {
        for(std::vector<Thread*>::const_iterator iter_thr = thr.begin();
                iter_thr != thr.end();
                ++iter_thr)
        {
            //Create current thread entry thr being child of stream (one for each thread possible)
            XMLString::transcode("Thread", m_tempStr, 99);
            DOMElement* thr = m_doc->createElement(m_tempStr);
            m_Stream->appendChild(thr);
            
            //Create attribute name of current thread thr (one for each thread possible)
            XMLString::transcode("name", m_tempStr, 99);
            DOMAttr* name_thr = m_doc->createAttribute(m_tempStr);
            XMLString::transcode((*iter_thr)->name().c_str(), m_tempStr, 99);
            name_thr->setValue(m_tempStr);
            thr->setAttributeNode(name_thr);
            
            //Add InputNode branches (tree structure: Stream:Thread:InputNode)
            //Processed for each InputNode belonging to current thread thr (multiple entries for each thread possible)
            for(std::vector<Node>::const_iterator iter_in_node = (*iter_thr)->nodeSequence().begin();
                iter_in_node != (*iter_thr)->nodeSequence().end();
                ++iter_in_node)
            {
                //Create current InputNode entry in_node being child of current Thread thr
                XMLString::transcode("InputNode", m_tempStr, 99);
                DOMElement* in_node = m_doc->createElement(m_tempStr);
                thr->appendChild(in_node);
                
                //Create attribute operator of current InputNode in_node (one for each InputNode possible)
                XMLString::transcode("operator", m_tempStr, 99);
                DOMAttr* in_node_op = m_doc->createAttribute(m_tempStr);
                unsigned int op_id = translateOperatorPointerToID((*iter_in_node).op());
                XMLString::transcode(boost::lexical_cast<std::string>(op_id).c_str(), m_tempStr, 99);
                in_node_op->setValue(m_tempStr);
                in_node->setAttributeNode(in_node_op);
                
                //Create attribute input of current InputNode in_node (one for each InputNode possible)
                XMLString::transcode("input", m_tempStr, 99);
                DOMAttr* in_node_input = m_doc->createAttribute(m_tempStr);
                XMLString::transcode(boost::lexical_cast<std::string>((*iter_in_node).id()).c_str(), m_tempStr, 99);
                in_node_input->setValue(m_tempStr);
                in_node->setAttributeNode(in_node_input);
            }
        }
    }
   
    void XmlWriter::write(const std::string& filename, Stream& stream)
    {
        if (filename.empty())
        {
            throw WrongArgument("Invalid file name.");
        }
        
        if (&stream == 0)
        {
            throw WrongArgument("Invalid argument: Null pointer.");
        }
        
        m_stream = &stream;
        
//         try
//         {
//             XMLPlatformUtils::Initialize();  // Initialize Xerces infrastructure
//         }
//         catch (const XMLException& toCatch)
//         {
//             char* message = XMLString::transcode(toCatch.getMessage());
//             std::cerr << "Error during initialization! :\n"
//                  << message << "\n";
//             XMLString::release(&message);
//             return;
//         }
        
        try
        {
//             XMLCh tempStr[100];
//             XMLString::transcode("", tempStr, 99);
//             DOMImplementation* impl = DOMImplementationRegistry::getDOMImplementation(tempStr);
// 
//             XMLString::transcode("Stream", tempStr, 99); 
//             DOMDocument* doc = impl->createDocument(0, tempStr, 0);
//             DOMElement* Stream = doc->getDocumentElement();
            XMLString::transcode("name", m_tempStr, 99);
            DOMAttr* name = m_doc->createAttribute(m_tempStr);
            XMLString::transcode(stream.name().c_str(), m_tempStr, 99);
            name->setValue(m_tempStr);
            m_Stream->setAttributeNode(name);
            
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
                XMLString::transcode("Operator", m_tempStr, 99);
                DOMElement* op = m_doc->createElement(m_tempStr);
                m_Stream->appendChild(op);
                
                //Create attribute id of current operator op (one for each operator possible)
                XMLString::transcode("id", m_tempStr, 99);
                DOMAttr* id = m_doc->createAttribute(m_tempStr);
                XMLString::transcode(boost::lexical_cast<std::string>(count_op).c_str(), m_tempStr, 99);
                id->setValue(m_tempStr);
                op->setAttributeNode(id);
                
                //Create attribute package of current operator op (one for each operator possible)
                XMLString::transcode("package", m_tempStr, 99);
                DOMAttr* package = m_doc->createAttribute(m_tempStr);
                XMLString::transcode((*iter_op)->info()->package().c_str(), m_tempStr, 99);
                package->setValue(m_tempStr);
                op->setAttributeNode(package);
                
                //Create attribute type of current operator op (one for each operator possible)
                XMLString::transcode("type", m_tempStr, 99);
                DOMAttr* type_op = m_doc->createAttribute(m_tempStr);
                XMLString::transcode((*iter_op)->info()->type().c_str(), m_tempStr, 99);
                type_op->setValue(m_tempStr);
                op->setAttributeNode(type_op);
                
                //Create attribute name
                XMLString::transcode("name", m_tempStr, 99);
                DOMAttr* name_op = m_doc->createAttribute(m_tempStr);
                XMLString::transcode((*iter_op)->name().c_str(), m_tempStr, 99);
                name_op->setValue(m_tempStr);
                op->setAttributeNode(name_op);
                
                //Add parameter branches (tree structure: stream:operator:parameter)
                //Processed for each parameter belonging to current operator op (multiple entries for each operator possible)
                for(std::vector<const Parameter*>::const_iterator iter_param = (*iter_op)->info()->parameters().begin();
                    iter_param != (*iter_op)->info()->parameters().end();
                    ++iter_param)
                {
                    //Create current parameter entry param being child of current operator op (one for each parameter possible)
                    XMLString::transcode("Parameter", m_tempStr, 99);
                    DOMElement* param = m_doc->createElement(m_tempStr);
                    op->appendChild(param);
                    
                    //Create attribute id of current parameter param (one for each parameter possible)
                    XMLString::transcode("id", m_tempStr, 99);
                    DOMAttr* id = m_doc->createAttribute(m_tempStr);
                    XMLString::transcode(boost::lexical_cast<std::string>((*iter_param)->id()).c_str(), m_tempStr, 99);
                    id->setValue(m_tempStr);
                    param->setAttributeNode(id);
                   
                    //Add Data branch
                    //Create Data entry data being child of current param (one for each parameter possible)
                    XMLString::transcode("Data", m_tempStr, 99);
                    DOMElement* data = m_doc->createElement(m_tempStr);
                    param->appendChild(data);
                    
                    //Create attribute type of current parameter param (one for each parameter possible)
                    XMLString::transcode("type", m_tempStr, 99);
                    DOMAttr* type_data = m_doc->createAttribute(m_tempStr);
                    XMLString::transcode((*iter_op)->getParameter((*iter_param)->id()).type().c_str(), m_tempStr, 99);
                    type_data->setValue(m_tempStr);
                    data->setAttributeNode(type_data);
                    
                    //Create attribute package of current parameter param (one for each parameter possible)
                    XMLString::transcode("package", m_tempStr, 99);
                    DOMAttr* package_data = m_doc->createAttribute(m_tempStr);
                    XMLString::transcode((*iter_op)->getParameter((*iter_param)->id()).package().c_str(), m_tempStr, 99);
                    package_data->setValue(m_tempStr);
                    data->setAttributeNode(package_data);
                    
                    //Create value of current parameter param (one for each parameter possible)
                    //First, create unique input parameter name for function Data::serialize()
                    std::string str = stream.name() + 
                                        "_" + "op" + boost::lexical_cast<std::string>(count_op) + 
                                        "_" + "parameter" + boost::lexical_cast<std::string>((*iter_param)->id());
                    XMLString::transcode((*iter_op)->getParameter((*iter_param)->id()).serialize(str, impl::XmlUtilities::computePath(filename)).c_str(), m_tempStr, 99);
                    DOMText* value = m_doc->createTextNode(m_tempStr);
                    data->appendChild(value);
                }
                
                //Add input branches (tree structure: stream:operator:input)
                //Processed for each input belonging to current operator op (multiple entries for each operator possible)
                for(std::vector<const Description*>::const_iterator iter_in = (*iter_op)->info()->inputs().begin();
                    iter_in != (*iter_op)->info()->inputs().end();
                    ++iter_in)
                {
                    //Create branches only for connected operators
//                     if (m_stream->source((*iter_op),(*iter_in)->id()))
                    
                    //Create current input entry in being child of current operator op (one for each parameter possible)
                    XMLString::transcode("Input", m_tempStr, 99);
                    DOMElement* in = m_doc->createElement(m_tempStr);
                    op->appendChild(in);
                    
                    //Create attribute id of current input in (one for each input possible)
                    XMLString::transcode("id", m_tempStr, 99);
                    DOMAttr* id = m_doc->createAttribute(m_tempStr);
                    XMLString::transcode(boost::lexical_cast<std::string>((*iter_in)->id()).c_str(), m_tempStr, 99);
                    id->setValue(m_tempStr);
                    in->setAttributeNode(id);
                    
//                     //Create attribute operator of current input in (one for each input possible)
//                     XMLString::transcode("operator", tempStr, 99);
//                     DOMAttr* op = doc->createAttribute(tempStr);
//                     unsigned int out_op_id = translateOperatorPointerToID(m_stream->source((*iter_op),(*iter_in)->id()).op());
//                     XMLString::transcode(boost::lexical_cast<std::string>(out_op_id).c_str(), tempStr, 99);
//                     op->setValue(tempStr);
//                     in->setAttributeNode(op);
                }
                
            }
            
            createThreadBranches(m_stream->threads());
            
//             for(std::vector<Thread*>::const_iterator iter_thr = stream.threads().begin();
//                 iter_thr != stream.threads().end();
//                 ++iter_thr)
//             {
//                 //Create current thread entry thr being child of stream (one for each thread possible)
//                 XMLString::transcode("Thread", tempStr, 99);
//                 DOMElement* thr = doc->createElement(tempStr);
//                 Stream->appendChild(thr);
//                 
//                 //Create attribute name of current thread thr (one for each thread possible)
//                 XMLString::transcode("name", tempStr, 99);
//                 DOMAttr* name_thr = doc->createAttribute(tempStr);
//                 XMLString::transcode((*iter_thr)->name().c_str(), tempStr, 99);
//                 name_thr->setValue(tempStr);
//                 thr->setAttributeNode(name_thr);
//                 
//                 //Add InputNode branches (tree structure: Stream:Thread:InputNode)
//                 //Processed for each InputNode belonging to current thread thr (multiple entries for each thread possible)
//                 for(std::vector<Node>::const_iterator iter_in_node = (*iter_thr)->nodeSequence().begin();
//                     iter_in_node != (*iter_thr)->nodeSequence().end();
//                     ++iter_in_node)
//                 {
//                     //Create current InputNode entry in_node being child of current Thread thr
//                     XMLString::transcode("InputNode", tempStr, 99);
//                     DOMElement* in_node = doc->createElement(tempStr);
//                     thr->appendChild(in_node);
//                     
//                     //Create attribute operator of current InputNode in_node (one for each InputNode possible)
//                     XMLString::transcode("operator", tempStr, 99);
//                     DOMAttr* in_node_op = doc->createAttribute(tempStr);
//                     unsigned int op_id = translateOperatorPointerToID((*iter_in_node).op());
//                     XMLString::transcode(boost::lexical_cast<std::string>(op_id).c_str(), tempStr, 99);
//                     in_node_op->setValue(tempStr);
//                     in_node->setAttributeNode(in_node_op);
//                     
//                     //Create attribute input of current InputNode in_node (one for each InputNode possible)
//                     XMLString::transcode("input", tempStr, 99);
//                     DOMAttr* in_node_input = doc->createAttribute(tempStr);
//                     XMLString::transcode(boost::lexical_cast<std::string>((*iter_in_node).id()).c_str(), tempStr, 99);
//                     in_node_input->setValue(tempStr);
//                     in_node->setAttributeNode(in_node_input);
//                 }
//             }
            
            DOMLSSerializer* serializer = m_impl->createLSSerializer();
            serializer->getDomConfig()->setParameter(XMLUni::fgDOMWRTFormatPrettyPrint, true);
            char* content = XMLString::transcode(serializer->writeToString(m_doc));
            std::cout << content << std::endl;
            XMLString::release(&content);
            serializer->release();

            // done with the document, must call release() to release the entire document resources
            m_doc->release();
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
