/* 
*  Copyright 2014 Matthias Fuchs
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

#include "stromx/raspi/test/ReadGpioTest.h"
#include <cppunit/TestAssert.h>
#include <cppunit/TestAssert.h>
#include "stromx/raspi/ReadGpio.h"

CPPUNIT_TEST_SUITE_REGISTRATION (stromx::raspi::ReadGpioTest);

namespace stromx
{
    using namespace runtime;

    namespace raspi
    {
        void ReadGpioTest::setUp()
        {
            m_operator = new OperatorTester(new ReadGpio());
            m_operator->initialize();
            m_operator->setParameter(ReadGpio::GPIO, Enum(4));
            m_operator->activate();
        }

        void ReadGpioTest::testExecute()
        {
            DataContainer data = m_operator->getOutputData(ReadGpio::OUTPUT);
        }

        void ReadGpioTest::tearDown()
        {
            delete m_operator;
        }
    }
}
