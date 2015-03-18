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

#ifndef STROMX_CVSUPPORT_ReadDirectoryTEST_H
#define STROMX_CVSUPPORT_ReadDirectoryTEST_H

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>
#include <stromx/runtime/DataContainer.h>

namespace stromx
{
    namespace runtime
    {
        class OperatorTester;
    }

    namespace cvsupport
    {
        class ReadDirectoryTest : public CPPUNIT_NS :: TestFixture
        {
            CPPUNIT_TEST_SUITE (ReadDirectoryTest);
            CPPUNIT_TEST (testInitialize);
            CPPUNIT_TEST (testActivateNoDirectory);
            CPPUNIT_TEST (testActivateValidDirectory);
            CPPUNIT_TEST (testExecute);
            CPPUNIT_TEST (testExecuteEmptyDirectory);
            CPPUNIT_TEST_SUITE_END ();

            public:
                ReadDirectoryTest() : m_operator(0) {}
                
                void setUp();
                void tearDown();

            protected:
                void testInitialize();
                void testActivateNoDirectory();
                void testActivateValidDirectory();
                void testExecute();
                void testExecuteEmptyDirectory();
                
            private:
                runtime::OperatorTester* m_operator;
        };
    }
}

#endif // STROMX_CVSUPPORT_ReadDirectoryTEST_H