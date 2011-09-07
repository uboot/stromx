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

#include "../Data.h"
#include "../Exception.h"
#include "../Node.h"
#include "../Operator.h"
#include "../Thread.h"
#include "XmlWriterImpl.h"

using namespace xercesc;

namespace stream
{
    namespace impl
    {
        XmlWriterImpl::XmlWriterImpl() : m_stream(0), m_impl(0), m_doc(0), m_Stream(0), m_filename("")
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
                m_impl = DOMImplementationRegistry::getDOMImplementation(Str2Xml(""));
                m_doc = m_impl->createDocument(0, Str2Xml("Stream"), 0);
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
        
        const unsigned int XmlWriterImpl::translateOperatorPointerToID(const Operator* const op) const
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
        
        void XmlWriterImpl::createInputNodes(const std::vector<Node> inNodes, DOMElement* const thread)
        {
            //Add InputNode branches (tree structure: Stream:Thread:InputNode)
            //Processed for each InputNode belonging to Thread (multiple entries for each Thread possible)
            for(std::vector<Node>::const_iterator iter_inNodes = inNodes.begin();
                iter_inNodes != inNodes.end();
                ++iter_inNodes)
            {
                //Create current InputNode being child of Thread 
                DOMElement* inNode = m_doc->createElement(Str2Xml("InputNode"));
                thread->appendChild(inNode);
                
                //Create attribute operator of current InputNode (one for each InputNode possible)
                DOMAttr* opAttr = m_doc->createAttribute(Str2Xml("operator"));
                unsigned int opId = translateOperatorPointerToID((*iter_inNodes).op());
                opAttr->setValue(Str2Xml(boost::lexical_cast<std::string>(opId).c_str()));
                inNode->setAttributeNode(opAttr);
                
                //Create attribute input of current InputNode (one for each InputNode possible)
                DOMAttr* inputAttr = m_doc->createAttribute(Str2Xml("input"));
                inputAttr->setValue(Str2Xml(boost::lexical_cast<std::string>((*iter_inNodes).id()).c_str()));
                inNode->setAttributeNode(inputAttr);
            }
        }
        
        void XmlWriterImpl::createThreads(const std::vector<Thread*> threads)
        {
            for(std::vector<Thread*>::const_iterator iter_thr = threads.begin();
                    iter_thr != threads.end();
                    ++iter_thr)
            {
                //Create current Thread being child of Stream (one for each Thread possible)
                DOMElement* thr = m_doc->createElement(Str2Xml("Thread"));
                m_Stream->appendChild(thr);
                
                //Create attribute name of Thread (one for each Thread possible)
                DOMAttr* nameAttr = m_doc->createAttribute(Str2Xml("name"));
                nameAttr->setValue(Str2Xml((*iter_thr)->name().c_str()));
                thr->setAttributeNode(nameAttr);
                
                //Create InputNodes of Thread (multiple entries for each Thread possible)
                createInputNodes((*iter_thr)->nodeSequence(), thr);
            }
        }
        
        void XmlWriterImpl::createParameters(const std::vector<const Parameter*> parameters, const Operator* const currOp, DOMElement* const op)
        {
            //Add parameter branches (tree structure: stream:operator:parameter)
            //Processed for each parameter belonging to current operator op (multiple entries for each operator possible)
            for(std::vector<const Parameter*>::const_iterator iter_param = parameters.begin();
                        iter_param != parameters.end();
                        ++iter_param)
            {
                //Create current parameter entry param being child of current operator op (one for each parameter possible)
                DOMElement* param = m_doc->createElement(Str2Xml("Parameter"));
                op->appendChild(param);
                
                //Create attribute id of current parameter param (one for each parameter possible)
                DOMAttr* id = m_doc->createAttribute(Str2Xml("id"));
                id->setValue(Str2Xml(boost::lexical_cast<std::string>((*iter_param)->id()).c_str()));
                param->setAttributeNode(id);
                
                createData(*iter_param, currOp, param);
            }
        }
        
        void XmlWriterImpl::createData(const Parameter*const parameter, const Operator*const currOp, DOMElement*const param)
        {
            //Add Data branch
            //Create Data entry data being child of current param (one for each parameter possible)
            DOMElement* data = m_doc->createElement(Str2Xml("Data"));
            param->appendChild(data);
            
            //Create attribute type of current parameter param (one for each parameter possible)
            DOMAttr* type_data = m_doc->createAttribute(Str2Xml("type"));
            type_data->setValue(Str2Xml(currOp->getParameter(parameter->id()).type().c_str()));
            data->setAttributeNode(type_data);
            
            //Create attribute package of current parameter param (one for each parameter possible)
            DOMAttr* package_data = m_doc->createAttribute(Str2Xml("package"));
            package_data->setValue(Str2Xml(currOp->getParameter(parameter->id()).package().c_str()));
            data->setAttributeNode(package_data);
            
            //Create value of current parameter param (one for each parameter possible)
            //First, create unique input parameter name for function Data::serialize()
            std::string str = m_stream->name() + 
                                "_" + "op" + boost::lexical_cast<std::string>(translateOperatorPointerToID(currOp)) + 
                                "_" + "parameter" + boost::lexical_cast<std::string>(parameter->id());
            DOMText* value = m_doc->createTextNode(Str2Xml(currOp->getParameter(parameter->id()).serialize(str, impl::XmlUtilities::computePath(m_filename)).c_str()));
            data->appendChild(value);
        }
        
        void XmlWriterImpl::createOperators(const std::vector<Operator*> operators)
        {
            //Add operator branches (tree structure: stream:operator)
            //Processed for each operator belonging to the stream object (multiple entries for stream possible)
            for(std::vector<Operator*>::const_iterator iter_op = operators.begin();
                iter_op != operators.end();
                ++iter_op)
            {
                //Create current operator entry op being child of stream (one for each operator possible)
                DOMElement* op = m_doc->createElement(Str2Xml("Operator"));
                m_Stream->appendChild(op);
                
                //Create attribute id of current operator op (one for each operator possible)
                DOMAttr* id = m_doc->createAttribute(Str2Xml("id"));
                id->setValue(Str2Xml(boost::lexical_cast<std::string>(translateOperatorPointerToID(*iter_op)).c_str()));
                op->setAttributeNode(id);
                
                //Create attribute package of current operator op (one for each operator possible)
                DOMAttr* package = m_doc->createAttribute(Str2Xml("package"));
                package->setValue(Str2Xml((*iter_op)->info()->package().c_str()));
                op->setAttributeNode(package);
                
                //Create attribute type of current operator op (one for each operator possible)
                DOMAttr* type_op = m_doc->createAttribute(Str2Xml("type"));
                type_op->setValue(Str2Xml((*iter_op)->info()->type().c_str()));
                op->setAttributeNode(type_op);
                
                //Create attribute name
                DOMAttr* name_op = m_doc->createAttribute(Str2Xml("name"));
                name_op->setValue(Str2Xml((*iter_op)->name().c_str()));
                op->setAttributeNode(name_op);
                
                createParameters((*iter_op)->info()->parameters(), (*iter_op), op);
            }
        }
    
        void XmlWriterImpl::write(const std::string& filename, Stream& stream)
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
            m_filename = filename;
            
            try
            {
    //             XMLCh tempStr[100];
    //             XMLString::transcode("", tempStr, 99);
    //             DOMImplementation* impl = DOMImplementationRegistry::getDOMImplementation(tempStr);
    // 
    //             XMLString::transcode("Stream", tempStr, 99); 
    //             DOMDocument* doc = impl->createDocument(0, tempStr, 0);
    //             DOMElement* Stream = doc->getDocumentElement();
                DOMAttr* name = m_doc->createAttribute(Str2Xml("name"));
                name->setValue(Str2Xml(m_stream->name().c_str()));
                m_Stream->setAttributeNode(name);
                

                    
// // //                     //Add input branches (tree structure: stream:operator:input)
// // //                     //Processed for each input belonging to current operator op (multiple entries for each operator possible)
// // //                     for(std::vector<const Description*>::const_iterator iter_in = (*iter_op)->info()->inputs().begin();
// // //                         iter_in != (*iter_op)->info()->inputs().end();
// // //                         ++iter_in)
// // //                     {
// // //                         //Create branches only for connected operators
// // //                         if (! m_stream->source((*iter_op),(*iter_in)->id()).empty())
// // //                         {
// // //                         
// // //                         //Create current input entry in being child of current operator op (one for each parameter possible)
// // //                         XMLString::transcode("Input", m_tempStr, 99);
// // //                         DOMElement* in = m_doc->createElement(m_tempStr);
// // //                         op->appendChild(in);
// // //                         
// // //                         //Create attribute id of current input in (one for each input possible)
// // //                         XMLString::transcode("id", m_tempStr, 99);
// // //                         DOMAttr* id = m_doc->createAttribute(m_tempStr);
// // //                         XMLString::transcode(boost::lexical_cast<std::string>((*iter_in)->id()).c_str(), m_tempStr, 99);
// // //                         id->setValue(m_tempStr);
// // //                         in->setAttributeNode(id);
// // //                         
// // // //                         //Create attribute operator of current input in (one for each input possible)
// // // //                         DOMAttr* op = m_doc->createAttribute(Str2Xml("operator"));
// // // //                         unsigned int out_op_id = translateOperatorPointerToID(m_stream->source((*iter_op),(*iter_in)->id()).op());
// // // //                         XMLString::transcode(boost::lexical_cast<std::string>(out_op_id).c_str(), m_tempStr, 99);
// // // //                         op->setValue(m_tempStr);
// // // //                         in->setAttributeNode(op);
// // // //                         }
// // // //                     }
                    
// // // //                 }
                createOperators(m_stream->operators());
                createThreads(m_stream->threads());
                
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
} 
