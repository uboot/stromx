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
#include "stromx/runtime/Exception.h"
#include "stromx/runtime/OperatorTester.h"
#include "stromx/runtime/ReadAccess.h"
#include "stromx/runtime/Block.h"
#include "stromx/runtime/TriggerData.h"
#include "stromx/runtime/test/BlockTest.h"

CPPUNIT_TEST_SUITE_REGISTRATION (stromx::runtime::BlockTest);

namespace stromx
{
    using namespace runtime;

    namespace runtime
    {
        void BlockTest::setUp ( void )
        {
            m_operator = new runtime::OperatorTester(new Block());
            m_operator->initialize();
            m_operator->activate();
            m_data = DataContainer(new UInt32(4));
            m_operator->setInputData(Block::INPUT, m_data);
        }
        
        void BlockTest::testExecuteActive()
        {
            /*** Test 1 ***/
            boost::thread t1(boost::bind(&BlockTest::triggerDelayed, this));
            DataContainer result = m_operator->getOutputData(Block::OUTPUT);
            
            ReadAccess<UInt32> access(result);
            access();
            
            m_operator->clearOutputData(Block::OUTPUT);
            m_operator->setInputData(Block::INPUT, m_data);
            t1.join();
            
            /*** Test 2 ***/
            boost::thread t2(boost::bind(&BlockTest::triggerDelayed, this));
            result = m_operator->getOutputData(Block::OUTPUT);
            
            m_operator->clearOutputData(Block::OUTPUT);
            m_operator->setInputData(Block::INPUT, m_data);
            t2.join();
            
            /*** Test 2 ***/
            boost::thread t3(boost::bind(&BlockTest::getOutputDataInterrupted, this));
            t3.interrupt();
            t3.join();
        }
        
        void BlockTest::testExecuteSwitchToInactive()
        {
            // wait for the output data in a separate thread
            boost::thread t(boost::bind(&BlockTest::getOutputData, this));
            boost::this_thread::sleep_for(boost::chrono::seconds(1));
            
            // trigger will always operatore immediately
            m_operator->setParameter(Block::STATE, Enum(Block::PASS_ALWAYS));
            
            // wait for the thread to finish
            t.join();
        }
        
        void BlockTest::testExecuteInactive()
        {
            m_operator->setParameter(Block::STATE, Enum(Block::PASS_ALWAYS));
            
            DataContainer result = m_operator->getOutputData(Block::OUTPUT);
            ReadAccess<UInt32> access(result);
            access();
        }
        
        void BlockTest::testExecuteTriggerInput()
        {
            // initialize the operator with trigger input
            m_operator->deinitialize();
            m_operator->setParameter(Block::TRIGGER_INPUT, Bool(true));
            m_operator->initialize();
            m_operator->activate();
            
            // wait for the output data in a separate thread
            boost::thread t(boost::bind(&BlockTest::getOutputData, this));
            boost::this_thread::sleep_for(boost::chrono::seconds(1));
            
            // set the input data
            m_operator->setInputData(Block::INPUT, m_data);
            
            // send the trigger
            DataContainer trigger(new TriggerData);
            m_operator->setInputData(Block::TRIGGER_DATA, trigger);
            
            // wait for the thread to finish
            t.join();
        }
        
        void BlockTest::getOutputDataInterrupted()
        {
            CPPUNIT_ASSERT_THROW(m_operator->getOutputData(Block::OUTPUT), Interrupt);
        }
        
        void BlockTest::getOutputData()
        {
            m_operator->getOutputData(Block::OUTPUT);
        }
            
        void BlockTest::triggerDelayed()
        {
            boost::this_thread::sleep_for(boost::chrono::seconds(1));
            m_operator->setParameter(Block::TRIGGER, runtime::TriggerData());
        }

        void BlockTest::tearDown ( void )
        {
            delete m_operator;
        }
    }
}
