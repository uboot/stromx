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

#include <cppunit/TestAssert.h>
#include "stromx/runtime/DataContainer.h"
#include "stromx/runtime/Fork.h"
#include "stromx/runtime/OperatorTester.h"
#include "stromx/runtime/ReadAccess.h"
#include "stromx/runtime/test/ForkTest.h"
#include "stromx/runtime/test/TestData.h"

CPPUNIT_TEST_SUITE_REGISTRATION (stromx::runtime::ForkTest);

namespace stromx
{
    using namespace runtime;

    namespace runtime
    {
        void ForkTest::setUp ( void )
        {
            m_operator = new runtime::OperatorTester(new Fork());
        }
        
        void ForkTest::testExecute()
        {
            m_operator->setParameter(Fork::NUM_OUTPUTS, UInt32(3));
            m_operator->initialize();
            m_operator->activate();
            
            for(unsigned int i = 0; i < 4; ++i)
            {
                DataContainer data(new UInt32(i));
                m_operator->setInputData(Fork::INPUT, data);
            }
            
            for(unsigned int i = 0; i < 3; ++i)
            {
                DataContainer data = m_operator->getOutputData(i);
                
                ReadAccess<UInt32> access(data);
                CPPUNIT_ASSERT_EQUAL(UInt32(i), access());
            }
        }
        
        void ForkTest::tearDown ( void )
        {
            delete m_operator;
        }
    }
}
