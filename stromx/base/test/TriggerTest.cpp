/* 
*  Copyright 2011 Matthias Fuchs
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

#include <boost/bind.hpp>
#include <boost/thread.hpp>
#include <cppunit/TestAssert.h>
#include "TriggerTest.h"
#include "../Image.h"
#include "../Trigger.h"
#include <stromx/core/Exception.h>
#include <stromx/core/OperatorTester.h>
#include <stromx/core/ReadAccess.h>
#include <stromx/core/Trigger.h>

CPPUNIT_TEST_SUITE_REGISTRATION (stromx::base::TriggerTest);

namespace stromx
{
    using namespace core;

    namespace base
    {
        void TriggerTest::setUp ( void )
        {
            m_operator = new core::OperatorTester(new Trigger());
            m_operator->initialize();
            m_operator->activate();
            m_image = DataContainer(new Image("lenna.jpg"));
            m_operator->setInputData(Trigger::INPUT, m_image);
        }
        
        void TriggerTest::testExecuteActive()
        {
            /*** Test 1 ***/
            boost::thread t1(boost::bind(&TriggerTest::triggerDelayed, this));
            DataContainer result = m_operator->getOutputData(Trigger::OUTPUT);
            
            ReadAccess<Image> access(result);
            access();
            
            m_operator->clearOutputData(Trigger::OUTPUT);
            m_operator->setInputData(Trigger::INPUT, m_image);
            t1.join();
            
            /*** Test 2 ***/
            boost::thread t2(boost::bind(&TriggerTest::triggerDelayed, this));
            result = m_operator->getOutputData(Trigger::OUTPUT);
            
            m_operator->clearOutputData(Trigger::OUTPUT);
            m_operator->setInputData(Trigger::INPUT, m_image);
            t2.join();
            
            /*** Test 2 ***/
            boost::thread t3(boost::bind(&TriggerTest::getOutputDataInterrupted, this));
            t3.interrupt();
            t3.join();
        }
        
        void TriggerTest::testExecuteSwitchToInactive()
        {
            // wait for the output data in a separate thread
            boost::thread t(boost::bind(&TriggerTest::getOutputData, this));
            boost::this_thread::sleep(boost::posix_time::seconds(1));
            
            // deactivate the trigger
            m_operator->setParameter(Trigger::ACTIVE, Bool(false));
            
            // wait for the thread to finish
            t.join();
        }
        
        void TriggerTest::testExecuteInactive()
        {
            m_operator->setParameter(Trigger::ACTIVE, Bool(false));
            
            DataContainer result = m_operator->getOutputData(Trigger::OUTPUT);
            ReadAccess<Image> access(result);
            access();
        }
        
        void TriggerTest::getOutputDataInterrupted()
        {
            CPPUNIT_ASSERT_THROW(m_operator->getOutputData(Trigger::OUTPUT), Interrupt);
        }
        
        void TriggerTest::getOutputData()
        {
            m_operator->getOutputData(Trigger::OUTPUT);
        }
            
        void TriggerTest::triggerDelayed()
        {
            boost::this_thread::sleep(boost::posix_time::seconds(1));
            m_operator->setParameter(Trigger::TRIGGER, core::Trigger());
        }

        void TriggerTest::tearDown ( void )
        {
            delete m_operator;
        }
    }
}
