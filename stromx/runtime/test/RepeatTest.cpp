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
#include "stromx/runtime/Repeat.h"
#include "stromx/runtime/OperatorTester.h"
#include "stromx/runtime/ReadAccess.h"
#include "stromx/runtime/test/RepeatTest.h"

CPPUNIT_TEST_SUITE_REGISTRATION (stromx::runtime::RepeatTest);

namespace stromx
{
    using namespace runtime;

    namespace runtime
    {
        void RepeatTest::setUp ( void )
        {
            m_operator = new runtime::OperatorTester(new Repeat());
            m_operator->initialize();
            m_operator->activate();
        }
        
        void RepeatTest::testExecute()
        {
            DataContainer input(new Int32(10));
            DataContainer numIterations(new UInt32(3));
            m_operator->setInputData(Repeat::NUM_ITERATIONS, numIterations);
            m_operator->setInputData(Repeat::INPUT, input);
            
            for (int i = 0; i < 3; ++i)
            {
                DataContainer output = m_operator->getOutputData(Repeat::OUTPUT);
                m_operator->clearOutputData(Repeat::OUTPUT);
                
                CPPUNIT_ASSERT_EQUAL(input, output);
            }
        }
        
        void RepeatTest::testExecuteNoIteration()
        {
            DataContainer input(new Int32(10));
            DataContainer numIterations(new UInt32(0));
            
            for (int i = 0; i < 3; ++i)
            {
                m_operator->setInputData(Repeat::NUM_ITERATIONS, numIterations);
                m_operator->setInputData(Repeat::INPUT, input);
            }
        }
        
        void RepeatTest::tearDown ( void )
        {
            delete m_operator;
        }
    }
}
