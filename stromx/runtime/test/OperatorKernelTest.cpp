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

#include "stromx/runtime/OperatorKernel.h"
#include "stromx/runtime/ParameterGroup.h"
#include "stromx/runtime/test/OperatorKernelTest.h"

CPPUNIT_TEST_SUITE_REGISTRATION (stromx::runtime::OperatorKernelTest);

namespace stromx
{
    namespace runtime
    {
        void OperatorKernelTest::setUp()
        {
            m_op.initialize();
        }
                
        void OperatorKernelTest::testValidateInputsExistingId()
        {
            Description input(TestOperator::INPUT_1, Variant::NONE);
            std::vector<const Description*> descriptors;
            descriptors.push_back(&input);
            
            CPPUNIT_ASSERT_THROW(m_op.validateInputs(descriptors), WrongArgument);
        }

        void OperatorKernelTest::testValidateInputsDuplicateId()
        {
            Description input(TestOperator::INPUT_2 + 1, Variant::NONE);
            std::vector<const Description*> descriptors;
            descriptors.push_back(&input);
            descriptors.push_back(&input);
            
            CPPUNIT_ASSERT_THROW(m_op.validateInputs(descriptors), WrongArgument);
        }

        void OperatorKernelTest::testValidateOutputsExistingId()
        {
            Description output(TestOperator::OUTPUT_1, Variant::NONE);
            std::vector<const Description*> descriptors;
            descriptors.push_back(&output);
            
            CPPUNIT_ASSERT_THROW(m_op.validateOutputs(descriptors), WrongArgument);
        }

        void OperatorKernelTest::testValidateOutputsDuplicateId()
        {
            Description output(TestOperator::OUTPUT_2 + 1, Variant::NONE);
            std::vector<const Description*> descriptors;
            descriptors.push_back(&output);
            descriptors.push_back(&output);
            
            CPPUNIT_ASSERT_THROW(m_op.validateOutputs(descriptors), WrongArgument);
        }

        void OperatorKernelTest::testValidateParametersExistingId()
        {
            Parameter param(TestOperator::SLEEP_TIME, Variant::UINT_32);
            std::vector<const Parameter*> params;
            params.push_back(&param);
            
            CPPUNIT_ASSERT_THROW(m_op.validateParameters(params), WrongArgument);
        }

        void OperatorKernelTest::testValidateParametersDuplicateId()
        {
            Parameter param(TestOperator::NUM_PARAMETERS, Variant::UINT_32);
            std::vector<const Parameter*> params;
            params.push_back(&param);
            params.push_back(&param);
            
            CPPUNIT_ASSERT_THROW(m_op.validateParameters(params), WrongArgument);
        }
        
        void OperatorKernelTest::testValidateParametersMissingGroup()
        {
            ParameterGroup group(TestOperator::NUM_PARAMETERS);
            Parameter param(TestOperator::NUM_PARAMETERS + 1, Variant::UINT_32, &group);
            std::vector<const Parameter*> params;
            params.push_back(&param);
            
            CPPUNIT_ASSERT_THROW(m_op.validateParameters(params), WrongArgument);
            
            params.push_back(&group);
            CPPUNIT_ASSERT_NO_THROW(m_op.validateParameters(params));
        }
    }
}