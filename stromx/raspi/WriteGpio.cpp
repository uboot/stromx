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

#include "stromx/raspi/WriteGpio.h"

#include <stromx/runtime/DataProvider.h>
#include <stromx/runtime/EnumParameter.h>
#include <stromx/runtime/Id2DataPair.h>
#include <stromx/runtime/OperatorException.h>
#include <stromx/runtime/ReadAccess.h>
#include "stromx/raspi/impl/Gpio.h"

namespace stromx
{
    using namespace runtime;
    
    namespace raspi
    {
        const std::string WriteGpio::TYPE("WriteGpio");
        const std::string WriteGpio::PACKAGE(STROMX_RASPI_PACKAGE_NAME);
        const Version WriteGpio::VERSION(0, 1, 0);
        
        WriteGpio::WriteGpio()
        : OperatorKernel(TYPE, PACKAGE, VERSION, setupInputs(), setupOutputs(), setupParameters())
        {
        }

        void WriteGpio::setParameter(unsigned int id, const Data& data)
        {            
            try
            {
                switch(id)
                {
                case GPIO:
                {
                    m_gpio = data_cast<runtime::Enum>(data);
                    break;
                }
                default:
                    throw WrongParameterId(id, *this);
                }
            }
            catch(std::bad_cast&)
            {
                throw WrongParameterType(parameter(id), *this);
            }
        }

        const DataRef WriteGpio::getParameter(const unsigned int id) const
        {
            switch(id)
            {
            case GPIO:
                return m_gpio;
            default:
                throw WrongParameterId(id, *this);
            }
        } 
        
        void WriteGpio::activate()
        {
            impl::GPIOExport(static_cast<int>(m_gpio));
            impl::GPIODirection(static_cast<int>(m_gpio), impl::OUT);
        }
        
        void WriteGpio::deactivate()
        {
            impl::GPIOUnexport(static_cast<int>(m_gpio));
        }
        
        void WriteGpio::execute(DataProvider& provider)
        {
            runtime::Id2DataPair input(INPUT);
            provider.receiveInputData(input);
            
            runtime::ReadAccess<runtime::Bool> access(input.data());
            int value = access();
            
            impl::GPIOWrite(static_cast<int>(m_gpio), value);
        }
        
        const std::vector<const Description*> WriteGpio::setupInputs()
        {
            std::vector<const Description*> inputs;
            
            Description* input = new Description(INPUT, DataVariant::BOOL);
            input->setTitle("Input");
            inputs.push_back(input);

            return inputs;
        }
        
        const std::vector<const Description*> WriteGpio::setupOutputs()
        {
            std::vector<const Description*> outputs;;
            
            return outputs;
        }
        
        const std::vector<const Parameter*> WriteGpio::setupParameters()
        {
            std::vector<const Parameter*> parameters;
            
            EnumParameter* gpio = new EnumParameter(GPIO);
            gpio->setTitle("GPIO");
            gpio->setAccessMode(runtime::Parameter::INITIALIZED_WRITE);
            gpio->add(EnumDescription(Enum(0), "0"));
            gpio->add(EnumDescription(Enum(1), "1"));
            gpio->add(EnumDescription(Enum(2), "2"));
            gpio->add(EnumDescription(Enum(3), "3"));
            gpio->add(EnumDescription(Enum(4), "4"));
            gpio->add(EnumDescription(Enum(7), "7"));
            gpio->add(EnumDescription(Enum(8), "8"));
            gpio->add(EnumDescription(Enum(9), "9"));
            gpio->add(EnumDescription(Enum(10), "10"));
            gpio->add(EnumDescription(Enum(14), "14"));
            gpio->add(EnumDescription(Enum(15), "15"));
            gpio->add(EnumDescription(Enum(17), "17"));
            gpio->add(EnumDescription(Enum(18), "18"));
            gpio->add(EnumDescription(Enum(21), "21"));
            gpio->add(EnumDescription(Enum(22), "22"));
            gpio->add(EnumDescription(Enum(23), "23"));
            gpio->add(EnumDescription(Enum(24), "24"));
            gpio->add(EnumDescription(Enum(25), "25"));
            gpio->add(EnumDescription(Enum(27), "27"));
            parameters.push_back(gpio);
            
            return parameters;
        }
        
        const OperatorProperties WriteGpio::setupProperties()
        {
            OperatorProperties properties;
            properties.isGreedy = true;
            return properties;
        }
    } 
}
