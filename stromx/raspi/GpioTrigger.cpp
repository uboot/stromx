/* 
*  Copyright 2014 Matthias Fuchs
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

#include "stromx/raspi/GpioTrigger.h"

#include <boost/format.hpp>
#include <stromx/runtime/DataProvider.h>
#include <stromx/runtime/EnumParameter.h>
#include <stromx/runtime/Id2DataPair.h>
#include <stromx/runtime/OperatorException.h>
#include <stromx/runtime/TriggerData.h>
#include "stromx/raspi/impl/Gpio.h"
#include "stromx/raspi/impl/Utilities.h"

namespace stromx
{
    using namespace runtime;
    
    namespace raspi
    {
        const std::string GpioTrigger::TYPE("GpioTrigger");
        const std::string GpioTrigger::PACKAGE(STROMX_RASPI_PACKAGE_NAME);
        const Version GpioTrigger::VERSION(0, 1, 0);
        
        GpioTrigger::GpioTrigger()
        : OperatorKernel(TYPE, PACKAGE, VERSION, setupInputs(), setupOutputs(), setupParameters()),
          m_edge(impl::RISING),
          m_interruptReadSocket(0),
          m_interruptWriteSocket(0),
          m_gpioSocket(0)
        {
        }

        void GpioTrigger::setParameter(unsigned int id, const Data& data)
        {    
            try
            {
                switch(id)
                {
                case GPIO:
                    m_gpio = data_cast<runtime::Enum>(data);
                    break;
                case EDGE:
                    m_edge = data_cast<runtime::Enum>(data);
                    break;
                default:
                    throw WrongParameterId(id, *this);
                }
            }
            catch(std::bad_cast&)
            {
                throw WrongParameterType(parameter(id), *this);
            }
        }

        const DataRef GpioTrigger::getParameter(const unsigned int id) const
        {
            switch(id)
            {
            case GPIO:
                return m_gpio;
            case EDGE:
                return m_edge;
            default:
                throw WrongParameterId(id, *this);
            };
        } 
        
        void GpioTrigger::activate()
        {
            
            if (impl::GPIOExport(static_cast<int>(m_gpio)))
            {
                throw OperatorError(*this, 
                    (boost::format("Failed to export GPIO %1%. Consider running this process as root.") % m_gpio).str());
            }
                                    
            if (impl::GPIODirection(static_cast<int>(m_gpio), impl::IN))
            {
                impl::GPIOUnexport(static_cast<int>(m_gpio));
                throw OperatorError(*this, 
                    (boost::format("Failed to set direction of GPIO %1%.") % m_gpio).str());
            }
                                    
            if (impl::GPIOEdge(static_cast<int>(m_gpio), m_edge))
            {
                impl::GPIOUnexport(static_cast<int>(m_gpio));
                throw OperatorError(*this, 
                    (boost::format("Failed to set edge of GPIO %1%.") % m_gpio).str());
            }
                                    
            if (impl::GPIOOpen(static_cast<int>(m_gpio), m_gpioSocket))
            {
                impl::GPIOUnexport(static_cast<int>(m_gpio));
                throw OperatorError(*this, 
                    (boost::format("Failed to open GPIO %1%.") % m_gpio).str());
            }
                                    
            if (impl::GPIOCreatePipe(m_interruptReadSocket, m_interruptWriteSocket))
            {
                impl::GPIOCloseSocket(m_gpioSocket);
                impl::GPIOUnexport(static_cast<int>(m_gpio));
                throw OperatorError(*this, 
                    (boost::format("Failed to open GPIO %1%.") % m_gpio).str());
            }
        }
        
        void GpioTrigger::deactivate()
        {
            if (impl::GPIOCloseSocket(m_gpioSocket))
            {
                throw OperatorError(*this, 
                    (boost::format("Failed to close GPIO %1%.") % m_gpio).str());
            }
            
            if (impl::GPIOCloseSocket(m_interruptReadSocket))
            {
                throw OperatorError(*this, 
                    (boost::format("Failed to close read end of interrupt pipe for GPIO %1%.") % m_gpio).str());
            }
            
            if (impl::GPIOCloseSocket(m_interruptWriteSocket))
            {
                throw OperatorError(*this, 
                    (boost::format("Failed to close write end of interrupt pipe for GPIO %1%.") % m_gpio).str());
            }
            
            if (impl::GPIOUnexport(static_cast<int>(m_gpio)))
            {
                throw OperatorError(*this, 
                    (boost::format("Failed to unexport GPIO %1%.") % m_gpio).str());
            }
        }
        
        void GpioTrigger::execute(DataProvider& provider)
        {
            bool interrupt = false;
            int value = impl::GPIOPoll(m_gpioSocket, m_interruptReadSocket, interrupt);
            if (value < 0)
            {
                throw OperatorError(*this, 
                    (boost::format("Failed poll GPIO %1%.") % m_gpio).str());
            }
            
            if (interrupt)
                throw Interrupt();
            
            DataContainer data(new Bool(value));
            Id2DataPair output(OUTPUT, data);
            provider.sendOutputData(output);
        }
        
        void GpioTrigger::interrupt()
        {
            if (impl::GPIOSendInterrupt(m_interruptWriteSocket))
            {
                throw OperatorError(*this, 
                    (boost::format("Failed interrupt polling for GPIO %1%.") % m_gpio).str());
            }            
        }
        
        const std::vector<const Description*> GpioTrigger::setupInputs()
        {
            std::vector<const Description*> inputs;

            return inputs;
        }
        
        const std::vector<const Description*> GpioTrigger::setupOutputs()
        {
            std::vector<const Description*> outputs;
            
            Description* output = new Description(OUTPUT, Variant::BOOL);
            output->setTitle("Output");
            outputs.push_back(output);
            
            return outputs;
        }
        
        const std::vector<const Parameter*> GpioTrigger::setupParameters()
        {
            std::vector<const Parameter*> parameters;
            
            EnumParameter* gpio = impl::createGpioParameter(GPIO);
            parameters.push_back(gpio);  
            
            EnumParameter* edge = new EnumParameter(EDGE);
            edge->setTitle("Edge");
            edge->setAccessMode(runtime::Parameter::INITIALIZED_WRITE);
            edge->add(EnumDescription(Enum(impl::RISING), "Rising"));
            edge->add(EnumDescription(Enum(impl::FALLING), "Falling"));
            edge->add(EnumDescription(Enum(impl::BOTH), "Both"));
            parameters.push_back(edge);
            
            return parameters;
        }
    } 
}
