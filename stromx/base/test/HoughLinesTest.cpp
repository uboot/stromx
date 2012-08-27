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
#include "HoughLinesTest.h"
#include "../HoughLines.h"
#include <stromx/core/DataContainer.h>
#include <stromx/core/OperatorTester.h>
#include <stromx/core/Primitive.h>
#include <stromx/core/ReadAccess.h>

CPPUNIT_TEST_SUITE_REGISTRATION (stromx::base::HoughLinesTest);

namespace stromx
{
    using namespace core;

    namespace base
    {
        void HoughLinesTest::setUp ( void )
        {
            m_operator = new core::OperatorTester(new HoughLines());
            m_operator->initialize();
            m_operator->activate();
        }
        
        void HoughLinesTest::testExecute()
        {
        }
        
        void HoughLinesTest::tearDown ( void )
        {
            delete m_operator;
        }
    }
}
