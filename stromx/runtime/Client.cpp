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

#include "stromx/runtime/Client.h"

#include "stromx/runtime/DataProvider.h"
#include "stromx/runtime/Id2DataComposite.h"
#include "stromx/runtime/Id2DataPair.h"
#include "stromx/runtime/NumericParameter.h"
#include "stromx/runtime/OperatorException.h"

namespace stromx
{
    namespace runtime
    {
        const std::string Client::TYPE("Client");
        const std::string Client::PACKAGE(STROMX_RUNTIME_PACKAGE_NAME);
        const Version Client::VERSION(0, 1, 0);
        const unsigned int Client::MIN_PORT = 49152;
        const unsigned int Client::MAX_PORT = 65535;
        
        Client::Client()
          : OperatorKernel(TYPE, PACKAGE, VERSION, setupParameters()),
            m_port(MIN_PORT)
        {
        }
        
        void Client::setParameter(unsigned int id, const runtime::Data& value)
        {
            UInt32 uintValue;
            
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

        const DataRef Client::getParameter(const unsigned int id) const
        {
            switch(id)
            {
            case PORT:
                return m_port;
            default:
                throw WrongParameterId(id, *this);
            }
        }
        
        void Client::initialize()
        {
            OperatorKernel::initialize(setupInputs(),
                                       setupOutputs(),
                                       std::vector<const Parameter*>());
        }
        
        void Client::execute(DataProvider& provider)
        {
            
        }
        
        const std::vector<const Description*> Client::setupInputs()
        {
            std::vector<const Description*> inputs;
            return inputs;
        }
        
        const std::vector<const Description*> Client::setupOutputs() const
        {
            std::vector<const Description*> outputs;
            
            Description* ouput = new Description(OUTPUT, DataVariant::DATA);
            ouput->setTitle("Output");
            outputs.push_back(ouput);
            
            return outputs;
        }
        
        const std::vector<const Parameter*> Client::setupParameters()
        {
            std::vector<const runtime::Parameter*> parameters;
            
            Parameter* url = new Parameter(URL, DataVariant::STRING);
            url->setTitle("URL");
            url->setAccessMode(runtime::Parameter::INITIALIZED_WRITE);
            parameters.push_back(url);
            
            NumericParameter<UInt32>* port = new NumericParameter<UInt32>(PORT);
            port->setTitle("TCP port");
            port->setAccessMode(runtime::Parameter::INITIALIZED_WRITE);
            port->setMin(UInt32(MIN_PORT));
            port->setMax(UInt32(MAX_PORT));
            parameters.push_back(port);
            
            return parameters;
        }
    }
}
