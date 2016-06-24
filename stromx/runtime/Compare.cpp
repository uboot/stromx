/* 
*  Copyright 2015 Matthias Fuchs
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

#include "stromx/runtime/Compare.h"
#include "stromx/runtime/DataProvider.h"
#include "stromx/runtime/EnumParameter.h"
#include "stromx/runtime/Id2DataComposite.h"
#include "stromx/runtime/Id2DataPair.h"
#include "stromx/runtime/Locale.h"
#include "stromx/runtime/NumericParameter.h"
#include "stromx/runtime/OperatorException.h"
#include "stromx/runtime/ReadAccess.h"
#include "stromx/runtime/Variant.h"
#include "stromx/runtime/VariantComposite.h"

#include <cmath>

namespace stromx
{
    namespace runtime
    {
        const std::string Compare::TYPE("Compare");
        const std::string Compare::PACKAGE(STROMX_RUNTIME_PACKAGE_NAME);
        const Version Compare::VERSION(STROMX_RUNTIME_VERSION_MAJOR, STROMX_RUNTIME_VERSION_MINOR, STROMX_RUNTIME_VERSION_PATCH);
        
        Compare::Compare()
        : OperatorKernel(TYPE, PACKAGE, VERSION, setupInitParameters())
        , m_compareToInput(false)
        , m_comparisonType(LESS)
        , m_value(0.0)
        , m_epsilon(1e-6)
        {
        }

        void Compare::setParameter(unsigned int id, const Data& value)
        {
            try
            {
                switch(id)
                {
                case COMPARE_TO_INPUT:
                    m_compareToInput = data_cast<Bool>(value);
                    break;
                case COMPARISON_TYPE:
                    m_comparisonType = data_cast<Enum>(value);
                    break;
                case PARAMETER_VALUE:
                    m_value = data_cast<Float64>(value);
                    break;
                case EPSILON:
                    m_epsilon = data_cast<Float64>(value);
                    break;
                default:
                    throw WrongParameterId(id, *this);
                }
            }
            catch(runtime::BadCast&)
            {
                throw WrongParameterType(parameter(id), *this);
            }
        }

        const DataRef Compare::getParameter(const unsigned int id) const
        {
            switch(id)
            {
            case COMPARE_TO_INPUT:
                return m_compareToInput;
            case COMPARISON_TYPE:
                return m_comparisonType;
            case PARAMETER_VALUE:
                return m_value;
            case EPSILON:
                return m_epsilon;
            default:
                throw WrongParameterId(id, *this);
            }
        } 
        
        void Compare::initialize()
        {
            OperatorKernel::initialize(setupInputs(), setupOutputs(), setupParameters());
        }
        
        void Compare::execute(DataProvider& provider)
        {
            double value1 = 0.0;
            double value2 = 0.0;
            Id2DataPair number1Mapper(NUMBER_1);
            Id2DataPair number2Mapper(NUMBER_2);
            
            if (m_compareToInput)
            {
                provider.receiveInputData(number1Mapper && number2Mapper);
                ReadAccess access1(number1Mapper.data());
                ReadAccess access2(number2Mapper.data());
                
                value1 = toDouble(access1.get());
                value2 = toDouble(access2.get());
            }
            else
            {
                provider.receiveInputData(number1Mapper);
                ReadAccess access(number1Mapper.data());
                
                value1 = toDouble(access.get());
                value2 = m_value;
            }
            
            bool result = false;
            switch(m_comparisonType)
            {
            case LESS:
                result = value1 < value2;
                break;
            case GREATER:
                result = value1 > value2;
                break;
            case LESS_OR_EQUAL:
                result = value1 < value2 + m_epsilon;
                break;
            case GREATER_OR_EQUAL:
                result = value1 + m_epsilon > value2;
                break;
            case EQUAL:
                result = std::abs(value1 - value2) < m_epsilon;
                break;
            case NOT_EQUAL:
                result = std::abs(value1 - value2) >= m_epsilon;
                break;
            default:
                throw InternalError("Unsupported comparison type.");
            }
        
            DataContainer resultContainer(new Bool(result));
            provider.sendOutputData(Id2DataPair(RESULT, resultContainer));
        }
        
        const std::vector<const Input*> Compare::setupInputs()
        {
            std::vector<const Input*> inputs;
            
            if (m_compareToInput)
            {
                Input* number1 = new Input(NUMBER_1, Variant::INT || Variant::FLOAT);
                number1->setTitle(L_("Input 1"));
                inputs.push_back(number1);
                
                Input* number2 = new Input(NUMBER_2, Variant::INT || Variant::FLOAT);
                number2->setTitle(L_("Number 2"));
                inputs.push_back(number2);
            }
            else
            {
                Input* number = new Input(NUMBER_1, Variant::INT || Variant::FLOAT);
                number->setTitle(L_("Number"));
                inputs.push_back(number);
            }

            return inputs;
        }
        
        const std::vector<const Output*> Compare::setupOutputs()
        {
            std::vector<const Output*> outputs;
            
            Output* result = new Output(RESULT, Variant::BOOL);
            result->setTitle(L_("Result"));
            outputs.push_back(result);
            
            return outputs;
        }
        
        const std::vector<const Parameter*> Compare::setupInitParameters()
        {
            std::vector<const Parameter*> parameters;
            
            Parameter* compareToInput = new Parameter(COMPARE_TO_INPUT, Variant::BOOL);
            compareToInput->setAccessMode(Parameter::NONE_WRITE);
            compareToInput->setTitle(L_("Compare to second input"));
            parameters.push_back(compareToInput);
            
            return parameters;
        }
        
        const std::vector<const Parameter*> Compare::setupParameters()
        {
            std::vector<const Parameter*> parameters;
            
            EnumParameter* comparisonType = new EnumParameter(COMPARISON_TYPE);
            comparisonType->setAccessMode(Parameter::ACTIVATED_WRITE);
            comparisonType->setTitle(L_("Comparison type"));
            comparisonType->add(EnumDescription(runtime::Enum(LESS), L_("Less")));
            comparisonType->add(EnumDescription(runtime::Enum(LESS_OR_EQUAL), L_("Less or equal")));
            comparisonType->add(EnumDescription(runtime::Enum(GREATER), L_("Greater")));
            comparisonType->add(EnumDescription(runtime::Enum(GREATER_OR_EQUAL), L_("Greater or equal")));
            comparisonType->add(EnumDescription(runtime::Enum(EQUAL), L_("Equal")));
            comparisonType->add(EnumDescription(runtime::Enum(NOT_EQUAL), L_("Not equal")));
            parameters.push_back(comparisonType);
            
            if (! m_compareToInput)
            {
                NumericParameter<Float64>* parameterValue = new NumericParameter<Float64>(PARAMETER_VALUE);
                parameterValue->setAccessMode(Parameter::ACTIVATED_WRITE);
                parameterValue->setTitle(L_("Value"));
                parameters.push_back(parameterValue);
            }
            
            NumericParameter<Float64>* epsilon = new NumericParameter<Float64>(EPSILON);
            epsilon->setAccessMode(Parameter::ACTIVATED_WRITE);
            epsilon->setTitle(L_("Epsilon"));
            epsilon->setMin(Float64(0.0));
            parameters.push_back(epsilon);
            
            return parameters;
        }
    } 
}
