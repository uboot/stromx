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

#include <cppunit/TestAssert.h>
#include "stromx/runtime/test/CompareTest.h"
#include "stromx/runtime/Compare.h"
#include "stromx/runtime/DataContainer.h"
#include "stromx/runtime/OperatorTester.h"
#include "stromx/runtime/ReadAccess.h"

CPPUNIT_TEST_SUITE_REGISTRATION (stromx::runtime::CompareTest);

namespace stromx
{
    using namespace runtime;

    namespace runtime
    {
        void CompareTest::setUp()
        {
            m_operator = new OperatorTester(new Compare());
        }

        void CompareTest::testExecuteLessThanParameterTrue()
        {
            m_operator->initialize();
            m_operator->activate();
            m_operator->setParameter(Compare::COMPARISON_TYPE, Enum(Compare::LESS));
            m_operator->setParameter(Compare::PARAMETER_VALUE, Float64(3.0));
            
            m_operator->setInputData(Compare::NUMBER_1, DataContainer(new Float32(1.0)));
            DataContainer result = m_operator->getOutputData(Compare::RESULT);
            
            ReadAccess access(result);
            CPPUNIT_ASSERT_EQUAL(Bool(true), access.get<Bool>());
        }

        void CompareTest::testExecuteLessThanParameterFalse()
        {
            m_operator->initialize();
            m_operator->activate();
            m_operator->setParameter(Compare::COMPARISON_TYPE, Enum(Compare::LESS));
            m_operator->setParameter(Compare::PARAMETER_VALUE, Float64(3.0));
            
            m_operator->setInputData(Compare::NUMBER_1, DataContainer(new Float32(4.0)));
            DataContainer result = m_operator->getOutputData(Compare::RESULT);
            
            ReadAccess access(result);
            CPPUNIT_ASSERT_EQUAL(Bool(false), access.get<Bool>());
        }

        void CompareTest::testExecuteLessThanInputTrue()
        {
            m_operator->setParameter(Compare::COMPARE_TO_INPUT, Bool(true));
            m_operator->initialize(); 
            m_operator->activate();
            m_operator->setParameter(Compare::COMPARISON_TYPE, Enum(Compare::LESS));
            
            m_operator->setInputData(Compare::NUMBER_1, DataContainer(new Int32(1.0)));
            m_operator->setInputData(Compare::NUMBER_2, DataContainer(new Float64(3.0)));
            DataContainer result = m_operator->getOutputData(Compare::RESULT);
            
            ReadAccess access(result);
            CPPUNIT_ASSERT_EQUAL(Bool(true), access.get<Bool>());
        }

        void CompareTest::testExecuteEqualToParameterTrue()
        {
            m_operator->initialize();
            m_operator->activate();
            m_operator->setParameter(Compare::COMPARISON_TYPE, Enum(Compare::EQUAL));
            m_operator->setParameter(Compare::PARAMETER_VALUE, Float64(3.0));
            m_operator->setParameter(Compare::EPSILON, Float64(0.1));
            
            m_operator->setInputData(Compare::NUMBER_1, DataContainer(new Float32(3.05)));
            DataContainer result = m_operator->getOutputData(Compare::RESULT);
            
            ReadAccess access(result);
            CPPUNIT_ASSERT_EQUAL(Bool(true), access.get<Bool>());
        }

        void CompareTest::testExecuteEqualToParameterFalse()
        {
            m_operator->initialize();
            m_operator->activate();
            m_operator->setParameter(Compare::COMPARISON_TYPE, Enum(Compare::EQUAL));
            m_operator->setParameter(Compare::PARAMETER_VALUE, Float64(3.0));
            
            m_operator->setInputData(Compare::NUMBER_1, DataContainer(new Float32(3.05)));
            DataContainer result = m_operator->getOutputData(Compare::RESULT);
            
            ReadAccess access(result);
            CPPUNIT_ASSERT_EQUAL(Bool(false), access.get<Bool>());
        }

        void CompareTest::testExecuteLessOrEqualToParameterTrue()
        {
            m_operator->initialize();
            m_operator->activate();
            m_operator->setParameter(Compare::COMPARISON_TYPE, Enum(Compare::LESS_OR_EQUAL));
            m_operator->setParameter(Compare::PARAMETER_VALUE, Float64(3.0));
            m_operator->setParameter(Compare::EPSILON, Float64(0.1));
            
            m_operator->setInputData(Compare::NUMBER_1, DataContainer(new Float32(3.05)));
            DataContainer result = m_operator->getOutputData(Compare::RESULT);
            
            ReadAccess access(result);
            CPPUNIT_ASSERT_EQUAL(Bool(true), access.get<Bool>());
        }

        void CompareTest::testExecuteLessOrEqualToParameterFalse()
        {
            m_operator->initialize();
            m_operator->activate();
            m_operator->setParameter(Compare::COMPARISON_TYPE, Enum(Compare::LESS_OR_EQUAL));
            m_operator->setParameter(Compare::PARAMETER_VALUE, Float64(3.0));
            
            m_operator->setInputData(Compare::NUMBER_1, DataContainer(new Float32(3.05)));
            DataContainer result = m_operator->getOutputData(Compare::RESULT);
            
            ReadAccess access(result);
            CPPUNIT_ASSERT_EQUAL(Bool(false), access.get<Bool>());
        }

        void CompareTest::testExecuteLessUInt32True()
        {
            m_operator->initialize();
            m_operator->activate();
            m_operator->setParameter(Compare::COMPARISON_TYPE, Enum(Compare::LESS));
            m_operator->setParameter(Compare::PARAMETER_VALUE, Float64(3.0));
            
            m_operator->setInputData(Compare::NUMBER_1, DataContainer(new UInt32(1)));
            DataContainer result = m_operator->getOutputData(Compare::RESULT);
            
            ReadAccess access(result);
            CPPUNIT_ASSERT_EQUAL(Bool(true), access.get<Bool>());
        }

        void CompareTest::testExecuteLessUInt32False()
        {
            m_operator->initialize();
            m_operator->activate();
            m_operator->setParameter(Compare::COMPARISON_TYPE, Enum(Compare::LESS));
            m_operator->setParameter(Compare::PARAMETER_VALUE, Float64(3.0));
            
            m_operator->setInputData(Compare::NUMBER_1, DataContainer(new UInt32(5)));
            DataContainer result = m_operator->getOutputData(Compare::RESULT);
            
            ReadAccess access(result);
            CPPUNIT_ASSERT_EQUAL(Bool(false), access.get<Bool>());
        }

        void CompareTest::tearDown()
        {
            delete m_operator;
        }
    }
}
