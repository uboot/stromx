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

#ifndef STROMX_RUNTIME_OPERATORKERNELTEST_H
#define STROMX_RUNTIME_OPERATORKERNELTEST_H

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>
#include "stromx/runtime/test/TestOperator.h"

namespace stromx
{
    namespace runtime
    {
        class OperatorKernel;
        
        class OperatorKernelTest : public CPPUNIT_NS :: TestFixture
        {
            CPPUNIT_TEST_SUITE (OperatorKernelTest);
            CPPUNIT_TEST (testValidateInputsExistingId);
            CPPUNIT_TEST (testValidateInputsDuplicateId);
            CPPUNIT_TEST (testValidateOutputsExistingId);
            CPPUNIT_TEST (testValidateOutputsDuplicateId);
            CPPUNIT_TEST (testValidateParametersExistingId);
            CPPUNIT_TEST (testValidateParametersDuplicateId);
            CPPUNIT_TEST (testValidateParametersMissingGroup);
            CPPUNIT_TEST (testValidateInputsAndOutputsDuplicateId);
            CPPUNIT_TEST (testValidateParametersExistingInputId);
            CPPUNIT_TEST_SUITE_END ();

        public:
                OperatorKernelTest() {}
                
                void setUp();
                void tearDown() {}

            protected:
                void testValidateInputsExistingId();
                void testValidateInputsDuplicateId();
                void testValidateOutputsExistingId();
                void testValidateOutputsDuplicateId();
                void testValidateParametersExistingId();
                void testValidateParametersDuplicateId();
                void testValidateParametersMissingGroup();
                void testValidateInputsAndOutputsDuplicateId();
                void testValidateParametersExistingInputId();
                
            private:
                TestOperator m_op;
        };
    }
}

#endif // STROMX_RUNTIME_OPERATORKERNELTEST_H