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

#include "stromx/raspi/test/GpioTriggerTest.h"

#include <boost/bind.hpp>
#include <boost/thread.hpp>
#include <cppunit/TestAssert.h>
#include <cppunit/TestAssert.h>
#include "stromx/raspi/GpioTrigger.h"

CPPUNIT_TEST_SUITE_REGISTRATION (stromx::raspi::GpioTriggerTest);

namespace stromx
{
    using namespace runtime;

    namespace raspi
    {
        void GpioTriggerTest::setUp()
        {
            m_operator = new OperatorTester(new GpioTrigger());
            m_operator->initialize();
            m_operator->setParameter(GpioTrigger::GPIO, Enum(4));
            m_operator->activate();
        }

        void GpioTriggerTest::testInterrupt()
        {
            // interrupt the execution in a separate thread
            boost::thread t(boost::bind(&GpioTriggerTest::interruptExecution, this));
            
            // wait for the interrupt
            CPPUNIT_ASSERT_THROW(m_operator->getOutputData(GpioTrigger::OUTPUT), Interrupt);
            t.join();
        }

        void GpioTriggerTest::tearDown()
        {
            delete m_operator;
        }
            
        void GpioTriggerTest::interruptExecution()
        {
            boost::this_thread::sleep_for(boost::chrono::seconds(1));
            m_operator->interrupt();
        }
    }
}
