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
        XmlWriterImpl::XmlWriterImpl() : m_stream(0), m_impl(0), m_doc(0), m_strElement(0), m_filename("")
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
        
        void XmlWriterImpl::createInputNodes(const Thread* const currThr, DOMElement* const thrElement)
        {
            //Add InputNode branches (tree structure: Stream:Thread:InputNode)
            //Processed for each InputNode belonging to Thread (multiple entries for each Thread possible)
            for(std::vector<Node>::const_iterator iter_inNodes = currThr->nodeSequence().begin();
                iter_inNodes != currThr->nodeSequence().end();
                ++iter_inNodes)
            {
                //Create current InputNode being child of Thread 
                DOMElement* inNodeElement = m_doc->createElement(Str2Xml("InputNode"));
                thrElement->appendChild(inNodeElement);
                
                //Create attribute operator of current InputNode (one for each InputNode possible)
                DOMAttr* opAttr = m_doc->createAttribute(Str2Xml("operator"));
                unsigned int opId = translateOperatorPointerToID((*iter_inNodes).op());
                opAttr->setValue(Str2Xml(boost::lexical_cast<std::string>(opId).c_str()));
                inNodeElement->setAttributeNode(opAttr);
                
                //Create attribute input of current InputNode (one for each InputNode possible)
                DOMAttr* inputAttr = m_doc->createAttribute(Str2Xml("input"));
                inputAttr->setValue(Str2Xml(boost::lexical_cast<std::string>((*iter_inNodes).id()).c_str()));
                inNodeElement->setAttributeNode(inputAttr);
            }
        }
        
        void XmlWriterImpl::createThreads(const std::vector<Thread*> threads)
        {
            //Add Thread branches (tree structure: Stream:Thread)
            //Processed for each thread belonging to the stream object (multiple entries for stream possible)
            for(std::vector<Thread*>::const_iterator iter_thr = threads.begin();
                    iter_thr != threads.end();
                    ++iter_thr)
            {
                //Create current Thread being child of Stream (one for each Thread possible)
                DOMElement* thrElement = m_doc->createElement(Str2Xml("Thread"));
                m_strElement->appendChild(thrElement);
                
                //Create attribute name of Thread (one for each Thread possible)
                DOMAttr* nameAttr = m_doc->createAttribute(Str2Xml("name"));
                nameAttr->setValue(Str2Xml((*iter_thr)->name().c_str()));
                thrElement->setAttributeNode(nameAttr);
                
                //Create InputNodes of Thread (multiple entries for each Thread possible)
                createInputNodes((*iter_thr), thrElement);
            }
        }
        
        void XmlWriterImpl::createParameters(const Operator* const currOp, DOMElement* const opElement)
        {
            //Add parameter branches (tree structure: stream:operator:parameter)
            //Processed for each parameter belonging to current operator currOp (multiple entries for each operator possible)
            for(std::vector<const Parameter*>::const_iterator iter_par = currOp->info()->parameters().begin();
                        iter_par != currOp->info()->parameters().end();
                        ++iter_par)
            {
                //Create current parameter entry param being child of current operator op (one for each parameter possible)
                DOMElement* parElement = m_doc->createElement(Str2Xml("Parameter"));
                opElement->appendChild(parElement);
                
                //Create attribute id of current parameter param (one for each parameter possible)
                DOMAttr* id = m_doc->createAttribute(Str2Xml("id"));
                id->setValue(Str2Xml(boost::lexical_cast<std::string>((*iter_par)->id()).c_str()));
                parElement->setAttributeNode(id);
                
                createData(*iter_par, currOp, parElement);
            }
        }
        
        void XmlWriterImpl::createData(const Parameter*const currPar, const Operator*const currOp, DOMElement*const parElement)
        {
            //Add Data branch
            //Create Data entry data being child of current param (one for each parameter possible)
            DOMElement* dataElement = m_doc->createElement(Str2Xml("Data"));
            parElement->appendChild(dataElement);
            
            //Create attribute type of current parameter param (one for each parameter possible)
            DOMAttr* typeAttr = m_doc->createAttribute(Str2Xml("type"));
            typeAttr->setValue(Str2Xml(currOp->getParameter(currPar->id()).type().c_str()));
            dataElement->setAttributeNode(typeAttr);
            
            //Create attribute package of current parameter param (one for each parameter possible)
            DOMAttr* packageAttr = m_doc->createAttribute(Str2Xml("package"));
            packageAttr->setValue(Str2Xml(currOp->getParameter(currPar->id()).package().c_str()));
            dataElement->setAttributeNode(packageAttr);
            
            //Create value of current parameter param (one for each parameter possible)
            //First, create unique input parameter name for function Data::serialize()
            std::string str = m_stream->name() + 
                                "_" + "op" + boost::lexical_cast<std::string>(translateOperatorPointerToID(currOp)) + 
                                "_" + "parameter" + boost::lexical_cast<std::string>(currPar->id());
            DOMText* value = m_doc->createTextNode(Str2Xml(currOp->getParameter(currPar->id()).serialize(str, impl::XmlUtilities::computePath(m_filename)).c_str()));
            dataElement->appendChild(value);
        }
        
        void XmlWriterImpl::createInputs(const stream::Operator*const currOp, DOMElement*const opElement)
        {
            for(std::vector<const Description*>::const_iterator iter_in = currOp->info()->inputs().begin();
                iter_in != currOp->info()->inputs().end();
                ++iter_in)
            {
                //Get the source node
                Node node = m_stream->source(currOp, (*iter_in)->id());
                
                //Create Input only for connected operators
                if (! node.empty())
                {
                    //Create Input entry in being child of current operator op (one for each parameter possible)
                    DOMElement* inElement = m_doc->createElement(Str2Xml("Input"));
                    opElement->appendChild(inElement);
                    
                    //Create attribute id of current input in (one for each input possible)
                    DOMAttr* idAttr = m_doc->createAttribute(Str2Xml("id"));
                    idAttr->setValue(Str2Xml(boost::lexical_cast<std::string>((*iter_in)->id()).c_str()));
                    inElement->setAttributeNode(idAttr);
                    
                    //Get the id of the source operator
                    unsigned sourceOp = translateOperatorPointerToID(node.op());
                    
                    //Write the id of the source operator
                    DOMAttr* opAttr = m_doc->createAttribute(Str2Xml("operator"));
                    opAttr->setValue(Str2Xml(boost::lexical_cast<std::string>(sourceOp).c_str()));
                    inElement->setAttributeNode(opAttr);
                    
                    //Write the id of the output
                    DOMAttr* outAttr = m_doc->createAttribute(Str2Xml("output"));
                    outAttr->setValue(Str2Xml(boost::lexical_cast<std::string>(node.id()).c_str()));
                    inElement->setAttributeNode(outAttr);
                }
            }
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
                DOMElement* opElement = m_doc->createElement(Str2Xml("Operator"));
                m_strElement->appendChild(opElement);
                
                //Create attribute id of current operator op (one for each operator possible)
                DOMAttr* idAttr = m_doc->createAttribute(Str2Xml("id"));
                idAttr->setValue(Str2Xml(boost::lexical_cast<std::string>(translateOperatorPointerToID(*iter_op)).c_str()));
                opElement->setAttributeNode(idAttr);
                
                //Create attribute package of current operator op (one for each operator possible)
                DOMAttr* packAttr = m_doc->createAttribute(Str2Xml("package"));
                packAttr->setValue(Str2Xml((*iter_op)->info()->package().c_str()));
                opElement->setAttributeNode(packAttr);
                
                //Create attribute type of current operator op (one for each operator possible)
                DOMAttr* typeAttr = m_doc->createAttribute(Str2Xml("type"));
                typeAttr->setValue(Str2Xml((*iter_op)->info()->type().c_str()));
                opElement->setAttributeNode(typeAttr);
                
                //Create attribute name of current operator op (one for each operator possible)
                DOMAttr* nameAttr = m_doc->createAttribute(Str2Xml("name"));
                nameAttr->setValue(Str2Xml((*iter_op)->name().c_str()));
                opElement->setAttributeNode(nameAttr);
                
                //Create attribute version of current operator op (one for each operator possible)
                DOMAttr* verAttr = m_doc->createAttribute(Str2Xml("version"));
                std::string str = boost::lexical_cast<std::string>((*iter_op)->info()->version().major()) + 
                                  "." + boost::lexical_cast<std::string>((*iter_op)->info()->version().minor());
                verAttr->setValue(Str2Xml(str.c_str()));
                opElement->setAttributeNode(verAttr);
                
                createParameters((*iter_op), opElement);
                createInputs((*iter_op), opElement);
            }
        }
    
        void XmlWriterImpl::write(const std::string& filename, const Stream& stream)
        {
            if (filename.empty())
            {
                throw WrongArgument("Invalid file name.");
            }
            
            m_stream = &stream;
            m_filename = filename;
            
            try
            {
                //Create Stream branch
                m_doc = m_impl->createDocument(0, Str2Xml("Stream"), 0);
                m_strElement = m_doc->getDocumentElement();
                
                //Create attribute name of Stream
                DOMAttr* nameAttr = m_doc->createAttribute(Str2Xml("name"));
                nameAttr->setValue(Str2Xml(m_stream->name().c_str()));
                m_strElement->setAttributeNode(nameAttr);
                
                //Create Operators of Stream (multiple entries for Stream possible)
                createOperators(m_stream->operators());
                //Create Threads of Stream (multiple entries for Stream possible)
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
