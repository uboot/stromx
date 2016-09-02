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
#include <xercesc/util/XMLEntityResolver.hpp>
#include <xercesc/util/XMLString.hpp>
#include <fstream>
#include <iostream>
#include "stromx/runtime/Data.h"
#include "stromx/runtime/DirectoryFileInput.h"
#include "stromx/runtime/Exception.h"
#include "stromx/runtime/Factory.h"
#include "stromx/runtime/Operator.h"
#include "stromx/runtime/Parameter.h"
#include "stromx/runtime/Stream.h"
#include "stromx/runtime/Thread.h"
#include "stromx/runtime/impl/XmlReaderImpl.h"
#include "stromx/runtime/impl/XmlUtilities.h"
namespace stromx
{
    namespace runtime
    {
        namespace impl
        {
           using namespace xercesc;
            
            namespace
            {
                 const std::string XSD =
" \
<xs:schema xmlns:xs=\"http://www.w3.org/2001/XMLSchema\" elementFormDefault=\"qualified\"> \
  <xs:element name=\"Stromx\"> \
    <xs:complexType> \
      <xs:sequence> \
        <xs:element minOccurs=\"0\" ref=\"Stream\"/> \
        <xs:element minOccurs=\"0\" ref=\"Parameters\"/> \
      </xs:sequence> \
      <xs:attribute name=\"version\" use=\"required\"> \
        <xs:simpleType> \
          <xs:restriction base=\"xs:string\"> \
           <xs:pattern value=\"[0-9]+.[0-9]+.[0-9]+\"/> \
          </xs:restriction> \
        </xs:simpleType> \
      </xs:attribute> \
    </xs:complexType> \
  </xs:element> \
  <xs:element name=\"Stream\"> \
    <xs:complexType> \
      <xs:sequence> \
        <xs:element minOccurs=\"0\" maxOccurs=\"unbounded\" ref=\"Operator\"/> \
        <xs:element minOccurs=\"0\" maxOccurs=\"unbounded\" ref=\"Thread\"/> \
      </xs:sequence> \
      <xs:attribute name=\"name\"/> \
    </xs:complexType> \
  </xs:element> \
  <xs:element name=\"Parameters\"> \
    <xs:complexType> \
      <xs:sequence> \
        <xs:element minOccurs=\"0\" maxOccurs=\"unbounded\" ref=\"Operator\"/> \
      </xs:sequence> \
    </xs:complexType> \
  </xs:element> \
  <xs:element name=\"Thread\"> \
    <xs:complexType> \
      <xs:sequence> \
        <xs:element minOccurs=\"0\" maxOccurs=\"unbounded\" ref=\"InputConnector\"/> \
      </xs:sequence> \
      <xs:attribute name=\"name\"/> \
      <xs:attribute name=\"color\"> \
        <xs:simpleType> \
          <xs:restriction base=\"xs:string\"> \
           <xs:pattern value=\"#([0-9a-f]{1,2}){3}\"/> \
          </xs:restriction> \
        </xs:simpleType> \
      </xs:attribute> \
    </xs:complexType> \
  </xs:element> \
  <xs:element name=\"Operator\"> \
    <xs:complexType> \
      <xs:sequence> \
        <xs:element minOccurs=\"0\" maxOccurs=\"unbounded\" ref=\"Parameter\"/> \
        <xs:element minOccurs=\"0\" maxOccurs=\"unbounded\" ref=\"Input\"/> \
      </xs:sequence> \
      <xs:attribute name=\"id\" use=\"required\" type=\"xs:NMTOKEN\"/> \
      <xs:attribute name=\"package\" use=\"required\"/> \
      <xs:attribute name=\"type\" use=\"required\"/> \
      <xs:attribute name=\"name\"/> \
      <xs:attribute name=\"version\" use=\"required\"> \
        <xs:simpleType> \
          <xs:restriction base=\"xs:string\"> \
           <xs:pattern value=\"[0-9]+.[0-9]+.[0-9]+\"/> \
          </xs:restriction> \
        </xs:simpleType> \
      </xs:attribute> \
      <xs:attribute name=\"isInitialized\" type=\"xs:boolean\"/> \
      <xs:attribute name=\"x\" type=\"xs:decimal\"/> \
      <xs:attribute name=\"y\" type=\"xs:decimal\"/> \
    </xs:complexType> \
  </xs:element> \
  <xs:element name=\"Parameter\"> \
    <xs:complexType> \
      <xs:sequence> \
        <xs:element minOccurs=\"0\" maxOccurs=\"1\" ref=\"Data\"/> \
      </xs:sequence> \
          <xs:attribute name=\"id\" use=\"required\" type=\"xs:NMTOKEN\"/> \
          <xs:attribute name=\"title\"/> \
          <xs:attribute name=\"behavior\"> \
            <xs:simpleType> \
            <xs:restriction base=\"xs:string\"> \
              <xs:enumeration value=\"persistent\" /> \
              <xs:enumeration value=\"push\" /> \
              <xs:enumeration value=\"pull\" /> \
            </xs:restriction> \
            </xs:simpleType> \
          </xs:attribute> \
    </xs:complexType> \
  </xs:element> \
  <xs:element name=\"Data\"> \
    <xs:complexType mixed=\"true\"> \
      <xs:attribute name=\"type\" use=\"required\"/> \
      <xs:attribute name=\"package\" use=\"required\"/> \
      <xs:attribute name=\"version\" use=\"required\"> \
        <xs:simpleType> \
          <xs:restriction base=\"xs:string\"> \
           <xs:pattern value=\"[0-9]+.[0-9]+.[0-9]+\"/> \
          </xs:restriction> \
        </xs:simpleType> \
      </xs:attribute> \
      <xs:attribute name=\"file\"/> \
    </xs:complexType> \
  </xs:element> \
  <xs:element name=\"Input\"> \
    <xs:complexType> \
      <xs:attribute name=\"id\" use=\"required\" type=\"xs:NMTOKEN\"/> \
      <xs:attribute name=\"title\"/> \
      <xs:attribute name=\"operator\" use=\"required\" type=\"xs:NMTOKEN\"/> \
      <xs:attribute name=\"output\" use=\"required\" type=\"xs:NMTOKEN\"/> \
      <xs:attribute name=\"outputTitle\"/> \
    </xs:complexType> \
  </xs:element> \
  <xs:element name=\"InputConnector\"> \
    <xs:complexType> \
      <xs:attribute name=\"operator\" use=\"required\" type=\"xs:NMTOKEN\"/> \
      <xs:attribute name=\"input\" use=\"required\" type=\"xs:NMTOKEN\"/> \
      <xs:attribute name=\"inputTitle\"/> \
    </xs:complexType> \
  </xs:element> \
</xs:schema> \
"
                ;
            
                class XercesErrorHandler : public HandlerBase
                {
        public:
                    explicit XercesErrorHandler(const std::string & filename) : m_filename(filename) {}
                    
                    virtual void error (const SAXParseException &/*exc*/)
                    {
                        throw InvalidFileFormat(m_filename, "XML file is not valid.");
                    }
                    
                private:
                    std::string m_filename;
                };
                
                class EntityResolver : public XMLEntityResolver
                {
        public:
                    virtual InputSource* resolveEntity (XMLResourceIdentifier* /*resourceIdentifier*/)
                    {
                        InputSource* grammar = new MemBufInputSource(reinterpret_cast<const XMLByte*>(XSD.c_str()), XSD.size(), "");
                        return grammar;
                    }
                };
            }
            
            XmlReaderImpl::XmlReaderImpl(const AbstractFactory* factory)
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
                XercesErrorHandler errHandler(filename);
                EntityResolver entityResolver;
                XercesDOMParser parser;

                parser.setErrorHandler(&errHandler);
                parser.setXMLEntityResolver(&entityResolver);
                parser.setDoNamespaces(true);
                parser.setDoSchema(true);
                parser.setExternalNoNamespaceSchemaLocation("stromx.xsd");
                parser.setValidationScheme(XercesDOMParser::Val_Always);

                std::stringbuf contentBuffer;
                m_input = &input;
                m_input->initialize("", filename);
                m_input->openFile(FileInput::TEXT).get(contentBuffer, 0);
                
                std::string content = contentBuffer.str();
                
                MemBufInputSource source(reinterpret_cast<const XMLByte*>(content.c_str()), content.size(), (XMLCh*)(0));

                try
                {
                    parser.parse(source);
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
                catch (const SAXException& toCatch)
                {
                    char* message = XMLString::transcode(toCatch.getMessage());
                    
                    InvalidFileFormat ex(filename, "", "SAX exception: " + std::string(message));
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
                    m_stream->setFactory(m_factory);
                    
                    DOMDocument* doc = parser.getDocument();
                    
                    if(! doc)
                        throw FileAccessFailed(filename, "Failed to read file.");
                    
                    DOMNodeList* streamNodes = doc->getElementsByTagName(Str2Xml("Stream"));
                    
                    if(! streamNodes->getLength())
                        throw FileAccessFailed(filename, "Found no element <Stream/>.");
                    
                    if(streamNodes->item(0)->getNodeType() != DOMNode::ELEMENT_NODE)
                        throw FileAccessFailed(filename, "Found no element <Stream/>.");
                    
                    DOMElement* stream = reinterpret_cast<DOMElement*>(streamNodes->item(0));
                    
                    Xml2Str name(stream->getAttribute(Str2Xml("name")));
                    m_stream->setName(std::string(name));
                    
                    DOMNodeList* operators = stream->getElementsByTagName(Str2Xml("Operator"));
                    XMLSize_t numOperators = operators->getLength();
                    
                    // read the operators
                    for(unsigned int i = 0; i < numOperators; ++i)
                    {
                        if(operators->item(i)->getNodeType() == DOMElement::ELEMENT_NODE)
                        {
                            DOMElement* op = reinterpret_cast<DOMElement*>(operators->item(i));
                            readOperator(op);
                        }
                    }
                    
                    // read the inputs of each operator
                    for(unsigned int i = 0; i < numOperators; ++i)
                    {
                        if(operators->item(i)->getNodeType() == DOMNode::ELEMENT_NODE)
                        {
                            DOMElement* op = reinterpret_cast<DOMElement*>(operators->item(i));
                            readOperatorInputs(op);
                        }
                    }
                    
                    DOMNodeList* threads = stream->getElementsByTagName(Str2Xml("Thread"));
                    XMLSize_t numThreads = threads->getLength();
                    
                    for(unsigned int i = 0; i < numThreads; ++i)
                    {
                        if(threads->item(i)->getNodeType() == DOMNode::ELEMENT_NODE)
                        {
                            DOMElement* threadElement = reinterpret_cast<DOMElement*>(threads->item(i));
                            Thread* thread = m_stream->addThread();
                            readThread(threadElement, thread);
                        }
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
                catch(runtime::Exception&)
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
            
            void XmlReaderImpl::readParameters(FileInput& input, const std::string & filename, const std::vector< stromx::runtime::Operator* > & operators)
            {
                XercesErrorHandler errHandler(filename);
                EntityResolver entityResolver;
                XercesDOMParser parser;

                parser.setErrorHandler(&errHandler);
                parser.setXMLEntityResolver(&entityResolver);
                parser.setDoNamespaces(true);
                parser.setDoSchema(true);
                parser.setExternalNoNamespaceSchemaLocation("stromx.xsd");
                parser.setValidationScheme(XercesDOMParser::Val_Always);

                std::stringbuf contentBuffer;
                m_input = &input;
                m_input->initialize("", filename);
                m_input->openFile(FileInput::TEXT).get(contentBuffer, 0);
                
                std::string content = contentBuffer.str();
                
                MemBufInputSource source(reinterpret_cast<const XMLByte*>(content.c_str()), content.size(), (XMLCh*)(0));

                try
                {
                    parser.parse(source);
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
                catch (const SAXException& toCatch)
                {
                    char* message = XMLString::transcode(toCatch.getMessage());
                    
                    InvalidFileFormat ex(filename, "", "SAX exception: " + std::string(message));
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
                    DOMDocument* doc = parser.getDocument();
                    
                    if(! doc)
                        throw FileAccessFailed(filename, "Failed to read file.");
                    
                    DOMNodeList* parameterNodes = doc->getElementsByTagName(Str2Xml("Parameters"));
                    
                    if(! parameterNodes->getLength())
                        throw FileAccessFailed(filename, "Found no element <Parameters/>.");
                    
                    if(parameterNodes->item(0)->getNodeType() != DOMElement::ELEMENT_NODE)
                        throw FileAccessFailed(filename, "Found no element <Parameters/>.");
                    
                    DOMElement* parametersItem = reinterpret_cast<DOMElement*>(parameterNodes->item(0));
                    
                    DOMNodeList* operatorNodes = parametersItem->getElementsByTagName(Str2Xml("Operator"));
                    XMLSize_t numOperators = operatorNodes->getLength();
                    
                    if(numOperators != operators.size())
                        throw FileAccessFailed(filename, "The number or <Operator/> tags does not match the number of input operators.");
                    
                    // read the operators
                    for(unsigned int i = 0; i < numOperators; ++i)
                    {
                        if(operatorNodes->item(i)->getNodeType() != DOMElement::ELEMENT_NODE)
                            continue;
                        
                        DOMElement* opElement = reinterpret_cast<DOMElement*>(operatorNodes->item(i));
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
                            if(parameters->item(i)->getNodeType() == DOMElement::ELEMENT_NODE)
                            {
                                DOMElement* paramElement = reinterpret_cast<DOMElement*>(parameters->item(i));
                                readParameter(paramElement);
                            }
                        }
                    
                        // set parameters
                        for(std::map<unsigned int, stromx::runtime::Data*>::const_iterator iter = m_id2DataMap.begin();
                            iter != m_id2DataMap.end();
                            ++iter)
                        {
                            try
                            {
                                op->setParameter(iter->first, *(iter->second));
                            }
                            catch(stromx::runtime::OperatorError&)
                            {
                                // ignore exceptions
                            }
                            
                            delete iter->second;
                        }
                        
                        // clear the current parameters
                        m_id2DataMap.clear();
                        m_id2BehaviorMap.clear();
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
                catch(runtime::Exception&)
                {
                    throw;
                }
                catch(boost::bad_lexical_cast & e)
                {
                    throw FileAccessFailed(filename, "", e.what());
                }
            }
            
            void XmlReaderImpl::readOperator(DOMElement*const opElement)
            {
                Xml2Str name(opElement->getAttribute(Str2Xml("name")));
                Xml2Str idStr(opElement->getAttribute(Str2Xml("id")));
                Xml2Str type(opElement->getAttribute(Str2Xml("type")));
                Xml2Str package(opElement->getAttribute(Str2Xml("package")));
                Xml2Str isInitialized(opElement->getAttribute(Str2Xml("isInitialized")));
                Xml2Str x(opElement->getAttribute(Str2Xml("x")));
                Xml2Str y(opElement->getAttribute(Str2Xml("y")));
                
                unsigned int id = boost::lexical_cast<unsigned int>((const char*)(idStr));
                
                if(m_id2OperatorMap.count(id))
                    throw XmlError("Multiple operators with the same ID.");
                
                OperatorKernel* opKernel = m_factory->newOperator(std::string(package), std::string(type));
                
                // add the kernel to the stream
                Operator* op = m_stream->addOperator(opKernel);
                
                op->setName(std::string(name));
                
                std::string xStr(x);
                std::string yStr(y);
                xStr = xStr.empty() ? "0" : xStr;
                yStr = yStr.empty() ? "0" : yStr;
                Position position(boost::lexical_cast<float>(xStr),
                                  boost::lexical_cast<float>(yStr));
                op->setPosition(position);
                
                m_id2OperatorMap[id] = op;
                m_id2DataMap.clear();
                m_id2BehaviorMap.clear();
                
                DOMNodeList* parameters = opElement->getElementsByTagName(Str2Xml("Parameter"));
                XMLSize_t numParameters = parameters->getLength();
                
                // read the parameters
                for(unsigned int i = 0; i < numParameters; ++i)
                {
                    if(parameters->item(i)->getNodeType() == DOMElement::ELEMENT_NODE)
                    {
                        DOMElement* paramElement = reinterpret_cast<DOMElement*>(parameters->item(i));
                        readParameter(paramElement);
                    }
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
                
                // if necessary initialize the operator and set the remaining parameters
                if (std::string(isInitialized) != "false" && std::string(isInitialized) != "0")
                {
                    m_stream->initializeOperator(op);
                    
                    // set the type of the connectors
                    for (std::map<unsigned int, Description::UpdateBehavior>::const_iterator iter = m_id2BehaviorMap.begin();
                         iter != m_id2BehaviorMap.end(); ++iter)
                    {
                        Description::Type originalType = op->info().description(iter->first).originalType();
                        if (originalType == Description::PARAMETER)
                            continue;
                        
                        m_stream->setConnectorType(op, iter->first, Description::PARAMETER, iter->second);
                    }
                
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
                }
                
                if(! m_id2DataMap.empty())
                    throw XmlError("Not all parameters of operator '" + op->name() + "' could be set.");
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
                    if(inputs->item(i)->getNodeType() == DOMElement::ELEMENT_NODE)
                    {
                        DOMElement* inputElement = reinterpret_cast<DOMElement*>(inputs->item(i));
                        readInput(inputElement, m_id2OperatorMap[id]);
                    }
                }
            }
            
            void XmlReaderImpl::readParameter(DOMElement*const paramElement)
            {
                Xml2Str idStr(paramElement->getAttribute(Str2Xml("id")));
                Xml2Str behaviorStr(paramElement->getAttribute(Str2Xml("behavior")));
                
                unsigned int id = boost::lexical_cast<unsigned int>((const char*)(idStr));
                Description::UpdateBehavior behavior = Description::PERSISTENT;
                if (std::string(behaviorStr) == "pull")
                    behavior = Description::PULL;
                else if (std::string(behaviorStr) == "push")
                    behavior = Description::PUSH;
                    
                if(m_id2BehaviorMap.count(id))
                    throw XmlError("Multiple parameters with the same ID " + boost::lexical_cast<std::string>(id) + ".");
                m_id2BehaviorMap[id] = behavior;
                
                DOMNodeList* dataElements = paramElement->getElementsByTagName(Str2Xml("Data"));
                XMLSize_t numDataElements = dataElements->getLength();
                
                if(numDataElements > 1)
                    throw XmlError("More than one <Data/> elements for parameter.");
                    
                if(! numDataElements)
                    return;
                    
                if(dataElements->item(0)->getNodeType() != DOMElement::ELEMENT_NODE)
                    throw XmlError("No <Data/> element for parameter.");
                    
                DOMElement* dataElement = reinterpret_cast<DOMElement*>(dataElements->item(0));
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
                
                Version version = boost::lexical_cast<Version>(versionString);
                
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
                
                Data* data = m_factory->newData(std::string(package), std::string(type));
                
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
                    std::string package = data->package();
                    std::string type = data->type();
                    delete data;
                    throw DeserializationError(package, type, e.what());
                }
                
                return data;
            }
            
            void XmlReaderImpl::readThread(DOMElement*const threadElement, Thread*const thread)
            {
                Xml2Str name(threadElement->getAttribute(Str2Xml("name")));
                thread->setName(std::string(name));
                
                std::string colorStr(Xml2Str(threadElement->getAttribute(Str2Xml("color"))));
                if (colorStr != "")
                {
                    Color color = boost::lexical_cast<Color>(colorStr);
                    thread->setColor(color);
                }
                
                DOMNodeList* inputs = threadElement->getElementsByTagName(Str2Xml("InputConnector"));
                XMLSize_t numInputs = inputs->getLength();
                
                for(unsigned int i = 0; i < numInputs; ++i)
                {
                    if(inputs->item(i)->getNodeType() == DOMElement::ELEMENT_NODE)
                    {
                        DOMElement* inputElement = reinterpret_cast<DOMElement*>(inputs->item(i));
                        readInputConnector(inputElement, thread);
                    }
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
