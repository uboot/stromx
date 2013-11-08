/* 
*  Copyright 2013 Matthias Fuchs
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

#include "stromx/runtime/test/ClientTest.h"

#include <cppunit/TestAssert.h>
#include "stromx/runtime/Client.h"

CPPUNIT_TEST_SUITE_REGISTRATION (stromx::runtime::ClientTest);

namespace stromx
{
    namespace runtime
    {
        void ClientTest::setUp()
        {
            m_operator = new Client();
        }

        void ClientTest::testExecute()
        {
        }

        void ClientTest::tearDown()
        {
            delete m_operator;
        }
    }
}
