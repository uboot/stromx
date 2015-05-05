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
#include "stromx/runtime/Filter.h"
#include "stromx/runtime/OperatorTester.h"
#include "stromx/runtime/ReadAccess.h"
#include "stromx/runtime/test/FilterTest.h"

CPPUNIT_TEST_SUITE_REGISTRATION (stromx::runtime::FilterTest);

namespace stromx
{
    using namespace runtime;

    namespace runtime
    {
        void FilterTest::setUp ( void )
        {
            m_operator = new runtime::OperatorTester(new Filter());
            m_operator->initialize();
            m_operator->activate();
        }
        
        void FilterTest::testExecutePreserve()
        {
            DataContainer input(new Int32(4));
            DataContainer flag(new Bool(true));
            
            m_operator->setInputData(Filter::INPUT, input);
            m_operator->setInputData(Filter::FILTER_FLAG, flag);
            
            DataContainer output = m_operator->getOutputData(Filter::OUTPUT);
            CPPUNIT_ASSERT_EQUAL(Int32(4), ReadAccess<Int32>(output).get());
        }
        
        void FilterTest::testExecuteRemove()
        {
            for (int i = 0; i < 5; ++i)
            {
                DataContainer input(new Int32(4));
                DataContainer flag(new Bool(false));
                
                m_operator->setInputData(Filter::INPUT, input);
                m_operator->setInputData(Filter::FILTER_FLAG, flag);
            }
        }
        
        void FilterTest::tearDown ( void )
        {
            delete m_operator;
        }
    }
}
