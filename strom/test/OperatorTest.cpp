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

#include "OperatorTest.h"

#include "TestOperator.h"

#include <strom/None.h>
#include <strom/DataContainer.h>
#include <strom/Exception.h>
#include <strom/OperatorException.h>

#include <strom/OperatorTester.h>

#include <boost/thread.hpp>
#include <boost/bind.hpp>

#include <cppunit/TestAssert.h>

CPPUNIT_TEST_SUITE_REGISTRATION (strom::OperatorTest);

namespace strom
{
    using namespace impl;
    
    void OperatorTest::setUp ( void )
    {
        m_testOperator = new TestOperator();
        m_operator = new OperatorTester(m_testOperator);
        m_operator->initialize();
        m_operator->activate();
        Data* data = new strom::None;
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
        
        boost::this_thread::sleep(boost::posix_time::seconds(1));
        
        t3.interrupt();
        t3.join();
                
        /*** Test 4 ***/
        m_operator->deactivate();
        CPPUNIT_ASSERT_THROW(m_operator->setInputData(TestOperator::INPUT_1, m_container), InvalidState);
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
        CPPUNIT_ASSERT_THROW(m_operator->clearOutputData(TestOperator::INPUT_1), InvalidState);
    }
    
    void OperatorTest::testInitialize()
    {
        OperatorTester* op = new OperatorTester(new TestOperator());
        CPPUNIT_ASSERT_EQUAL(OperatorTester::NONE, op->status());
        
        CPPUNIT_ASSERT_NO_THROW(op->initialize());
        CPPUNIT_ASSERT_EQUAL(OperatorTester::INITIALIZED, op->status());
    }
    
    void OperatorTest::testActivate()
    {
        CPPUNIT_ASSERT_THROW(m_operator->activate(), InvalidState);
        
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
        boost::this_thread::sleep(boost::posix_time::seconds(1));
        m_operator->setInputData(id, m_container);
    }
    
    void OperatorTest::clearOutputDataDelayed ( const unsigned int id )
    {
        boost::this_thread::sleep(boost::posix_time::seconds(1));
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
        boost::this_thread::sleep(boost::posix_time::seconds(1));
        
        t1.interrupt();
        t1.join();
        
        /*** Test 4 ***/
        m_operator->deactivate();
        CPPUNIT_ASSERT_THROW(data1 = m_operator->getOutputData(TestOperator::OUTPUT_1), InvalidState);  
    }

    void OperatorTest::testGetParameter()
    {
        CPPUNIT_ASSERT_NO_THROW(m_operator->getParameter(TestOperator::SLEEP_TIME));
        
        m_operator->deactivate();
        
        const Data& value = m_operator->getParameter(TestOperator::SLEEP_TIME);
        CPPUNIT_ASSERT_NO_THROW(dynamic_cast<const UInt32&>(value));
        
        CPPUNIT_ASSERT_THROW(m_operator->getParameter(-1), WrongParameterId);
    }

    void OperatorTest::testGetParameterCast()
    {
        CPPUNIT_ASSERT_NO_THROW(m_operator->getParameter<UInt32>(TestOperator::SLEEP_TIME));
    }
    
    void OperatorTest::testGetParameterWrongCast()
    {
        CPPUNIT_ASSERT_THROW(m_operator->getParameter<UInt16>(TestOperator::SLEEP_TIME), BadCast);
    }

    void OperatorTest::testSetParameter()
    {
        UInt32 value(2000);
        CPPUNIT_ASSERT_THROW(m_operator->setParameter(TestOperator::SLEEP_TIME, value), ParameterAccessViolation);
        
        m_operator->deactivate();
        
        CPPUNIT_ASSERT_NO_THROW(m_operator->setParameter(TestOperator::SLEEP_TIME, value)); 
        const Data& testValue = m_operator->getParameter(TestOperator::SLEEP_TIME);
        CPPUNIT_ASSERT_EQUAL(UInt32(2000), dynamic_cast<const UInt32&>(testValue));
        
        CPPUNIT_ASSERT_THROW(m_operator->setParameter(-1, value), WrongParameterId);
        
        UInt16 wrongType;
        CPPUNIT_ASSERT_THROW(m_operator->setParameter(TestOperator::SLEEP_TIME, wrongType), WrongParameterType);
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
    
    void OperatorTest::tearDown ( void )
    {
        delete m_operator;
    }
} 