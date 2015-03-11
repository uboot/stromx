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
#include <boost/assert.hpp>
#include <boost/asio.hpp>
#include <boost/lexical_cast.hpp>

#include "stromx/runtime/impl/Client.h"
#include "stromx/runtime/AbstractFactory.h"
#include "stromx/runtime/DataProvider.h"
#include "stromx/runtime/Id2DataComposite.h"
#include "stromx/runtime/Id2DataPair.h"
#include "stromx/runtime/InputProvider.h"
#include "stromx/runtime/NumericParameter.h"
#include "stromx/runtime/OperatorException.h"

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
            m_port(MIN_PORT),
            m_client(0)
        {
        }
        
        Receive::~Receive()
        {
            delete m_client;
        }

        void Receive::activate()
        {
            try
            {
                std::string portString = boost::lexical_cast<std::string>(uint16_t(m_port));
                m_client = new impl::Client(m_url, portString);
            }
            catch(impl::Client::NoConnection&)
            {
                throw OperatorError(*this, "Failed to connect to send operator.");
            }
        }
        
        void Receive::interrupt()
        {
            BOOST_ASSERT(m_client);
            m_client->stop();
        }

        void Receive::deactivate()
        {
            delete m_client;
            m_client = 0;
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
            DataContainer data;
            try
            {
                data = m_client->receive(provider.factory());
            }
            catch(impl::Client::NoConnection&)
            {
                throw OperatorError(*this, "Lost the connection to send operator.");
            }
            catch(impl::Client::Stopped&)
            {
                return;
            }
            
            Id2DataPair outputMapper(OUTPUT, data);
            provider.sendOutputData(outputMapper);
        }
        
        const std::vector<const Description*> Receive::setupInputs()
        {
            std::vector<const Description*> inputs;
            return inputs;
        }
        
        const std::vector<const Description*> Receive::setupOutputs() const
        {
            std::vector<const Description*> outputs;
            
            Description* ouput = new Description(OUTPUT, Variant::DATA);
            ouput->setTitle("Output");
            outputs.push_back(ouput);
            
            return outputs;
        }
        
        const std::vector<const Parameter*> Receive::setupParameters()
        {
            std::vector<const runtime::Parameter*> parameters;
            
            Parameter* url = new Parameter(URL, Variant::STRING);
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
