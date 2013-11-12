/* 
 *  Copyright 2013 Matthias Fuchs
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

#include "stromx/runtime/Receive.h"

#include <boost/array.hpp>
#include <boost/asio.hpp>
#include <boost/lexical_cast.hpp>

#include "stromx/runtime/AbstractFactory.h"
#include "stromx/runtime/DataProvider.h"
#include "stromx/runtime/Id2DataComposite.h"
#include "stromx/runtime/Id2DataPair.h"
#include "stromx/runtime/InputProvider.h"
#include "stromx/runtime/NumericParameter.h"
#include "stromx/runtime/OperatorException.h"

namespace
{
    class StreamInput : public stromx::runtime::InputProvider
    {            
    public:
        StreamInput(boost::asio::streambuf & textBuffer,
                    boost::asio::streambuf & fileBuffer)
          : m_textStream(&textBuffer),
            m_fileStream(&fileBuffer),
            m_hasFile(fileBuffer.size() != 0)
        {}
        
        std::istream & text()
        {
            return m_textStream;
        }
        
        bool hasFile() const
        {
            return m_hasFile;
        }
        
        std::istream & openFile(const OpenMode /*mode*/)
        {
            return m_fileStream;
        }
        
        std::istream & file()
        {
            return m_fileStream;
        }
        
    private:
        std::istream m_textStream;
        std::istream m_fileStream;
        bool m_hasFile;
    };
}

namespace stromx
{
    namespace runtime
    {
        const std::string Receive::LINE_DELIMITER("\r\n");
        const std::string Receive::TYPE("Receive");
        const std::string Receive::PACKAGE(STROMX_RUNTIME_PACKAGE_NAME);
        const Version Receive::VERSION(0, 1, 0);
        const unsigned int Receive::MIN_PORT = 49152;
        const unsigned int Receive::MAX_PORT = 65535;
        
        Receive::Receive()
          : OperatorKernel(TYPE, PACKAGE, VERSION, setupParameters()),
            m_url("localhost"),
            m_port(MIN_PORT)
        {
        }
        
        void Receive::setParameter(unsigned int id, const runtime::Data& value)
        {
            UInt16 uintValue;
            
            switch(id)
            {
            case URL:
                m_url = data_cast<String>(value);
                break;
            case PORT:
                uintValue = data_cast<UInt16>(value);
                if(uintValue < MIN_PORT)
                    throw WrongParameterValue(parameter(PORT), *this, "Too small port number.");
                if(uintValue > MAX_PORT)
                    throw WrongParameterValue(parameter(PORT), *this, "Too large port number.");
                m_port = uintValue;
                break;
            default:
                throw WrongParameterId(id, *this);
            }
        }

        const DataRef Receive::getParameter(const unsigned int id) const
        {
            switch(id)
            {
            case URL:
                return m_url;
            case PORT:
                return m_port;
            default:
                throw WrongParameterId(id, *this);
            }
        }
        
        void Receive::initialize()
        {
            OperatorKernel::initialize(setupInputs(),
                                       setupOutputs(),
                                       std::vector<const Parameter*>());
        }
        
        void Receive::execute(DataProvider& provider)
        {
            using boost::asio::ip::tcp;
            
            Data* outData = 0;
            
            try
            {
                std::string portString = boost::lexical_cast<std::string>(m_port);
                boost::asio::io_service io_service;

                tcp::resolver resolver(io_service);
                tcp::resolver::query query(m_url, portString);
                tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);

                tcp::socket socket(io_service);
                boost::asio::connect(socket, endpoint_iterator);

                boost::asio::streambuf buf;
                for (int i = 0; i < 5; ++i)
                    boost::asio::read_until(socket, buf, LINE_DELIMITER);
                
                Version serverVersion;
                std::string package;
                std::string type;
                Version dataVersion;
                unsigned int textBufferSize = 0;
                unsigned int fileBufferSize = 0;
                std::istream stream(&buf);
                
                stream >> serverVersion;
                stream >> package;
                stream >> type;
                stream >> dataVersion;
                stream >> textBufferSize;
                stream >> fileBufferSize;
                
                boost::asio::streambuf textBuffer(textBufferSize);
                boost::asio::streambuf fileBuffer(fileBufferSize);
                
                boost::asio::read(socket, textBuffer);
                boost::asio::read(socket, fileBuffer);
                
                socket.close();
                
                StreamInput input(textBuffer, fileBuffer);
                
                outData = provider.factory().newData(package, type);
                outData->deserialize(input, dataVersion);
                Id2DataPair outputDataMapper(OUTPUT, DataContainer(outData));
                provider.sendOutputData(outputDataMapper);
            }
            catch (std::exception& e)
            {
                std::cerr << e.what() << std::endl;
                throw;
            }
        }
        
        const std::vector<const Description*> Receive::setupInputs()
        {
            std::vector<const Description*> inputs;
            return inputs;
        }
        
        const std::vector<const Description*> Receive::setupOutputs() const
        {
            std::vector<const Description*> outputs;
            
            Description* ouput = new Description(OUTPUT, DataVariant::DATA);
            ouput->setTitle("Output");
            outputs.push_back(ouput);
            
            return outputs;
        }
        
        const std::vector<const Parameter*> Receive::setupParameters()
        {
            std::vector<const runtime::Parameter*> parameters;
            
            Parameter* url = new Parameter(URL, DataVariant::STRING);
            url->setTitle("URL");
            url->setAccessMode(runtime::Parameter::INITIALIZED_WRITE);
            parameters.push_back(url);
            
            NumericParameter<UInt16>* port = new NumericParameter<UInt16>(PORT);
            port->setTitle("TCP port");
            port->setAccessMode(runtime::Parameter::INITIALIZED_WRITE);
            port->setMin(UInt16(MIN_PORT));
            port->setMax(UInt16(MAX_PORT));
            parameters.push_back(port);
            
            return parameters;
        }
    }
}
