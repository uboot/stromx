/* 
 *  Copyright 2011 Matthias Fuchs
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

#include <boost/assert.hpp>
#include <boost/lexical_cast.hpp>
#include <xercesc/framework/MemBufInputSource.hpp>
#include <xercesc/parsers/XercesDOMParser.hpp>
#include <xercesc/sax/HandlerBase.hpp>
#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/util/XMLString.hpp>
#include <iostream>
#include <fstream>
#include "XmlReaderImpl.h"
#include "XmlUtilities.h"
#include "../Exception.h"
#include "../Factory.h"
#include "../Data.h"
#include "../DirectoryFileInput.h"
#include "../Stream.h"
#include "../Operator.h"
#include "../Thread.h"
#include "../Parameter.h"

namespace stromx
{
    namespace core
    {
        namespace impl
        {
           using namespace xercesc;
            
            namespace
            {
                 const std::string DTD =
" \
<!ELEMENT Stromx (Stream?, Parameters?)> \
<!ATTLIST Stromx \
    version CDATA #REQUIRED \
> \
\
<!ELEMENT Stream (Operator*, Thread*)> \
<!ATTLIST Stream \
    name CDATA #IMPLIED \
> \
\
<!ELEMENT Parameters (Operator*)> \
\
<!ELEMENT Thread (InputConnector*)> \
<!ATTLIST Thread \
    name CDATA #IMPLIED \
> \
\
<!ELEMENT Operator (Parameter*, Input*)> \
<!ATTLIST Operator \
    id NMTOKEN #REQUIRED \
    package CDATA #REQUIRED \
    type CDATA #REQUIRED \
    name CDATA #IMPLIED \
    version CDATA #REQUIRED \
> \
\
<!ELEMENT Parameter (Data)> \
<!ATTLIST Parameter \
    id NMTOKEN #REQUIRED \
> \
\
<!ELEMENT Data (#PCDATA)> \
<!ATTLIST Data \
    type CDATA #REQUIRED \
    package CDATA #REQUIRED \
    version CDATA #REQUIRED \
    file CDATA #IMPLIED \
> \
\
<!ELEMENT Input EMPTY> \
<!ATTLIST Input \
    id NMTOKEN #REQUIRED \
    operator NMTOKEN #REQUIRED \
    output NMTOKEN #REQUIRED \
> \
\
<!ELEMENT InputConnector EMPTY> \
<!ATTLIST InputConnector \
    operator NMTOKEN #REQUIRED \
    input NMTOKEN #REQUIRED \
> \
"
                ;
            
                class XercesErrorHandler : public HandlerBase
                {
                public:
                    XercesErrorHandler(const std::string & filename) : m_filename(filename) {}
                    
                    virtual void error (const SAXParseException &/*exc*/)
                    {
                        throw InvalidFileFormat(m_filename, "XML file is not valid.");
                    }
                    
                private:
                    std::string m_filename;
                };
                
                class XercesEntityResolver : public HandlerBase
                {
                public:
                    virtual InputSource* resolveEntity (const XMLCh *const /*publicId*/, const XMLCh *const /*systemId*/)
                    {
                        InputSource* grammar = new MemBufInputSource(reinterpret_cast<const XMLByte*>(DTD.c_str()), DTD.size(), "");
                        return grammar;
                    }
                };
            }
            
            XmlReaderImpl::XmlReaderImpl(const core::Factory& factory)
              : m_factory(factory),
                m_stream(0),
                m_input(0)
            {
                try
                {
                    XMLPlatformUtils::Initialize();  // Initialize Xerces infrastructure
                }
                catch (const XMLException& toCatch)
                {
                    char* message = XMLString::transcode(toCatch.getMessage());
                    
                    InternalError ex("Failed to initialize xerces-c: " + std::string(message));
                    XMLString::release(&message);
                    throw ex;
                }
            }

            XmlReaderImpl::~XmlReaderImpl()
            {  
                try
                {
                    XMLPlatformUtils::Terminate();  // Terminate after release of memory
                }
                catch(XMLException&)
                {
                }
                
                for(std::map<unsigned int, Data*>::iterator iter = m_id2DataMap.begin();
                    iter != m_id2DataMap.end();
                    ++iter)
                {
                    delete iter->second;
                }
            }

            Stream* XmlReaderImpl::readStream(FileInput & input, const std::string & filename)
            {    
                std::auto_ptr<ErrorHandler> errHandler(new XercesErrorHandler(filename));
                std::auto_ptr<EntityResolver> entityResolver(new XercesEntityResolver);
                
                std::auto_ptr<XercesDOMParser> parser(new XercesDOMParser());

                parser->setErrorHandler(errHandler.get());
                parser->setEntityResolver(entityResolver.get());
                parser->setValidationScheme(XercesDOMParser::Val_Always);

                std::stringbuf contentBuffer;
                m_input = &input;
                m_input->initialize("", filename);
                m_input->openFile(FileInput::TEXT).get(contentBuffer, 0);
                
                std::string content = contentBuffer.str();
                
                MemBufInputSource source(reinterpret_cast<const XMLByte*>(content.c_str()), content.size(), (XMLCh*)(0));

                try
                {
                    parser->parse(source);
                }
                catch (const XMLException& toCatch)
                {
                    char* message = XMLString::transcode(toCatch.getMessage());
                    
                    InvalidFileFormat ex(filename, "", "XML exception: " + std::string(message));
                    XMLString::release(&message);
                    throw ex;
                }
                catch (const DOMException& toCatch)
                {
                    char* message = XMLString::transcode(toCatch.msg);
                    
                    InternalError ex("DOM exception: " + std::string(message));
                    XMLString::release(&message);
                    throw ex;
                }
                catch (Exception&)
                {
                    throw;
                }
                catch (...)
                {
                    throw InternalError("Unexpected exception.");
                }
                
                try
                {
                    m_stream = new Stream();
                    
                    DOMDocument* doc = parser->getDocument();
                    
                    if(! doc)
                        throw FileAccessFailed(filename, "Failed to read file.");
                    
                    DOMNodeList* streamNodes = doc->getElementsByTagName(Str2Xml("Stream"));
                    
                    if(! streamNodes->getLength())
                        throw FileAccessFailed(filename, "Found no element <Stream/>.");
                    
                    DOMElement* stream = dynamic_cast<DOMElement*>(streamNodes->item(0));
                    
                    Xml2Str name(stream->getAttribute(Str2Xml("name")));
                    m_stream->setName(std::string(name));
                    
                    DOMNodeList* operators = stream->getElementsByTagName(Str2Xml("Operator"));
                    XMLSize_t numOperators = operators->getLength();
                    
                    // read the operators
                    for(unsigned int i = 0; i < numOperators; ++i)
                    {
                        DOMElement* op = dynamic_cast<DOMElement*>(operators->item(i));
                        readOperator(op);
                    }
                    
                    // read the inputs of each operator
                    for(unsigned int i = 0; i < numOperators; ++i)
                    {
                        DOMElement* op = dynamic_cast<DOMElement*>(operators->item(i));
                        readOperatorInputs(op);
                    }
                    
                    DOMNodeList* threads = stream->getElementsByTagName(Str2Xml("Thread"));
                    XMLSize_t numThreads = threads->getLength();
                    
                    for(unsigned int i = 0; i < numThreads; ++i)
                    {
                        DOMElement* threadElement = dynamic_cast<DOMElement*>(threads->item(i));
                        Thread* thread = m_stream->addThread();
                        readThread(threadElement, thread);
                    }
                }
                catch(xercesc::XMLException& toCatch)
                {
                    char* message = XMLString::transcode(toCatch.getMessage());
                    
                    InternalError ex("XML exception: " + std::string(message));
                    XMLString::release(&message);
                    
                    delete m_stream;
                    throw ex;
                }
                catch(XmlError& e)
                {
                    delete m_stream;
                    throw InconsistentFileContent(filename, e.message());
                }
                catch(WrongArgument& e)
                {
                    delete m_stream;
                    throw InconsistentFileContent(filename, e.message());
                }
                catch(core::Exception&)
                {
                    delete m_stream;
                    throw;
                }
                catch(boost::bad_lexical_cast & e)
                {
                    delete m_stream;
                    throw FileAccessFailed(filename, "", e.what());
                }
                
                return m_stream;
            }  
            
            void XmlReaderImpl::readParameters(FileInput& input, const std::string filename, const std::vector< stromx::core::Operator* > operators)
            {
                std::auto_ptr<ErrorHandler> errHandler(new XercesErrorHandler(filename));
                std::auto_ptr<EntityResolver> entityResolver(new XercesEntityResolver);
                
                XercesDOMParser* parser = new XercesDOMParser();

                parser->setErrorHandler(errHandler.get());
                parser->setEntityResolver(entityResolver.get());
                parser->setValidationScheme(XercesDOMParser::Val_Always);

                std::stringbuf contentBuffer;
                m_input = &input;
                m_input->initialize("", filename);
                m_input->openFile(FileInput::TEXT).get(contentBuffer, 0);
                
                std::string content = contentBuffer.str();
                
                MemBufInputSource source(reinterpret_cast<const XMLByte*>(content.c_str()), content.size(), (XMLCh*)(0));

                try
                {
                    parser->parse(source);
                }
                catch (const XMLException& toCatch)
                {
                    char* message = XMLString::transcode(toCatch.getMessage());
                    
                    InvalidFileFormat ex(filename, "", "XML exception: " + std::string(message));
                    XMLString::release(&message);
                    throw ex;
                }
                catch (const DOMException& toCatch)
                {
                    char* message = XMLString::transcode(toCatch.msg);
                    
                    InternalError ex("DOM exception: " + std::string(message));
                    XMLString::release(&message);
                    throw ex;
                }
                catch (Exception&)
                {
                    throw;
                }
                catch (...)
                {
                    throw InternalError("Unexpected exception.");
                }
                
                try
                {
                    DOMDocument* doc = parser->getDocument();
                    
                    if(! doc)
                        throw FileAccessFailed(filename, "Failed to read file.");
                    
                    DOMNodeList* parameterNodes = doc->getElementsByTagName(Str2Xml("Parameters"));
                    
                    if(! parameterNodes->getLength())
                        throw FileAccessFailed(filename, "Found no element <Parameters/>.");
                    
                    DOMElement* parametersItem = dynamic_cast<DOMElement*>(parameterNodes->item(0));
                    
                    DOMNodeList* operatorNodes = parametersItem->getElementsByTagName(Str2Xml("Operator"));
                    XMLSize_t numOperators = operatorNodes->getLength();
                    
                    if(numOperators != operators.size())
                        throw FileAccessFailed(filename, "The number or <Operator/> tags does not match the number of input operators.");
                    
                    // read the operators
                    for(unsigned int i = 0; i < numOperators; ++i)
                    {
                        DOMElement* opElement = dynamic_cast<DOMElement*>(operatorNodes->item(i));
                        Xml2Str idStr(opElement->getAttribute(Str2Xml("id")));
                        unsigned int id = boost::lexical_cast<unsigned int>((const char*)(idStr));
                        
                        if(id >= operators.size())
                            throw InconsistentFileContent(filename, "No operator with ID " + boost::lexical_cast<std::string>(id) + ".");
                        
                        // get the operator with the correct ID
                        Operator* op = operators[id];
                
                        DOMNodeList* parameters = opElement->getElementsByTagName(Str2Xml("Parameter"));
                        XMLSize_t numParameters = parameters->getLength();
                        
                        // read the parameters of the current operator
                        for(unsigned int i = 0; i < numParameters; ++i)
                        {
                            DOMElement* paramElement = dynamic_cast<DOMElement*>(parameters->item(i));
                            readParameter(paramElement);
                        }
                    
                        // set parameters
                        for(std::map<unsigned int, stromx::core::Data*>::const_iterator iter = m_id2DataMap.begin();
                            iter != m_id2DataMap.end();
                            ++iter)
                        {
                            try
                            {
                                op->setParameter(iter->first, *(iter->second));
                            }
                            catch(stromx::core::OperatorError&)
                            {
                                // ignore exceptions
                            }
                            
                            delete iter->second;
                        }
                        
                        // clear the current parameters
                        m_id2DataMap.clear();
                    }
                }
                catch(xercesc::XMLException& toCatch)
                {
                    char* message = XMLString::transcode(toCatch.getMessage());
                    
                    InternalError ex("XML exception: " + std::string(message));
                    XMLString::release(&message);
                    throw ex;
                }
                catch(XmlError& e)
                {
                    delete m_stream;
                    throw InconsistentFileContent(filename, e.message());
                }
                catch(core::Exception&)
                {
                    throw;
                }
                catch(boost::bad_lexical_cast & e)
                {
                    throw FileAccessFailed(filename, "", e.what());
                }

                delete parser;
            }
            
            void XmlReaderImpl::readOperator(DOMElement*const opElement)
            {
                Xml2Str name(opElement->getAttribute(Str2Xml("name")));
                Xml2Str idStr(opElement->getAttribute(Str2Xml("id")));
                Xml2Str type(opElement->getAttribute(Str2Xml("type")));
                Xml2Str package(opElement->getAttribute(Str2Xml("package")));
                
                unsigned int id = boost::lexical_cast<unsigned int>((const char*)(idStr));
                
                if(m_id2OperatorMap.count(id))
                    throw XmlError("Multiple operators with the same ID.");
                
                Operator* op = m_factory.newOperator(std::string(package), std::string(type));
                op->setName(std::string(name));
                
                m_id2OperatorMap[id] = op;
                
                DOMNodeList* parameters = opElement->getElementsByTagName(Str2Xml("Parameter"));
                XMLSize_t numParameters = parameters->getLength();
                
                // read the parameters
                for(unsigned int i = 0; i < numParameters; ++i)
                {
                    DOMElement* paramElement = dynamic_cast<DOMElement*>(parameters->item(i));
                    readParameter(paramElement);
                }
            
                // set parameters before initialization
                for(std::vector<const Parameter*>::const_iterator iter = op->info().parameters().begin();
                    iter != op->info().parameters().end();
                    ++iter)
                {
                    if((*iter)->accessMode() != Parameter::NONE_WRITE)
                        continue;
                        
                    std::map<unsigned int, Data*>::iterator idDataPair = m_id2DataMap.find((*iter)->id());
                    
                    if(idDataPair == m_id2DataMap.end())
                        continue;
                    
                    op->setParameter(idDataPair->first, *idDataPair->second);
                    
                    delete idDataPair->second;
                    m_id2DataMap.erase(idDataPair);
                }
                
                // initialize
                op->initialize();
            
                // set parameters after initialization
                for(std::vector<const Parameter*>::const_iterator iter = op->info().parameters().begin();
                    iter != op->info().parameters().end();
                    ++iter)
                {
                    if((*iter)->accessMode() != Parameter::INITIALIZED_WRITE
                        &&  (*iter)->accessMode() != Parameter::ACTIVATED_WRITE)
                    {
                        continue;
                    }
                        
                    std::map<unsigned int, Data*>::iterator idDataPair = m_id2DataMap.find((*iter)->id());
                    
                    if(idDataPair == m_id2DataMap.end())
                        continue;
                    
                    op->setParameter(idDataPair->first, *idDataPair->second);
                    
                    delete idDataPair->second;
                    m_id2DataMap.erase(idDataPair);
                }
                
                if(! m_id2DataMap.empty())
                    throw XmlError("Not all parameters of operator '" + op->name() + "' could be set.");
                
                m_stream->addOperator(op);
            }
            
            void XmlReaderImpl::readOperatorInputs(DOMElement*const opElement)
            {
                Xml2Str idStr(opElement->getAttribute(Str2Xml("id")));
                unsigned int id = boost::lexical_cast<unsigned int>((const char*)(idStr));
                
                BOOST_ASSERT(m_id2OperatorMap.count(id));
                
                DOMNodeList* inputs = opElement->getElementsByTagName(Str2Xml("Input"));
                XMLSize_t numInputs = inputs->getLength();
                
                // read the parameters
                for(unsigned int i = 0; i < numInputs; ++i)
                {
                    DOMElement* inputElement = dynamic_cast<DOMElement*>(inputs->item(i));
                    readInput(inputElement, m_id2OperatorMap[id]);
                }
            }
            
            void XmlReaderImpl::readParameter(DOMElement*const paramElement)
            {
                Xml2Str idStr(paramElement->getAttribute(Str2Xml("id")));
                
                unsigned int id = boost::lexical_cast<unsigned int>((const char*)(idStr));
                
                DOMNodeList* dataElements = paramElement->getElementsByTagName(Str2Xml("Data"));
                XMLSize_t numDataElements = dataElements->getLength();
                
                if(! numDataElements)
                    return;
                
                if(numDataElements != 1)
                    throw XmlError("More than one <Data/> elements for parameter.");
                
                DOMElement* dataElement = dynamic_cast<DOMElement*>(dataElements->item(0));
                Data* data = readData(dataElement);
                
                if(m_id2DataMap.count(id))
                    throw XmlError("Multiple parameters with the same ID " + boost::lexical_cast<std::string>(id) + ".");
                
                m_id2DataMap[id] = data;
            }
            
            Data* XmlReaderImpl::readData(DOMElement*const dataElement)
            {
                Xml2Str type(dataElement->getAttribute(Str2Xml("type")));
                Xml2Str package(dataElement->getAttribute(Str2Xml("package")));
                std::string versionString(Xml2Str(dataElement->getAttribute(Str2Xml("version"))));
                std::string file(Xml2Str(dataElement->getAttribute(Str2Xml("file"))));
                
                Version version = XmlUtilities::convertToVersion(versionString);
                
                DOMNodeList* dataTextElements = dataElement->getChildNodes();
                XMLSize_t numDataTextElements = dataTextElements->getLength();
                std::string textData;
                
                if(numDataTextElements > 1)
                    throw XmlError("More than one children of <Data/>.");
                
                if(numDataTextElements)
                {
                    DOMNode* node = dataTextElements->item(0);
                    if(node->getNodeType() != DOMNode::TEXT_NODE)
                        throw XmlError("Child of <Data/> must be a text node.");
                    
                    textData = std::string(Xml2Str(node->getNodeValue()));
                }
                
                Data* data = m_factory.newData(std::string(package), std::string(type));
                
                try
                {
                    m_input->initialize(textData, file);
                    data->deserialize(*m_input, version);
                }
                catch(FileException &)
                {
                    throw;
                }
                catch(std::exception& e)
                {
                    delete data;
                    throw DeserializationError(data->package(), data->type(), e.what());
                }
                
                return data;
            }
            
            void XmlReaderImpl::readThread(DOMElement*const threadElement, Thread*const thread)
            {
                Xml2Str type(threadElement->getAttribute(Str2Xml("name")));
                thread->setName(std::string(type));
                
                DOMNodeList* inputs = threadElement->getElementsByTagName(Str2Xml("InputConnector"));
                XMLSize_t numInputs = inputs->getLength();
                
                for(unsigned int i = 0; i < numInputs; ++i)
                {
                    DOMElement* inputElement = dynamic_cast<DOMElement*>(inputs->item(i));
                    readInputConnector(inputElement, thread);
                }
            }
            
            void XmlReaderImpl::readInputConnector(DOMElement*const inputNodeElement, Thread*const thread)
            {
                Xml2Str opIdStr(inputNodeElement->getAttribute(Str2Xml("operator")));
                Xml2Str inputIdStr(inputNodeElement->getAttribute(Str2Xml("input")));
                
                unsigned int opId = boost::lexical_cast<unsigned int>(std::string(opIdStr));
                unsigned int inputId = boost::lexical_cast<unsigned int>(std::string(inputIdStr));
                
                std::map<unsigned int, Operator*>::iterator idOpPair = m_id2OperatorMap.find(opId);
                
                if(idOpPair == m_id2OperatorMap.end())
                    throw XmlError("No operator with ID " + std::string(opIdStr) + ".");
                
                thread->addInput(idOpPair->second, inputId);
            }
            
            void XmlReaderImpl::readInput(DOMElement*const inputElement, Operator*const op)
            {
                Xml2Str opIdStr(inputElement->getAttribute(Str2Xml("operator")));
                Xml2Str inputIdStr(inputElement->getAttribute(Str2Xml("id")));
                Xml2Str outputIdStr(inputElement->getAttribute(Str2Xml("output")));
                
                unsigned int opId = boost::lexical_cast<unsigned int>(std::string(opIdStr));
                unsigned int inputId = boost::lexical_cast<unsigned int>(std::string(inputIdStr));
                unsigned int outputId = boost::lexical_cast<unsigned int>(std::string(outputIdStr));
                
                std::map<unsigned int, Operator*>::iterator idOpPair = m_id2OperatorMap.find(opId);
                
                if(idOpPair == m_id2OperatorMap.end())
                    throw XmlError("No source operator with ID " + std::string(opIdStr) + ".");
                
                Operator* source = idOpPair->second;
                
                m_stream->connect(source, outputId, op, inputId);
            }
        }
    }
}
