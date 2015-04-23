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
#include "stromx/runtime/DataContainer.h"
#include "stromx/runtime/Exception.h"
#include "stromx/runtime/Factory.h"
#include "stromx/runtime/None.h"
#include "stromx/runtime/OperatorException.h"
#include "stromx/runtime/OperatorTester.h"
#include "stromx/runtime/test/OperatorTest.h"
#include "stromx/runtime/test/TestOperator.h"

CPPUNIT_TEST_SUITE_REGISTRATION (stromx::runtime::OperatorTest);

namespace stromx
{
    namespace runtime
    {
        using namespace impl;
        
        void OperatorTest::setUp ( void )
        {
            m_testOperator = new TestOperator();
            m_operator = new OperatorTester(m_testOperator);
            m_operator->initialize();
            m_operator->activate();
            m_operator->setName("Operator");
            Data* data = new runtime::None;
            m_container = DataContainer(data);  
        }

        void OperatorTest::testSetInputData ( void )
        {
            DataContainer data1;
            DataContainer data2;
            
            /*** Test 1 ***/
            boost::thread t1(boost::bind(&OperatorTest::setInputDataDelayed, this, _1), TestOperator::INPUT_1);
            CPPUNIT_ASSERT_NO_THROW(m_operator->setInputData(TestOperator::INPUT_2, m_container));
            
            CPPUNIT_ASSERT_NO_THROW(data1 = m_operator->getOutputData(TestOperator::OUTPUT_1));
            CPPUNIT_ASSERT_NO_THROW(data2 = m_operator->getOutputData(TestOperator::OUTPUT_2));
            
            CPPUNIT_ASSERT_EQUAL(m_container, data1);
            CPPUNIT_ASSERT_EQUAL(m_container, data2);
            
            t1.join();
            
            CPPUNIT_ASSERT_NO_THROW(m_operator->clearOutputData(TestOperator::OUTPUT_1));
            CPPUNIT_ASSERT_NO_THROW(m_operator->clearOutputData(TestOperator::OUTPUT_2));
            
            /*** Test 2 ***/
            
            CPPUNIT_ASSERT_NO_THROW(m_operator->setInputData(TestOperator::INPUT_1, m_container));
            boost::thread t2(boost::bind(&OperatorTester::getOutputData, m_operator, _1), TestOperator::OUTPUT_1);
            CPPUNIT_ASSERT_NO_THROW(m_operator->setInputData(TestOperator::INPUT_2, m_container));
            
            t2.join();
            
            CPPUNIT_ASSERT_NO_THROW(m_operator->clearOutputData(TestOperator::OUTPUT_1));
            CPPUNIT_ASSERT_NO_THROW(m_operator->clearOutputData(TestOperator::OUTPUT_2));
            
            /*** Test 3 ***/
            CPPUNIT_ASSERT_NO_THROW(m_operator->setInputData(TestOperator::INPUT_1, m_container));
            CPPUNIT_ASSERT_NO_THROW(m_operator->setInputData(TestOperator::INPUT_2, m_container));
            CPPUNIT_ASSERT_NO_THROW(m_operator->setInputData(TestOperator::INPUT_1, m_container));  
            CPPUNIT_ASSERT_NO_THROW(m_operator->setInputData(TestOperator::INPUT_2, m_container));
            boost::thread t3(boost::bind(&OperatorTest::setInputDataWithInterrupt, this, _1), TestOperator::INPUT_1);
            
            boost::this_thread::sleep_for(boost::chrono::seconds(1));
            
            t3.interrupt();
            t3.join();
                    
            /*** Test 4 ***/
            m_operator->deactivate();
            CPPUNIT_ASSERT_THROW(m_operator->setInputData(TestOperator::INPUT_1, m_container), WrongOperatorState);
        }
        
        void OperatorTest::testClearOutputData()
        {
            /*** Test 1 ***/
            CPPUNIT_ASSERT_NO_THROW(m_operator->setInputData(TestOperator::INPUT_1, m_container));
            CPPUNIT_ASSERT_NO_THROW(m_operator->setInputData(TestOperator::INPUT_2, m_container));
            CPPUNIT_ASSERT_NO_THROW(m_operator->getOutputData(TestOperator::OUTPUT_1));
            CPPUNIT_ASSERT_NO_THROW(m_operator->getOutputData(TestOperator::OUTPUT_2));
            
            CPPUNIT_ASSERT_NO_THROW(m_operator->clearOutputData(TestOperator::OUTPUT_1));
            CPPUNIT_ASSERT_NO_THROW(m_operator->clearOutputData(TestOperator::OUTPUT_2));
            
            CPPUNIT_ASSERT_NO_THROW(m_operator->setInputData(TestOperator::INPUT_1, m_container));
            CPPUNIT_ASSERT_NO_THROW(m_operator->setInputData(TestOperator::INPUT_2, m_container));
            CPPUNIT_ASSERT_NO_THROW(m_operator->getOutputData(TestOperator::OUTPUT_1));
            CPPUNIT_ASSERT_NO_THROW(m_operator->getOutputData(TestOperator::OUTPUT_2));
            
            m_operator->deactivate();
            m_operator->activate();
            
            /*** Test 2 ***/
            boost::thread t1(boost::bind(&OperatorTest::clearOutputDataDelayed, this, _1), TestOperator::OUTPUT_1);
            boost::thread t2(boost::bind(&OperatorTest::clearOutputDataDelayed, this, _1), TestOperator::OUTPUT_2);
            m_operator->setInputData(TestOperator::INPUT_1, m_container);
            m_operator->setInputData(TestOperator::INPUT_2, m_container);
            m_operator->setInputData(TestOperator::INPUT_1, m_container);
            m_operator->setInputData(TestOperator::INPUT_2, m_container);
            m_operator->setInputData(TestOperator::INPUT_1, m_container);
            m_operator->setInputData(TestOperator::INPUT_2, m_container);
            
            t1.join();
            t2.join();    
            
            /*** Test 3 ***/
            m_operator->deactivate();
            CPPUNIT_ASSERT_THROW(m_operator->clearOutputData(TestOperator::INPUT_1), WrongOperatorState);
        }
        
        void OperatorTest::testInitialize()
        {
            OperatorTester* op = new OperatorTester(new TestOperator());
            CPPUNIT_ASSERT_EQUAL((unsigned int)(1), (unsigned int)(op->info().parameters().size()));
            CPPUNIT_ASSERT_EQUAL((unsigned int)(1), (unsigned int)(op->info().inputs().size()));
            CPPUNIT_ASSERT_EQUAL((unsigned int)(1), (unsigned int)(op->info().outputs().size()));
            CPPUNIT_ASSERT_EQUAL(OperatorTester::NONE, op->status());
            
            CPPUNIT_ASSERT_NO_THROW(op->initialize());
            CPPUNIT_ASSERT_EQUAL(OperatorTester::INITIALIZED, op->status());
            CPPUNIT_ASSERT_EQUAL((unsigned int)(TestOperator::NUM_PARAMETERS),
                                 (unsigned int)(op->info().parameters().size()));
            CPPUNIT_ASSERT_EQUAL((unsigned int)(2), (unsigned int)(op->info().inputs().size()));
            CPPUNIT_ASSERT_EQUAL((unsigned int)(2), (unsigned int)(op->info().outputs().size()));
        }
        
        void OperatorTest::testDeinitialize()
        {
            OperatorTester* op = new OperatorTester(new TestOperator());
            CPPUNIT_ASSERT_NO_THROW(op->deinitialize());
            
            op->initialize();
            CPPUNIT_ASSERT_NO_THROW(op->deinitialize());
            CPPUNIT_ASSERT_EQUAL(OperatorTester::NONE, op->status());
            CPPUNIT_ASSERT_EQUAL((unsigned int)(1), (unsigned int)(op->info().parameters().size()));
            CPPUNIT_ASSERT_EQUAL((unsigned int)(1), (unsigned int)(op->info().inputs().size()));
            CPPUNIT_ASSERT_EQUAL((unsigned int)(1), (unsigned int)(op->info().outputs().size()));
            
            op->initialize();
            op->activate();
            CPPUNIT_ASSERT_NO_THROW(op->deinitialize());
            CPPUNIT_ASSERT_EQUAL(OperatorTester::NONE, op->status());
        }
        
        void OperatorTest::testActivate()
        {
            try
            {
                m_operator->activate();
            }
            catch(OperatorError& e)
            {
                CPPUNIT_ASSERT_EQUAL(std::string("Operator"), e.name());
            }
            
            CPPUNIT_ASSERT_THROW(m_operator->activate(), WrongOperatorState);
            
            m_operator->deactivate();
            
            CPPUNIT_ASSERT_NO_THROW(m_operator->activate());
            CPPUNIT_ASSERT_EQUAL(OperatorTester::ACTIVE, m_operator->status());
        }
        
        void OperatorTest::testDeactivate()
        {
            m_operator->setInputData(TestOperator::INPUT_1, m_container);
            m_operator->setInputData(TestOperator::INPUT_2, m_container);
            
            CPPUNIT_ASSERT_NO_THROW(m_operator->deactivate());
            CPPUNIT_ASSERT_EQUAL(OperatorTester::INITIALIZED, m_operator->status());
            
            CPPUNIT_ASSERT_NO_THROW(m_operator->deactivate());
            
            m_operator->activate();
            CPPUNIT_ASSERT_NO_THROW(m_operator->setInputData(TestOperator::INPUT_1, m_container));
            CPPUNIT_ASSERT_NO_THROW(m_operator->setInputData(TestOperator::INPUT_2, m_container));
        }

        void OperatorTest::setInputDataDelayed ( const unsigned int id )
        {
            boost::this_thread::sleep_for(boost::chrono::seconds(1));
            m_operator->setInputData(id, m_container);
        }
        
        void OperatorTest::clearOutputDataDelayed ( const unsigned int id )
        {
            boost::this_thread::sleep_for(boost::chrono::seconds(1));
            m_operator->clearOutputData(id);
        }
        
        void OperatorTest::getOutputDataWithInterrupt(const unsigned int id)
        {
            CPPUNIT_ASSERT_THROW(m_operator->getOutputData(id), Interrupt);
        }
        
        void OperatorTest::setInputDataWithInterrupt(const unsigned int id)
        {
            CPPUNIT_ASSERT_THROW(m_operator->setInputData(id, m_container), Interrupt);
        }
            
        void OperatorTest::testGetOutputData()
        {
            DataContainer data1;
            DataContainer data2;
            
            /*** Test 1 ***/
            m_operator->setInputData(TestOperator::INPUT_1, m_container);
            m_operator->setInputData(TestOperator::INPUT_2, m_container);
            
            CPPUNIT_ASSERT_EQUAL((unsigned int)(0), m_testOperator->numExecutes());
            CPPUNIT_ASSERT_NO_THROW(data1 = m_operator->getOutputData(TestOperator::OUTPUT_1));
            CPPUNIT_ASSERT_EQUAL((unsigned int)(1), m_testOperator->numExecutes());
            
            CPPUNIT_ASSERT_NO_THROW(data2 = m_operator->getOutputData(TestOperator::OUTPUT_2));
            
            CPPUNIT_ASSERT_EQUAL(m_container, data1);
            CPPUNIT_ASSERT_EQUAL(m_container, data2);
            
            m_operator->clearOutputData(TestOperator::OUTPUT_1);
            m_operator->clearOutputData(TestOperator::OUTPUT_2);
            
            /*** Test 2 ***/
            m_operator->setInputData(TestOperator::INPUT_1, m_container);
            
            boost::thread t1(boost::bind(&OperatorTest::getOutputDataWithInterrupt, this, _1), TestOperator::OUTPUT_1);
            boost::this_thread::sleep_for(boost::chrono::seconds(1));
            
            t1.interrupt();
            t1.join();
            
            /*** Test 4 ***/
            m_operator->deactivate();
            CPPUNIT_ASSERT_THROW(data1 = m_operator->getOutputData(TestOperator::OUTPUT_1), WrongOperatorState);  
        }

        void OperatorTest::testGetParameter()
        {
            CPPUNIT_ASSERT_NO_THROW(m_operator->getParameter(TestOperator::SLEEP_TIME));
            
            m_operator->deactivate();
            
            DataRef value = m_operator->getParameter(TestOperator::SLEEP_TIME);
            CPPUNIT_ASSERT_NO_THROW(data_cast<UInt32>(value));
            
            CPPUNIT_ASSERT_THROW(m_operator->getParameter(-1), WrongParameterId);
        }

        void OperatorTest::testGetParameterNoTimeout()
        {
            CPPUNIT_ASSERT_NO_THROW(m_operator->getParameter(TestOperator::SLEEP_TIME, 300));
        }
        
        void OperatorTest::testGetParameterTimeout()
        { 
            setWaitingTime(1000);
            boost::thread t(boost::bind(&Operator::getOutputData, m_operator, TestOperator::OUTPUT_1));
            
            boost::this_thread::sleep_for(boost::chrono::milliseconds(500));
            CPPUNIT_ASSERT_THROW(m_operator->getParameter(TestOperator::TEST_DATA, 100), Timeout);
            
            t.join();
        }

        void OperatorTest::testSetParameter()
        {
            UInt32 value(2000);
            CPPUNIT_ASSERT_THROW(m_operator->setParameter(TestOperator::SLEEP_TIME, value), ParameterAccessViolation);
            
            m_operator->deactivate();
            
            CPPUNIT_ASSERT_NO_THROW(m_operator->setParameter(TestOperator::SLEEP_TIME, value)); 
            DataRef testValue = m_operator->getParameter(TestOperator::SLEEP_TIME);
            CPPUNIT_ASSERT_EQUAL(UInt32(2000), data_cast<UInt32>(testValue));
            
            CPPUNIT_ASSERT_THROW(m_operator->setParameter(-1, value), WrongParameterId);
            
            UInt16 wrongType;
            CPPUNIT_ASSERT_THROW(m_operator->setParameter(TestOperator::SLEEP_TIME, wrongType), WrongParameterType);
        }

        void OperatorTest::testSetParameterNoTimeout()
        {
            CPPUNIT_ASSERT_NO_THROW(m_operator->setParameter(TestOperator::TEST_DATA, TestData(), 100));
        }
        
        void OperatorTest::testSetParameterTimeout()
        { 
            setWaitingTime(1000);
            boost::thread t(boost::bind(&Operator::getOutputData, m_operator, TestOperator::OUTPUT_1));
            
            boost::this_thread::sleep_for(boost::chrono::milliseconds(500));
            CPPUNIT_ASSERT_THROW(m_operator->setParameter(TestOperator::TEST_DATA, TestData(), 100), Timeout);
            
            t.join();
        }
        
        void OperatorTest::testGetParameterStatusNone()
        {
            OperatorTester* wrapper = new OperatorTester(new TestOperator());
            
            CPPUNIT_ASSERT_NO_THROW(wrapper->getParameter(TestOperator::BUFFER_SIZE));
        }

        void OperatorTest::testSetParameterStatusNone()
        {
            UInt32 value(2000);
            OperatorTester* wrapper = new OperatorTester(new TestOperator());
            
            CPPUNIT_ASSERT_NO_THROW(wrapper->setParameter(TestOperator::BUFFER_SIZE, value));
        
            wrapper->initialize();
            CPPUNIT_ASSERT_THROW(wrapper->setParameter(TestOperator::BUFFER_SIZE, value), ParameterAccessViolation);
        }
        
        void OperatorTest::testAddObserver()
        {
            CPPUNIT_ASSERT_NO_THROW(m_operator->addObserver(&m_observer1));
            CPPUNIT_ASSERT_NO_THROW(m_operator->addObserver(&m_observer2));
            CPPUNIT_ASSERT_THROW(m_operator->addObserver(0), WrongArgument);
        }

        void OperatorTest::testRemoveObserver()
        {
            m_operator->addObserver(&m_observer1);
            m_operator->addObserver(&m_observer2);
            
            CPPUNIT_ASSERT_NO_THROW(m_operator->removeObserver(&m_observer1));
            CPPUNIT_ASSERT_THROW(m_operator->removeObserver(&m_observer1), WrongArgument);
            CPPUNIT_ASSERT_NO_THROW(m_operator->removeObserver(&m_observer2));
        }
        
        void OperatorTest::testObserver()
        {
            m_operator->addObserver(&m_observer1);
            m_operator->setInputData(TestOperator::INPUT_1, m_container);
            
            CPPUNIT_ASSERT_EQUAL(Connector::INPUT, m_observer1.lastConnector().type());
            CPPUNIT_ASSERT_EQUAL((unsigned int)(TestOperator::INPUT_1), m_observer1.lastConnector().id());
            CPPUNIT_ASSERT_EQUAL(static_cast<const Operator*>(m_operator), m_observer1.lastConnector().op());
            CPPUNIT_ASSERT_EQUAL(DataContainer(), m_observer1.lastOldData());
            CPPUNIT_ASSERT_EQUAL(m_container, m_observer1.lastNewData());
            
            m_operator->setInputData(TestOperator::INPUT_2, m_container);
            m_operator->getOutputData(TestOperator::OUTPUT_1);
            
            CPPUNIT_ASSERT_EQUAL(Connector::OUTPUT, m_observer1.lastConnector().type());
            CPPUNIT_ASSERT_EQUAL((unsigned int)(TestOperator::OUTPUT_2), m_observer1.lastConnector().id());
            CPPUNIT_ASSERT_EQUAL(static_cast<const Operator*>(m_operator), m_observer1.lastConnector().op());
            CPPUNIT_ASSERT(m_observer1.lastOldData().empty());
            CPPUNIT_ASSERT(! m_observer1.lastNewData().empty());
            
            m_operator->clearOutputData(TestOperator::OUTPUT_2);
            
            CPPUNIT_ASSERT_EQUAL(Connector::OUTPUT, m_observer1.lastConnector().type());
            CPPUNIT_ASSERT_EQUAL((unsigned int)(TestOperator::OUTPUT_2), m_observer1.lastConnector().id());
            CPPUNIT_ASSERT_EQUAL(static_cast<const Operator*>(m_operator), m_observer1.lastConnector().op());
            CPPUNIT_ASSERT(! m_observer1.lastOldData().empty());
            CPPUNIT_ASSERT(m_observer1.lastNewData().empty());
        }
        
        void OperatorTest::testTwoObservers()
        {
            m_operator->addObserver(&m_observer1);
            m_operator->addObserver(&m_observer2);
            m_operator->setInputData(TestOperator::INPUT_1, m_container);
            
            CPPUNIT_ASSERT_EQUAL(Connector::INPUT, m_observer1.lastConnector().type());
            CPPUNIT_ASSERT_EQUAL((unsigned int)(TestOperator::INPUT_1), m_observer1.lastConnector().id());
            CPPUNIT_ASSERT_EQUAL(static_cast<const Operator*>(m_operator), m_observer1.lastConnector().op());
            CPPUNIT_ASSERT_EQUAL(DataContainer(), m_observer1.lastOldData());
            CPPUNIT_ASSERT_EQUAL(m_container, m_observer1.lastNewData());
            
            CPPUNIT_ASSERT_EQUAL(Connector::INPUT, m_observer2.lastConnector().type());
            CPPUNIT_ASSERT_EQUAL((unsigned int)(TestOperator::INPUT_1), m_observer2.lastConnector().id());
            CPPUNIT_ASSERT_EQUAL(static_cast<const Operator*>(m_operator), m_observer2.lastConnector().op());
            CPPUNIT_ASSERT_EQUAL(DataContainer(), m_observer2.lastOldData());
            CPPUNIT_ASSERT_EQUAL(m_container, m_observer2.lastNewData());

        }

        void OperatorTest::tearDown ( void )
        {
            delete m_operator;
        }
        
        void OperatorTest::setWaitingTime(const unsigned int timeInMs)
        {
            m_operator->deactivate();
            m_operator->setParameter(TestOperator::SLEEP_TIME, UInt32(timeInMs));
            m_operator->activate();
            m_operator->setInputData(TestOperator::INPUT_1, m_container);
            m_operator->setInputData(TestOperator::INPUT_2, m_container);
        }
        
        void OperatorTest::TestObserver::observe(const Connector& connector, 
                                                 const DataContainer& oldData,
                                                 const DataContainer& newData,
                                                 const Thread* const thread) const
        {
            m_lastConnector = connector;
            m_lastOldData = oldData;
            m_lastNewData = newData;
            m_thread = thread;
        }
        
        void OperatorTest::testSetFactory()
        {
            CPPUNIT_ASSERT_EQUAL((const AbstractFactory*)(0), m_operator->factory());
            
            Factory factory;
            m_operator->deactivate();
            m_operator->setFactory(&factory);
            
            CPPUNIT_ASSERT_EQUAL((const AbstractFactory*)(&factory), m_operator->factory());
        }
        
        void OperatorTest::testSetFactoryWrongState()
        {
            Factory factory;
            CPPUNIT_ASSERT_THROW(m_operator->setFactory(&factory), WrongState);
        }
    }
}
