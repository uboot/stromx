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

#include "stromx/runtime/test/OperatorKernelTest.h"

#include "stromx/runtime/OperatorKernel.h"
#include "stromx/runtime/ParameterGroup.h"
#include "stromx/runtime/Variant.h"

CPPUNIT_TEST_SUITE_REGISTRATION (stromx::runtime::OperatorKernelTest);

namespace stromx
{
    namespace runtime
    {
        namespace
        {
            class DefaultParameterOperator : public OperatorKernel
            {
            public:
                DefaultParameterOperator()
                    : OperatorKernel("", "", Version(), setupInputs(), 
                        std::vector<const Output*>())
                {}
                        
                void initialize()
                {
                    OperatorKernel::initialize(std::vector<const Input*>(), 
                        setupOutputs(), std::vector<const Parameter*>());
                }
                
                OperatorKernel* clone() const { return 0; }
                void execute(DataProvider&) {}
                
                static const std::vector<const Input* > setupInputs()
                {
                    std::vector<const Input*> inputs;
                    
                    Input* input = new Input(0, Variant::NONE);
                    input->setDefaultType(Input::PARAMETER, Input::PUSH);
                    inputs.push_back(input);
                    
                    return inputs;
                }
                
                static const std::vector<const Output* > setupOutputs()
                {
                    std::vector<const Output*> outputs;
                    
                    Output* output = new Output(1, Variant::DATA);
                    output->setDefaultType(Input::PARAMETER);
                    outputs.push_back(output);
                    
                    return outputs;
                }
            };
            
            class ConnectorOperator : public OperatorKernel
            {
            public:
                ConnectorOperator()
                    : OperatorKernel("", "", Version(), setupInputs(), 
                        std::vector<const Output*>())
                {}
                        
                void initialize()
                {
                    OperatorKernel::initialize(std::vector<const Input*>(), 
                        setupOutputs(), setupParameters());
                }
                
                OperatorKernel* clone() const { return 0; }
                void execute(DataProvider&) {}
                
                static const std::vector<const Input* > setupInputs()
                {
                    std::vector<const Input*> inputs;
                    
                    Input* input = new Input(0, Variant::NONE);
                    inputs.push_back(input);
                    
                    return inputs;
                }
                
                static const std::vector<const Output* > setupOutputs()
                {
                    std::vector<const Output*> outputs;
                    
                    Output* output = new Output(1, Variant::DATA);
                    outputs.push_back(output);
                    
                    return outputs;
                }
                
                static const std::vector<const Parameter* > setupParameters()
                {
                    std::vector<const Parameter*> parameters;
                    
                    Parameter* parameter = new Parameter(2, Variant::DATA);
                    parameters.push_back(parameter);
                    
                    return parameters;
                }
            };
        }
        
        void OperatorKernelTest::setUp()
        {
            m_op.initialize();
        }
                
        void OperatorKernelTest::testValidateInputsExistingId()
        {
            Input input(TestOperator::INPUT_1, Variant::NONE);
            std::vector<const Input*> descriptors;
            descriptors.push_back(&input);
            
            CPPUNIT_ASSERT_THROW(m_op.validateDescriptions(
                                    descriptors,
                                    std::vector<const Output*>(), 
                                    std::vector<const Parameter*>()),
                                 WrongArgument);
        }

        void OperatorKernelTest::testValidateInputsDuplicateId()
        {
            Input input(TestOperator::INPUT_2 + 1, Variant::NONE);
            std::vector<const Input*> descriptors;
            descriptors.push_back(&input);
            descriptors.push_back(&input);
            
            CPPUNIT_ASSERT_THROW(m_op.validateDescriptions(
                                    descriptors,
                                    std::vector<const Output*>(), 
                                    std::vector<const Parameter*>()),
                                 WrongArgument);
        }

        void OperatorKernelTest::testValidateOutputsExistingId()
        {
            Output output(TestOperator::OUTPUT_1, Variant::NONE);
            std::vector<const Output*> descriptors;
            descriptors.push_back(&output);
            
            CPPUNIT_ASSERT_THROW(m_op.validateDescriptions(
                                    std::vector<const Input*>(),
                                    descriptors, 
                                    std::vector<const Parameter*>()),
                                 WrongArgument);
        }

        void OperatorKernelTest::testValidateOutputsDuplicateId()
        {
            Output output(TestOperator::OUTPUT_2 + 1, Variant::NONE);
            std::vector<const Output*> descriptors;
            descriptors.push_back(&output);
            descriptors.push_back(&output);
            
            CPPUNIT_ASSERT_THROW(m_op.validateDescriptions(
                                    std::vector<const Input*>(),
                                    descriptors, 
                                    std::vector<const Parameter*>()),
                                 WrongArgument);
        }

        void OperatorKernelTest::testValidateParametersExistingId()
        {
            Parameter param(TestOperator::SLEEP_TIME, Variant::UINT_32);
            std::vector<const Parameter*> params;
            params.push_back(&param);
            
            CPPUNIT_ASSERT_THROW(m_op.validateDescriptions(
                                    std::vector<const Input*>(),
                                    std::vector<const Output*>(),
                                    params),
                                 WrongArgument);
        }

        void OperatorKernelTest::testValidateParametersDuplicateId()
        {
            Parameter param(TestOperator::NUM_PARAMETERS, Variant::UINT_32);
            std::vector<const Parameter*> params;
            params.push_back(&param);
            params.push_back(&param);
            
            CPPUNIT_ASSERT_THROW(m_op.validateDescriptions(
                                    std::vector<const Input*>(),
                                    std::vector<const Output*>(),
                                    params),
                                 WrongArgument);
        }
        
        void OperatorKernelTest::testValidateParametersMissingGroup()
        {
            ParameterGroup group(TestOperator::NUM_DESCRIPTIONS);
            Parameter param(TestOperator::NUM_DESCRIPTIONS + 1, Variant::UINT_32, &group);
            std::vector<const Parameter*> params;
            params.push_back(&param);
            
            CPPUNIT_ASSERT_THROW(m_op.validateDescriptions(
                                    std::vector<const Input*>(),
                                    std::vector<const Output*>(),
                                    params),
                                 WrongArgument);
            
            params.push_back(&group);
            CPPUNIT_ASSERT_NO_THROW(m_op.validateDescriptions(
                                        std::vector<const Input*>(),
                                        std::vector<const Output*>(),
                                        params)
                                   );
        }
        
        void OperatorKernelTest::testValidateInputsAndOutputsDuplicateId()
        {
            Input input(TestOperator::OUTPUT_2 + 1, Variant::NONE);
            Output output(TestOperator::OUTPUT_2 + 1, Variant::NONE);
            std::vector<const Input*> inputs;
            std::vector<const Output*> outputs;
            inputs.push_back(&input);
            outputs.push_back(&output);
            
            CPPUNIT_ASSERT_THROW(m_op.validateDescriptions(
                                    inputs,
                                    outputs,
                                    std::vector<const Parameter*>()),
                                 WrongArgument);
        }
        
        void OperatorKernelTest::testValidateParametersExistingInputId()
        {
            Parameter param(TestOperator::OUTPUT_1, Variant::UINT_32);
            std::vector<const Parameter*> params;
            params.push_back(&param);
            
            CPPUNIT_ASSERT_THROW(m_op.validateDescriptions(
                                    std::vector<const Input*>(),
                                    std::vector<const Output*>(),
                                    params),
                                 WrongArgument);
        }
        
        void OperatorKernelTest::testDefaultTypeParameter()
        {
            DefaultParameterOperator kernel;
            CPPUNIT_ASSERT_EQUAL(std::size_t(0), kernel.parameters().size());
            
            kernel.initialize();
            CPPUNIT_ASSERT_EQUAL(std::size_t(0), kernel.inputs().size());
            CPPUNIT_ASSERT_EQUAL(std::size_t(0), kernel.outputs().size());
            CPPUNIT_ASSERT_EQUAL(std::size_t(2), kernel.parameters().size());
            CPPUNIT_ASSERT_EQUAL(DescriptionBase::PUSH, kernel.parameter(0).updateBehavior());
            CPPUNIT_ASSERT_EQUAL(DescriptionBase::PERSISTENT, kernel.parameter(1).updateBehavior());
            CPPUNIT_ASSERT(kernel.parameter(0).variant().isVariant(Variant::NONE));
            CPPUNIT_ASSERT(kernel.parameter(1).variant().isVariant(Variant::DATA));
        } 
        
        void OperatorKernelTest::testSetConnectorTypeInputToParameter()
        {
            ConnectorOperator kernel;
            kernel.initialize();
            
            kernel.setConnectorType(0, DescriptionBase::PARAMETER, DescriptionBase::PUSH);
            CPPUNIT_ASSERT_EQUAL(std::size_t(0), kernel.inputs().size());
            CPPUNIT_ASSERT_EQUAL(std::size_t(2), kernel.parameters().size());    
            CPPUNIT_ASSERT_EQUAL(DescriptionBase::PUSH, kernel.parameter(0).updateBehavior());   
            CPPUNIT_ASSERT(kernel.parameter(0).variant().isVariant(Variant::NONE));   
        } 
        
        void OperatorKernelTest::testSetConnectorTypeNoInput()
        {
            ConnectorOperator kernel;
            CPPUNIT_ASSERT_THROW(kernel.setConnectorType(0, DescriptionBase::PARAMETER),
                                 WrongArgument);
        }
        
        void OperatorKernelTest::testSetConnectorTypeOutputToParameter()
        {
            ConnectorOperator kernel;
            kernel.initialize();
            
            kernel.setConnectorType(1, DescriptionBase::PARAMETER, DescriptionBase::PULL);
            CPPUNIT_ASSERT_EQUAL(std::size_t(0), kernel.outputs().size());
            CPPUNIT_ASSERT_EQUAL(std::size_t(2), kernel.parameters().size());    
            CPPUNIT_ASSERT_EQUAL(DescriptionBase::PULL, kernel.parameter(1).updateBehavior());   
            CPPUNIT_ASSERT(kernel.parameter(1).variant().isVariant(Variant::DATA));   
        } 
        
        void OperatorKernelTest::testSetConnectorTypeNoOutput()
        {
            ConnectorOperator kernel;
            CPPUNIT_ASSERT_THROW(kernel.setConnectorType(1, DescriptionBase::PARAMETER),
                                 WrongArgument);
        } 

        void OperatorKernelTest::testSetConnectorTypeParameterToInput()
        {
            ConnectorOperator kernel;
            kernel.initialize();
            kernel.setConnectorType(0, DescriptionBase::PARAMETER, DescriptionBase::PUSH);
            
            kernel.setConnectorType(0, DescriptionBase::INPUT);
            CPPUNIT_ASSERT_EQUAL(std::size_t(1), kernel.parameters().size());
            CPPUNIT_ASSERT_EQUAL(std::size_t(1), kernel.inputs().size());
        }

        void OperatorKernelTest::testSetConnectorTypeParameterToOutput()
        {
            ConnectorOperator kernel;
            kernel.initialize();
            kernel.setConnectorType(1, DescriptionBase::PARAMETER, DescriptionBase::PULL);
            
            kernel.setConnectorType(1, DescriptionBase::OUTPUT);
            CPPUNIT_ASSERT_EQUAL(std::size_t(1), kernel.parameters().size());
            CPPUNIT_ASSERT_EQUAL(std::size_t(1), kernel.outputs().size());
        }
        
        void OperatorKernelTest::testSetConnectorTypeInvalidTypeToInput()
        {
            ConnectorOperator kernel;
            kernel.initialize();
            kernel.setConnectorType(1, DescriptionBase::PARAMETER, DescriptionBase::PULL);
            
            CPPUNIT_ASSERT_THROW(kernel.setConnectorType(1, DescriptionBase::INPUT),
                                 WrongArgument);
        }
        
        void OperatorKernelTest::testSetConnectorTypeInvalidTypeToOutput()
        {
            ConnectorOperator kernel;
            kernel.initialize();
            kernel.setConnectorType(0, DescriptionBase::PARAMETER, DescriptionBase::PUSH);
            
            CPPUNIT_ASSERT_THROW(kernel.setConnectorType(0, DescriptionBase::OUTPUT),
                                 WrongArgument);
        }
        
        void OperatorKernelTest::testSetConnectorTypeParameterToParameter()
        {
            ConnectorOperator kernel;
            kernel.initialize();
            kernel.setConnectorType(2, DescriptionBase::PARAMETER, DescriptionBase::PUSH);
            
            CPPUNIT_ASSERT_EQUAL(std::size_t(1), kernel.parameters().size());
            CPPUNIT_ASSERT_EQUAL(DescriptionBase::PERSISTENT, kernel.parameter(2).updateBehavior());   
        }
        
        void OperatorKernelTest::testSetConnectorTypeInputToPullParameter()
        {
            ConnectorOperator kernel;
            kernel.initialize();
            
            CPPUNIT_ASSERT_THROW(kernel.setConnectorType(0, DescriptionBase::PARAMETER,
                                                         DescriptionBase::PULL),
                                 WrongArgument);
        }
        
        void OperatorKernelTest::testSetConnectorTypeOutputToPushParameter()
        {
            ConnectorOperator kernel;
            kernel.initialize();
            
            CPPUNIT_ASSERT_THROW(kernel.setConnectorType(1, DescriptionBase::PARAMETER,
                                                         DescriptionBase::PUSH),
                                 WrongArgument);
        }
    }
}