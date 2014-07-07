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

#include "stromx/raspi/test/WriteGpioTest.h"
#include <cppunit/TestAssert.h>
#include <cppunit/TestAssert.h>
#include "stromx/raspi/WriteGpio.h"

CPPUNIT_TEST_SUITE_REGISTRATION (stromx::raspi::WriteGpioTest);

namespace stromx
{
    using namespace runtime;

    namespace raspi
    {
        void WriteGpioTest::setUp()
        {
            m_operator = new OperatorTester(new WriteGpio());
            m_operator->initialize();
            m_operator->setParameter(WriteGpio::GPIO, Enum(4));
            m_operator->activate();
        }

        void WriteGpioTest::testExecute()
        {
            runtime::DataContainer data(new runtime::Bool(true));
            
            m_operator->setInputData(WriteGpio::INPUT, data);
        }

        void WriteGpioTest::tearDown()
        {
            delete m_operator;
        }
    }
}
