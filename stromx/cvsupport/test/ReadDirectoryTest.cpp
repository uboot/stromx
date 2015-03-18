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
#include <stromx/runtime/DataContainer.h>
#include <stromx/runtime/OperatorException.h>
#include <stromx/runtime/OperatorTester.h>
#include <stromx/runtime/Primitive.h>
#include <stromx/runtime/ReadAccess.h>
#include "stromx/cvsupport/ReadDirectory.h"
#include "stromx/cvsupport/Image.h"
#include "stromx/cvsupport/test/ReadDirectoryTest.h"

CPPUNIT_TEST_SUITE_REGISTRATION (stromx::cvsupport::ReadDirectoryTest);

namespace stromx
{
    using namespace runtime;

    namespace cvsupport
    {
        void ReadDirectoryTest::setUp ( void )
        {
            m_operator = new runtime::OperatorTester(new ReadDirectory());
            m_operator->initialize();
            m_operator->activate();
        }
        
        void ReadDirectoryTest::testExecute()
        {
            CPPUNIT_ASSERT_THROW(m_operator->getOutputData(ReadDirectory::OUTPUT), OperatorError);
        }
        
        void ReadDirectoryTest::tearDown ( void )
        {
            delete m_operator;
        }
    }
}
