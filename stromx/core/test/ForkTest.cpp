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
#include "ForkTest.h"
#include "TestData.h"
#include "../Fork.h"
#include "../Image.h"
#include "../DataContainer.h"
#include "../OperatorTester.h"
#include "../ReadAccess.h"

CPPUNIT_TEST_SUITE_REGISTRATION (stromx::core::ForkTest);

namespace stromx
{
    using namespace core;

    namespace core
    {
        void ForkTest::setUp ( void )
        {
            m_operator = new core::OperatorTester(new Fork());
            m_operator->initialize();
            m_operator->activate();
        }
        
        void ForkTest::testExecute()
        {
            
        }
        
        void ForkTest::tearDown ( void )
        {
            delete m_operator;
        }
    }
}
