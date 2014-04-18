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
        : OperatorKernel(TYPE, PACKAGE, VERSION, setupInputs(), setupOutputs(), setupParameters())
        {
        }

        void GpioTrigger::setParameter(unsigned int id, const Data&)
        {
            throw WrongParameterId(id, *this);
        }

        const DataRef GpioTrigger::getParameter(const unsigned int id) const
        {
            throw WrongParameterId(id, *this);
        } 
        
        void GpioTrigger::activate()
        {
            if (impl::GPIOExport(static_cast<int>(m_gpio)))
            {
                throw OperatorError(*this, 
                    (boost::format("Failed to export GPIO %1%") % m_gpio).str());
            }
                                    
            if (impl::GPIODirection(static_cast<int>(m_gpio), impl::OUT))
            {
                impl::GPIOUnexport(static_cast<int>(m_gpio));
                throw OperatorError(*this, 
                    (boost::format("Failed to set direction of GPIO %1%") % m_gpio).str());
            }
        }
        
        void GpioTrigger::deactivate()
        {
            if (impl::GPIOUnexport(static_cast<int>(m_gpio)))
            {
                throw OperatorError(*this, 
                    (boost::format("Failed to unexport GPIO %1%") % m_gpio).str());
            }
        }
        
        void GpioTrigger::execute(DataProvider& provider)
        {
            int value = impl::GPIORead(static_cast<int>(m_gpio));
            
            if (-1 == value)
            {
                throw OperatorError(*this, 
                    (boost::format("Failed to read from GPIO %1%") % m_gpio).str());
            }
            
            DataContainer data(new Bool(value));
            Id2DataPair output(OUTPUT, data);
            provider.sendOutputData(output);
        }
        
        const std::vector<const Description*> GpioTrigger::setupInputs()
        {
            std::vector<const Description*> inputs;

            return inputs;
        }
        
        const std::vector<const Description*> GpioTrigger::setupOutputs()
        {
            std::vector<const Description*> outputs;
            
            Description* output = new Description(OUTPUT, DataVariant::BOOL);
            output->setTitle("Output");
            outputs.push_back(output);
            
            return outputs;
        }
        
        const std::vector<const Parameter*> GpioTrigger::setupParameters()
        {
            std::vector<const Parameter*> parameters;
            
            EnumParameter* gpio = impl::createGpioParameter(GPIO);
            parameters.push_back(gpio);
            
            return parameters;
        }
    } 
}
