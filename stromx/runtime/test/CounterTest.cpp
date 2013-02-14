/* 
*  Copyright 2011 Matthias Fuchs
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
#include "stromx/runtime/test/CounterTest.h"
#include "stromx/runtime/Counter.h"
#include "stromx/runtime/DataContainer.h"
#include "stromx/runtime/OperatorTester.h"
#include "stromx/runtime/ReadAccess.h"

CPPUNIT_TEST_SUITE_REGISTRATION (stromx::runtime::CounterTest);

namespace stromx
{
    using namespace runtime;

    namespace runtime
    {
        void CounterTest::setUp()
        {
            m_operator = new OperatorTester(new Counter());
            m_operator->initialize();
            m_operator->activate();
        }

        void CounterTest::testExecute()
        {
            DataContainer data = m_operator->getOutputData(Counter::OUTPUT);
            CPPUNIT_ASSERT_EQUAL(UInt32(0), ReadAccess<UInt32>(data)());
            
            m_operator->clearOutputData(Counter::OUTPUT);
            
            data = m_operator->getOutputData(Counter::OUTPUT);
            CPPUNIT_ASSERT_EQUAL(UInt32(1), ReadAccess<UInt32>(data)());
            
            m_operator->deactivate();
            m_operator->activate();
            
            data = m_operator->getOutputData(Counter::OUTPUT);
            CPPUNIT_ASSERT_EQUAL(UInt32(0), ReadAccess<UInt32>(data)());
        }

        void CounterTest::tearDown()
        {
            delete m_operator;
        }
    }
}
