/* 
 *  Copyright 2012 Matthias Fuchs
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

#include "stromx/runtime/Server.h"

#include <boost/asio.hpp>
#include <boost/lexical_cast.hpp>

#include "stromx/runtime/DataProvider.h"
#include "stromx/runtime/Id2DataComposite.h"
#include "stromx/runtime/Id2DataPair.h"
#include "stromx/runtime/NumericParameter.h"
#include "stromx/runtime/OperatorException.h"
#include "stromx/runtime/OutputProvider.h"
#include "stromx/runtime/ReadAccess.h"

namespace
{
    class StreamOutput : public stromx::runtime::OutputProvider
    {
    public:
        StreamOutput()
          : m_textStream(&m_textBuffer),
            m_fileStream(&m_fileBuffer)
        {}
        
        std::ostream & text ()
        {
            return m_textStream;
        }
        
        std::ostream & openFile (const std::string &/*ext*/, const OpenMode /*mode*/)
        {
            return m_fileStream;
        }
        
        std::ostream & file ()
        {
            return m_fileStream;
        }
        
        boost::asio::streambuf & textBuffer() { return m_textBuffer; }
        boost::asio::streambuf & fileBuffer() { return m_fileBuffer; }
        
    private:
        boost::asio::streambuf m_textBuffer;
        boost::asio::streambuf m_fileBuffer;
        
        std::ostream m_textStream;
        std::ostream m_fileStream;
    };
}


namespace stromx
{
    namespace runtime
    {
        
        const std::string Server::LINE_DELIMITER("\r\n");
        const std::string Server::TYPE("Server");
        const std::string Server::PACKAGE(STROMX_RUNTIME_PACKAGE_NAME);
        const Version Server::VERSION(0, 1, 0);
        const unsigned int Server::MIN_PORT = 49152;
        const unsigned int Server::MAX_PORT = 65535;
        
        Server::Server()
          : OperatorKernel(TYPE, PACKAGE, VERSION, setupParameters()),
            m_port(MIN_PORT)
        {
        }
        
        void Server::setParameter(unsigned int id, const runtime::Data& value)
        {
            UInt16 uintValue;
            
            switch(id)
            {
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

        const DataRef Server::getParameter(const unsigned int id) const
        {
            switch(id)
            {
            case PORT:
                return m_port;
            default:
                throw WrongParameterId(id, *this);
            }
        }
        
        void Server::initialize()
        {
            OperatorKernel::initialize(setupInputs(),
                                       setupOutputs(),
                                       std::vector<const Parameter*>());
        }
        
        void Server::execute(DataProvider& provider)
        {
            using boost::asio::ip::tcp;
            
            Id2DataPair input(INPUT);
            provider.receiveInputData(input);
            ReadAccess<> access(input.data());
            
            try
            {
                boost::asio::io_service io_service;

                tcp::acceptor acceptor(io_service, tcp::endpoint(tcp::v4(), m_port));
                tcp::socket socket(io_service);
                acceptor.accept(socket);
                
                StreamOutput output;
                access().serialize(output);
                
                boost::asio::streambuf data;
                std::ostream dataStream(&data);
                dataStream << VERSION << LINE_DELIMITER;
                dataStream << access().package() << LINE_DELIMITER;
                dataStream << access().type() << LINE_DELIMITER;
                dataStream << access().version() << LINE_DELIMITER;
                dataStream << output.textBuffer().size() << LINE_DELIMITER;
                dataStream << output.fileBuffer().size() << LINE_DELIMITER;

                boost::asio::write(socket, data);
                boost::asio::write(socket, output.textBuffer());
                boost::asio::write(socket, output.fileBuffer());
            }
            catch (std::exception& e)
            {
                std::cerr << e.what() << std::endl;
                throw;
            }
        }
        
        const std::vector<const Description*> Server::setupInputs()
        {
            std::vector<const Description*> inputs;
            Description* input = new Description(INPUT, DataVariant::DATA);
            input->setTitle("Input");
            inputs.push_back(input);
            
            return inputs;
        }
        
        const std::vector<const Description*> Server::setupOutputs() const
        {
            std::vector<const Description*> outputs;
            return outputs;
        }
        
        const std::vector<const Parameter*> Server::setupParameters()
        {
            std::vector<const runtime::Parameter*> parameters;
            
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
