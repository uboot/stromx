#include "OperatorWrapperTest.h"

#include "TestOperator.h"

#include <stream/OperatorWrapper.h>
#include <stream/None.h>
#include <stream/DataContainer.h>
#include <stream/Exception.h>

#include <boost/thread.hpp>
#include <boost/bind.hpp>

#include <cppunit/TestAssert.h>

CPPUNIT_TEST_SUITE_REGISTRATION (stream::OperatorWrapperTest);

namespace stream
{
    void OperatorWrapperTest::setUp ( void )
    {
        m_testOperator = new TestOperator(0);
        m_operator = new OperatorWrapper(m_testOperator);
        m_operator->activate();
        Data* data = new stream::None;
        m_container = new DataContainer(data);  
    }

    void OperatorWrapperTest::setInputDataTest ( void )
    {
        DataContainer* data1;
        DataContainer* data2;
        
        /*** Test 1 ***/
        CPPUNIT_ASSERT_NO_THROW(m_operator->setInputData(TestOperator::INPUT_1, m_container));
        CPPUNIT_ASSERT_EQUAL((unsigned int)(0), m_testOperator->numExecutes());
        
        CPPUNIT_ASSERT_NO_THROW(m_operator->setInputData(TestOperator::INPUT_2, m_container));
        CPPUNIT_ASSERT_EQUAL((unsigned int)(0), m_testOperator->numExecutes());
        
        CPPUNIT_ASSERT_NO_THROW(data1 = m_operator->getOutputData(TestOperator::OUTPUT_1));
        CPPUNIT_ASSERT_EQUAL((unsigned int)(1), m_testOperator->numExecutes());
        
        CPPUNIT_ASSERT_NO_THROW(data2 = m_operator->getOutputData(TestOperator::OUTPUT_2));
        CPPUNIT_ASSERT_EQUAL((unsigned int)(1), m_testOperator->numExecutes());
        
        CPPUNIT_ASSERT_EQUAL(m_container, data1);
        CPPUNIT_ASSERT_EQUAL(m_container, data2);
        
        CPPUNIT_ASSERT_NO_THROW(m_operator->clearOutputData(TestOperator::OUTPUT_1));
        CPPUNIT_ASSERT_NO_THROW(m_operator->clearOutputData(TestOperator::OUTPUT_2));
        
        /*** Test 2 ***/
        boost::thread t1(boost::bind(&OperatorWrapperTest::setInputDataDelayed, this, _1), TestOperator::INPUT_1);
        CPPUNIT_ASSERT_NO_THROW(m_operator->setInputData(TestOperator::INPUT_2, m_container));
        
        CPPUNIT_ASSERT_NO_THROW(data1 = m_operator->getOutputData(TestOperator::OUTPUT_1));
        CPPUNIT_ASSERT_NO_THROW(data2 = m_operator->getOutputData(TestOperator::OUTPUT_2));
        
        CPPUNIT_ASSERT_EQUAL(m_container, data1);
        CPPUNIT_ASSERT_EQUAL(m_container, data2);
        
        t1.join();
        
        CPPUNIT_ASSERT_NO_THROW(m_operator->clearOutputData(TestOperator::OUTPUT_1));
        CPPUNIT_ASSERT_NO_THROW(m_operator->clearOutputData(TestOperator::OUTPUT_2));
        
        /*** Test 3 ***/
        
        CPPUNIT_ASSERT_NO_THROW(m_operator->setInputData(TestOperator::INPUT_1, m_container));
        boost::thread t2(boost::bind(&OperatorWrapper::getOutputData, m_operator, _1), TestOperator::OUTPUT_1);
        CPPUNIT_ASSERT_NO_THROW(m_operator->setInputData(TestOperator::INPUT_2, m_container));
        
        t2.join();
        
        CPPUNIT_ASSERT_NO_THROW(m_operator->clearOutputData(TestOperator::OUTPUT_1));
        CPPUNIT_ASSERT_NO_THROW(m_operator->clearOutputData(TestOperator::OUTPUT_2));
        
        /*** Test 4 ***/
        CPPUNIT_ASSERT_NO_THROW(m_operator->setInputData(TestOperator::INPUT_1, m_container));
        CPPUNIT_ASSERT_NO_THROW(m_operator->setInputData(TestOperator::INPUT_2, m_container));
        CPPUNIT_ASSERT_NO_THROW(m_operator->setInputData(TestOperator::INPUT_1, m_container));
    }
    
    void OperatorWrapperTest::clearOutputDataTest()
    {
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
    }
    
    void OperatorWrapperTest::activateTest()
    {
        CPPUNIT_ASSERT_EQUAL(OperatorWrapper::ACTIVE, m_operator->status());
        
        CPPUNIT_ASSERT_NO_THROW(m_operator->setInputData(TestOperator::INPUT_1, m_container));
        CPPUNIT_ASSERT_NO_THROW(m_operator->setInputData(TestOperator::INPUT_2, m_container));
        
        CPPUNIT_ASSERT_NO_THROW(m_operator->deactivate());
        
        CPPUNIT_ASSERT_EQUAL(OperatorWrapper::INACTIVE, m_operator->status());
        
        CPPUNIT_ASSERT_NO_THROW(m_operator->activate());
        
        CPPUNIT_ASSERT_EQUAL(OperatorWrapper::ACTIVE, m_operator->status());
    }
    
    void OperatorWrapperTest::deactivateTest()
    {
        CPPUNIT_ASSERT_NO_THROW(m_operator->deactivate());
        
        CPPUNIT_ASSERT_EQUAL(OperatorWrapper::INACTIVE, m_operator->status());
        
        CPPUNIT_ASSERT_NO_THROW(m_operator->activate());
        
        CPPUNIT_ASSERT_NO_THROW(m_operator->setInputData(TestOperator::INPUT_1, m_container));
        CPPUNIT_ASSERT_NO_THROW(m_operator->setInputData(TestOperator::INPUT_2, m_container));
    }

    
    void OperatorWrapperTest::setInputDataDelayed ( const unsigned int id )
    {
        boost::this_thread::sleep(boost::posix_time::seconds(1));
        m_operator->setInputData(id, m_container);
    }
    
    void OperatorWrapperTest::clearOutputDataDelayed ( const unsigned int id )
    {
        boost::this_thread::sleep(boost::posix_time::seconds(1));
        m_operator->clearOutputData(id);
    }
    
    void OperatorWrapperTest::getOutputDataWithInterrupt(const unsigned int id)
    {
        CPPUNIT_ASSERT_THROW(m_operator->getOutputData(id), InterruptException);
    }
    
    void OperatorWrapperTest::setInputDataWithInterrupt(const unsigned int id)
    {
        CPPUNIT_ASSERT_THROW(m_operator->setInputData(id, m_container), InterruptException);
    }
        
    void OperatorWrapperTest::getOutputDataTest()
    {
        DataContainer* data1;
        
        /*** Test 1 ***/
        CPPUNIT_ASSERT_NO_THROW(m_operator->setInputData(TestOperator::INPUT_1, m_container));
        
        boost::thread t1(boost::bind(&OperatorWrapperTest::getOutputDataWithInterrupt, this, _1), TestOperator::OUTPUT_1);
        boost::this_thread::sleep(boost::posix_time::seconds(1));
        
        t1.interrupt();
        t1.join();
        
        CPPUNIT_ASSERT_NO_THROW(m_operator->deactivate());
        CPPUNIT_ASSERT_NO_THROW(m_operator->activate());
        
        /*** Test 2 ***/
        boost::thread t2(boost::bind(&OperatorWrapperTest::clearOutputDataDelayed, this, _1), TestOperator::OUTPUT_1);
        boost::thread t3(boost::bind(&OperatorWrapperTest::clearOutputDataDelayed, this, _1), TestOperator::OUTPUT_2);
        CPPUNIT_ASSERT_NO_THROW(m_operator->setInputData(TestOperator::INPUT_1, m_container));
        CPPUNIT_ASSERT_NO_THROW(m_operator->setInputData(TestOperator::INPUT_2, m_container));
        CPPUNIT_ASSERT_NO_THROW(m_operator->setInputData(TestOperator::INPUT_1, m_container));
        CPPUNIT_ASSERT_NO_THROW(m_operator->setInputData(TestOperator::INPUT_2, m_container));
        CPPUNIT_ASSERT_NO_THROW(m_operator->setInputData(TestOperator::INPUT_1, m_container));
        
        t2.join();
        t3.join();
        
        CPPUNIT_ASSERT_NO_THROW(m_operator->deactivate());
        CPPUNIT_ASSERT_NO_THROW(m_operator->activate());
        
        /*** Test 3 ***/
        CPPUNIT_ASSERT_NO_THROW(m_operator->setInputData(TestOperator::INPUT_1, m_container));
        CPPUNIT_ASSERT_NO_THROW(m_operator->setInputData(TestOperator::INPUT_2, m_container));
        CPPUNIT_ASSERT_NO_THROW(m_operator->setInputData(TestOperator::INPUT_1, m_container));
        CPPUNIT_ASSERT_NO_THROW(m_operator->setInputData(TestOperator::INPUT_2, m_container));
        boost::thread t4(boost::bind(&OperatorWrapperTest::setInputDataWithInterrupt, this, _1), TestOperator::INPUT_1);
        
        boost::this_thread::sleep(boost::posix_time::seconds(1));
        
        t4.interrupt();
        t4.join();
        
        CPPUNIT_ASSERT_NO_THROW(m_operator->clearOutputData(TestOperator::OUTPUT_1));
        CPPUNIT_ASSERT_NO_THROW(m_operator->clearOutputData(TestOperator::OUTPUT_2));
    }

    
    void OperatorWrapperTest::tearDown ( void )
    {
        delete m_operator;
        delete m_container;
    }
} 