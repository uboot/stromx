/* 
*  Copyright 2013 Matthias Fuchs
*
*  This file is part of stromx-studio.
*
*  Stromx-studio is free software: you can redistribute it and/or modify
*  it under the terms of the GNU General Public License as published by
*  the Free Software Foundation, either version 3 of the License, or
*  (at your option) any later version.
*
*  Stromx-studio is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*  GNU General Public License for more details.
*
*  You should have received a copy of the GNU General Public License
*  along with stromx-studio.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "stromx/test/ExceptionOperator.h"

#include <stromx/runtime/DataProvider.h>
#include <stromx/runtime/Id2DataPair.h>
#include <stromx/runtime/OperatorException.h>
#include <stromx/runtime/Primitive.h>

using namespace stromx::runtime;

namespace stromx
{
    namespace test
    {
        const std::string ExceptionOperator::TYPE("ExceptionOperator");
        const std::string ExceptionOperator::PACKAGE(STROMX_TEST_PACKAGE_NAME);
        const Version ExceptionOperator::VERSION(1, 2, 3);

        ExceptionOperator::ExceptionOperator()
        : OperatorKernel(TYPE, PACKAGE, VERSION,
                        setupInputs(), setupOutputs(),
                        setupParameters())
        {
        }

        void ExceptionOperator::setParameter(unsigned int id, const Data& value)
        {
            try
            {
                switch(id)
                {
                case THROW_INITIALIZE:
                    m_throwInitialize = data_cast<Bool>(value);
                    break;
                case THROW_ACTIVATE:
                    m_throwActivate = data_cast<Bool>(value);
                    break;
                case THROW_EXECUTE:
                    m_throwExecute = data_cast<Bool>(value);
                    break;
                case THROW_DEACTIVATE:
                    m_throwDeactivate = data_cast<Bool>(value);
                    break;
                case THROW_DEINITIALIZE:
                    m_throwDeinitialize = data_cast<Bool>(value);
                    break;
                case THROW_PARAMETER:
                    m_throwParameter = data_cast<Bool>(value);
                    break;
                case PARAMETER:
                    if (m_throwParameter)
                        throw WrongParameterId(id, *this);
                    else
                        m_parameter = data_cast<Int32>(value);
                    break;
                default:
                    throw WrongParameterId(id, *this);
                }
            }
            catch(std::bad_cast&)
            {
                throw WrongParameterType(*parameters()[id], *this);
            }
        }

        const DataRef ExceptionOperator::getParameter(const unsigned int id) const
        {
            switch(id)
            {
            case THROW_INITIALIZE:
                return m_throwInitialize;
            case THROW_ACTIVATE:
                return m_throwActivate;
            case THROW_EXECUTE:
                return m_throwExecute;
            case THROW_DEACTIVATE:
                return m_throwDeactivate;
            case THROW_DEINITIALIZE:
                return m_throwDeinitialize;
            case THROW_PARAMETER:
                return m_throwParameter;
            case PARAMETER:
                if (m_throwParameter)
                    throw WrongParameterId(id, *this);
                else
                    return m_parameter;
            default:
                throw WrongParameterId(id, *this);
            }
        }  

        void ExceptionOperator::initialize()
        {
            if(m_throwInitialize)
                throw OperatorError(*this, "Failed to initialize operator.");
        }

        void ExceptionOperator::activate()
        {
            if(m_throwActivate)
                throw OperatorError(*this, "Failed to activate operator.");
        }  

        void ExceptionOperator::deactivate()
        {
            if(m_throwDeactivate)
                throw OperatorError(*this, "Failed to deactivate operator.");
        }

        void ExceptionOperator::deinitialize()
        {
            if(m_throwDeinitialize)
                throw OperatorError(*this, "Failed to deinitialize operator.");
        }

        void ExceptionOperator::execute(DataProvider& provider)
        {
            Id2DataPair input(INPUT);
            
            provider.receiveInputData(input);
            
            provider.sleep(100);
            
            if(m_throwExecute)
                throw OperatorError(*this, "Failed to execute operator.");
        }
                
        const std::vector<const Description*> ExceptionOperator::setupInputs()
        {
            std::vector<const Description*> inputs;
            Description* description = 0;
            
            description = new Description(INPUT, Variant::NONE);
            description->setTitle("Input");
            inputs.push_back(description);
            
            return inputs;
        }

        const std::vector<const Description*> ExceptionOperator::setupOutputs()
        {
            std::vector<const Description*> outputs;
            
            return outputs;
        }

        const std::vector<const Parameter*> ExceptionOperator::setupParameters()
        {
            std::vector<const Parameter*> parameters;
            
            Parameter* param = new Parameter(THROW_INITIALIZE, Variant::BOOL);
            param->setTitle("Exception on initialize");
            param->setAccessMode(Parameter::NONE_WRITE);
            parameters.push_back(param);
            
            param = new Parameter(THROW_ACTIVATE, Variant::BOOL);
            param->setTitle("Exception on activate");
            param->setAccessMode(Parameter::INITIALIZED_WRITE);
            parameters.push_back(param);
            
            param = new Parameter(THROW_EXECUTE, Variant::BOOL);
            param->setTitle("Exception on execute");
            param->setAccessMode(Parameter::ACTIVATED_WRITE);
            parameters.push_back(param);
            
            param = new Parameter(THROW_DEACTIVATE, Variant::BOOL);
            param->setTitle("Exception on deactivate");
            param->setAccessMode(Parameter::INITIALIZED_WRITE);
            parameters.push_back(param);
            
            param = new Parameter(THROW_DEINITIALIZE, Variant::BOOL);
            param->setTitle("Exception on deinitialize");
            param->setAccessMode(Parameter::INITIALIZED_WRITE);
            parameters.push_back(param);
            
            param = new Parameter(THROW_PARAMETER, Variant::BOOL);
            param->setTitle("Exception on parameter access");
            param->setAccessMode(Parameter::INITIALIZED_WRITE);
            parameters.push_back(param);
            
            param = new Parameter(PARAMETER, Variant::INT_32);
            param->setTitle("Integer parameter");
            param->setAccessMode(Parameter::INITIALIZED_WRITE);
            parameters.push_back(param);
            
            return parameters;
        }
    }
}


