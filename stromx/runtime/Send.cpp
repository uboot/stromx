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

#include "stromx/runtime/Send.h"

#include "stromx/runtime/impl/Server.h"
#include "stromx/runtime/DataProvider.h"
#include "stromx/runtime/Id2DataComposite.h"
#include "stromx/runtime/Id2DataPair.h"
#include "stromx/runtime/NumericParameter.h"
#include "stromx/runtime/OperatorException.h"
#include "stromx/runtime/OutputProvider.h"
#include "stromx/runtime/ReadAccess.h"

namespace stromx
{
    namespace runtime
    {
        
        const std::string Send::LINE_DELIMITER("\r\n");
        const std::string Send::TYPE("Send");
        const std::string Send::PACKAGE(STROMX_RUNTIME_PACKAGE_NAME);
        const Version Send::VERSION(0, 1, 0);
        const unsigned int Send::MIN_PORT = 49152;
        const unsigned int Send::MAX_PORT = 65535;
        
        Send::Send()
          : OperatorKernel(TYPE, PACKAGE, VERSION, setupParameters(), setupProperties()),
            m_port(MIN_PORT),
            m_server(0)
        {
        }
        
        Send::~Send()
        {
            delete m_server;
        }
        
        void Send::setParameter(unsigned int id, const runtime::Data& value)
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

        const DataRef Send::getParameter(const unsigned int id) const
        {
            switch(id)
            {
            case PORT:
                return m_port;
            default:
                throw WrongParameterId(id, *this);
            }
        }
        
        void Send::initialize()
        {
            OperatorKernel::initialize(setupInputs(),
                                       setupOutputs(),
                                       std::vector<const Parameter*>());
        }
        
        void Send::activate()
        {
            try
            {
                m_server = new impl::Server(m_port);
            }
            catch(std::exception&)
            {
                throw OperatorError(*this, "Failed to start server.");
            }
            
        }

        void Send::deactivate()
        {
            m_server->stop();
            m_server->join();
            
            delete m_server;
            m_server = 0;
        }
        
        void Send::execute(DataProvider& provider)
        {
            Id2DataPair inputMapper(INPUT);
            provider.receiveInputData(inputMapper);
            
            m_server->send(inputMapper.data());
        }
        
        const std::vector<const Description*> Send::setupInputs()
        {
            std::vector<const Description*> inputs;
            Description* input = new Description(INPUT, Variant::DATA);
            input->setTitle("Input");
            inputs.push_back(input);
            
            return inputs;
        }
        
        const std::vector<const Description*> Send::setupOutputs()
        {
            std::vector<const Description*> outputs;
            return outputs;
        }
        
        const std::vector<const Parameter*> Send::setupParameters()
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
        
        const OperatorProperties Send::setupProperties()
        {
            OperatorProperties properties;
            properties.isGreedy = true;
            return properties;
        }
    }
}
