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

#include "SynchronizedOperatorKernelTest.h"

#include "TestOperator.h"

#include <stream/None.h>
#include <stream/DataContainer.h>
#include <stream/Exception.h>
#include <stream/OperatorException.h>

#include <stream/impl/SynchronizedOperatorKernel.h>

#include <boost/thread.hpp>
#include <boost/bind.hpp>

#include <cppunit/TestAssert.h>

CPPUNIT_TEST_SUITE_REGISTRATION (stream::SynchronizedOperatorKernelTest);

namespace stream
{
    using namespace impl;
    
    void SynchronizedOperatorKernelTest::setUp ( void )
    {
        m_testOperator = new TestOperator();
        m_operatorWrapper = new SynchronizedOperatorKernel(m_testOperator);
        m_operatorWrapper->initialize();
        m_operatorWrapper->activate();
        Data* data = new stream::None;
        m_container = DataContainer(data);  
    }

    void SynchronizedOperatorKernelTest::testSetInputData ( void )
    {
        DataContainer data1;
        DataContainer data2;
        
        /*** Test 1 ***/
        boost::thread t1(boost::bind(&SynchronizedOperatorKernelTest::setInputDataDelayed, this, _1), TestOperator::INPUT_1);
        CPPUNIT_ASSERT_NO_THROW(m_operatorWrapper->setInputData(TestOperator::INPUT_2, m_container));
        
        CPPUNIT_ASSERT_NO_THROW(data1 = m_operatorWrapper->getOutputData(TestOperator::OUTPUT_1));
        CPPUNIT_ASSERT_NO_THROW(data2 = m_operatorWrapper->getOutputData(TestOperator::OUTPUT_2));
        
        CPPUNIT_ASSERT_EQUAL(m_container, data1);
        CPPUNIT_ASSERT_EQUAL(m_container, data2);
        
        t1.join();
        
        CPPUNIT_ASSERT_NO_THROW(m_operatorWrapper->clearOutputData(TestOperator::OUTPUT_1));
        CPPUNIT_ASSERT_NO_THROW(m_operatorWrapper->clearOutputData(TestOperator::OUTPUT_2));
        
        /*** Test 2 ***/
        
        CPPUNIT_ASSERT_NO_THROW(m_operatorWrapper->setInputData(TestOperator::INPUT_1, m_container));
        boost::thread t2(boost::bind(&SynchronizedOperatorKernel::getOutputData, m_operatorWrapper, _1), TestOperator::OUTPUT_1);
        CPPUNIT_ASSERT_NO_THROW(m_operatorWrapper->setInputData(TestOperator::INPUT_2, m_container));
        
        t2.join();
        
        CPPUNIT_ASSERT_NO_THROW(m_operatorWrapper->clearOutputData(TestOperator::OUTPUT_1));
        CPPUNIT_ASSERT_NO_THROW(m_operatorWrapper->clearOutputData(TestOperator::OUTPUT_2));
        
        /*** Test 3 ***/
        CPPUNIT_ASSERT_NO_THROW(m_operatorWrapper->setInputData(TestOperator::INPUT_1, m_container));
        CPPUNIT_ASSERT_NO_THROW(m_operatorWrapper->setInputData(TestOperator::INPUT_2, m_container));
        CPPUNIT_ASSERT_NO_THROW(m_operatorWrapper->setInputData(TestOperator::INPUT_1, m_container));  
        CPPUNIT_ASSERT_NO_THROW(m_operatorWrapper->setInputData(TestOperator::INPUT_2, m_container));
        boost::thread t3(boost::bind(&SynchronizedOperatorKernelTest::setInputDataWithInterrupt, this, _1), TestOperator::INPUT_1);
        
        boost::this_thread::sleep(boost::posix_time::seconds(1));
        
        t3.interrupt();
        t3.join();
    }
    
    void SynchronizedOperatorKernelTest::testClearOutputData()
    {
        /*** Test 1 ***/
        CPPUNIT_ASSERT_NO_THROW(m_operatorWrapper->setInputData(TestOperator::INPUT_1, m_container));
        CPPUNIT_ASSERT_NO_THROW(m_operatorWrapper->setInputData(TestOperator::INPUT_2, m_container));
        CPPUNIT_ASSERT_NO_THROW(m_operatorWrapper->getOutputData(TestOperator::OUTPUT_1));
        CPPUNIT_ASSERT_NO_THROW(m_operatorWrapper->getOutputData(TestOperator::OUTPUT_2));
        
        CPPUNIT_ASSERT_NO_THROW(m_operatorWrapper->clearOutputData(TestOperator::OUTPUT_1));
        CPPUNIT_ASSERT_NO_THROW(m_operatorWrapper->clearOutputData(TestOperator::OUTPUT_2));
        
        CPPUNIT_ASSERT_NO_THROW(m_operatorWrapper->setInputData(TestOperator::INPUT_1, m_container));
        CPPUNIT_ASSERT_NO_THROW(m_operatorWrapper->setInputData(TestOperator::INPUT_2, m_container));
        CPPUNIT_ASSERT_NO_THROW(m_operatorWrapper->getOutputData(TestOperator::OUTPUT_1));
        CPPUNIT_ASSERT_NO_THROW(m_operatorWrapper->getOutputData(TestOperator::OUTPUT_2));
        
        m_operatorWrapper->deactivate();
        m_operatorWrapper->activate();
          
        /*** Test 2 ***/
        boost::thread t1(boost::bind(&SynchronizedOperatorKernelTest::clearOutputDataDelayed, this, _1), TestOperator::OUTPUT_1);
        boost::thread t2(boost::bind(&SynchronizedOperatorKernelTest::clearOutputDataDelayed, this, _1), TestOperator::OUTPUT_2);
        m_operatorWrapper->setInputData(TestOperator::INPUT_1, m_container);
        m_operatorWrapper->setInputData(TestOperator::INPUT_2, m_container);
        m_operatorWrapper->setInputData(TestOperator::INPUT_1, m_container);
        m_operatorWrapper->setInputData(TestOperator::INPUT_2, m_container);
        m_operatorWrapper->setInputData(TestOperator::INPUT_1, m_container);
        m_operatorWrapper->setInputData(TestOperator::INPUT_2, m_container);
        
        t1.join();
        t2.join();    
    }
    
    void SynchronizedOperatorKernelTest::testInitialize()
    {
        SynchronizedOperatorKernel* operatorWrapper = new SynchronizedOperatorKernel(new TestOperator());
        CPPUNIT_ASSERT_EQUAL(SynchronizedOperatorKernel::NONE, operatorWrapper->status());
        
        CPPUNIT_ASSERT_NO_THROW(operatorWrapper->initialize());
        CPPUNIT_ASSERT_EQUAL(SynchronizedOperatorKernel::INITIALIZED, operatorWrapper->status());
    }
    
    void SynchronizedOperatorKernelTest::testActivate()
    {
        CPPUNIT_ASSERT_THROW(m_operatorWrapper->activate(), InvalidState);
        
        m_operatorWrapper->deactivate();
        
        CPPUNIT_ASSERT_NO_THROW(m_operatorWrapper->activate());
        CPPUNIT_ASSERT_EQUAL(SynchronizedOperatorKernel::ACTIVE, m_operatorWrapper->status());
    }
    
    void SynchronizedOperatorKernelTest::testDeactivate()
    {
        m_operatorWrapper->setInputData(TestOperator::INPUT_1, m_container);
        m_operatorWrapper->setInputData(TestOperator::INPUT_2, m_container);
        
        CPPUNIT_ASSERT_NO_THROW(m_operatorWrapper->deactivate());
        CPPUNIT_ASSERT_EQUAL(SynchronizedOperatorKernel::INITIALIZED, m_operatorWrapper->status());
        
        CPPUNIT_ASSERT_NO_THROW(m_operatorWrapper->deactivate());
        
        m_operatorWrapper->activate();
        CPPUNIT_ASSERT_NO_THROW(m_operatorWrapper->setInputData(TestOperator::INPUT_1, m_container));
        CPPUNIT_ASSERT_NO_THROW(m_operatorWrapper->setInputData(TestOperator::INPUT_2, m_container));
    }

    void SynchronizedOperatorKernelTest::setInputDataDelayed ( const unsigned int id )
    {
        boost::this_thread::sleep(boost::posix_time::seconds(1));
        m_operatorWrapper->setInputData(id, m_container);
    }
    
    void SynchronizedOperatorKernelTest::clearOutputDataDelayed ( const unsigned int id )
    {
        boost::this_thread::sleep(boost::posix_time::seconds(1));
        m_operatorWrapper->clearOutputData(id);
    }
    
    void SynchronizedOperatorKernelTest::getOutputDataWithInterrupt(const unsigned int id)
    {
        CPPUNIT_ASSERT_THROW(m_operatorWrapper->getOutputData(id), Interrupt);
    }
    
    void SynchronizedOperatorKernelTest::setInputDataWithInterrupt(const unsigned int id)
    {
        CPPUNIT_ASSERT_THROW(m_operatorWrapper->setInputData(id, m_container), Interrupt);
    }
        
    void SynchronizedOperatorKernelTest::testGetOutputData()
    {
        DataContainer data1;
        DataContainer data2;
        
        /*** Test 1 ***/
        m_operatorWrapper->setInputData(TestOperator::INPUT_1, m_container);
        m_operatorWrapper->setInputData(TestOperator::INPUT_2, m_container);
        
        CPPUNIT_ASSERT_EQUAL((unsigned int)(0), m_testOperator->numExecutes());
        CPPUNIT_ASSERT_NO_THROW(data1 = m_operatorWrapper->getOutputData(TestOperator::OUTPUT_1));
        CPPUNIT_ASSERT_EQUAL((unsigned int)(1), m_testOperator->numExecutes());
        
        CPPUNIT_ASSERT_NO_THROW(data2 = m_operatorWrapper->getOutputData(TestOperator::OUTPUT_2));
        
        CPPUNIT_ASSERT_EQUAL(m_container, data1);
        CPPUNIT_ASSERT_EQUAL(m_container, data2);
        
        m_operatorWrapper->clearOutputData(TestOperator::OUTPUT_1);
        m_operatorWrapper->clearOutputData(TestOperator::OUTPUT_2);
        
        /*** Test 2 ***/
        m_operatorWrapper->setInputData(TestOperator::INPUT_1, m_container);
        
        boost::thread t1(boost::bind(&SynchronizedOperatorKernelTest::getOutputDataWithInterrupt, this, _1), TestOperator::OUTPUT_1);
        boost::this_thread::sleep(boost::posix_time::seconds(1));
        
        t1.interrupt();
        t1.join();      
    }

    void SynchronizedOperatorKernelTest::testGetParameter()
    {
        CPPUNIT_ASSERT_NO_THROW(m_operatorWrapper->getParameter(TestOperator::SLEEP_TIME));
        
        m_operatorWrapper->deactivate();
        
        const Data& value = m_operatorWrapper->getParameter(TestOperator::SLEEP_TIME);
        CPPUNIT_ASSERT_NO_THROW(dynamic_cast<const UInt32&>(value));
        
        CPPUNIT_ASSERT_THROW(m_operatorWrapper->getParameter(-1), WrongParameterId);
    }

    void SynchronizedOperatorKernelTest::testSetParameter()
    {
        UInt32 value(2000);
        CPPUNIT_ASSERT_THROW(m_operatorWrapper->setParameter(TestOperator::SLEEP_TIME, value), ParameterAccessViolation);
        
        m_operatorWrapper->deactivate();
        
        CPPUNIT_ASSERT_NO_THROW(m_operatorWrapper->setParameter(TestOperator::SLEEP_TIME, value)); 
        const Data& testValue = m_operatorWrapper->getParameter(TestOperator::SLEEP_TIME);
        CPPUNIT_ASSERT_EQUAL(UInt32(2000), dynamic_cast<const UInt32&>(testValue));
        
        CPPUNIT_ASSERT_THROW(m_operatorWrapper->setParameter(-1, value), WrongParameterId);
        
        UInt16 wrongType;
        CPPUNIT_ASSERT_THROW(m_operatorWrapper->setParameter(TestOperator::SLEEP_TIME, wrongType), WrongParameterType);
    }
    
    void SynchronizedOperatorKernelTest::testGetParameterStatusNone()
    {
        SynchronizedOperatorKernel* wrapper = new SynchronizedOperatorKernel(new TestOperator());
        
        CPPUNIT_ASSERT_NO_THROW(wrapper->getParameter(TestOperator::BUFFER_SIZE));
    }

    void SynchronizedOperatorKernelTest::testSetParameterStatusNone()
    {
        UInt32 value(2000);
        SynchronizedOperatorKernel* wrapper = new SynchronizedOperatorKernel(new TestOperator());
        
        CPPUNIT_ASSERT_NO_THROW(wrapper->setParameter(TestOperator::BUFFER_SIZE, value));
    
        wrapper->initialize();
        CPPUNIT_ASSERT_THROW(wrapper->setParameter(TestOperator::BUFFER_SIZE, value), ParameterAccessViolation);
    }
    
    void SynchronizedOperatorKernelTest::tearDown ( void )
    {
        delete m_operatorWrapper;
    }
} 