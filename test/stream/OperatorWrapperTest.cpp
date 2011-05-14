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
        m_operator = new OperatorWrapper(new TestOperator(0));
        m_operator->activate();
        Data* data = new stream::None;
        m_container = new DataContainer(data);  
    }

    void OperatorWrapperTest::setInputDataTest_1 ( void )
    {
        DataContainer* data1;
        DataContainer* data2;
        
        TestOperator& op = reinterpret_cast<TestOperator&>(m_operator->op());
        
        CPPUNIT_ASSERT_NO_THROW(m_operator->setInputData(TestOperator::INPUT_1, m_container));
        CPPUNIT_ASSERT_EQUAL((unsigned int)(0), op.numExecutes());
        
        CPPUNIT_ASSERT_NO_THROW(m_operator->setInputData(TestOperator::INPUT_2, m_container));
        CPPUNIT_ASSERT_EQUAL((unsigned int)(0), op.numExecutes());
        
        CPPUNIT_ASSERT_NO_THROW(data1 = m_operator->getOutputData(TestOperator::INPUT_1));
        CPPUNIT_ASSERT_EQUAL((unsigned int)(1), op.numExecutes());
        
        CPPUNIT_ASSERT_NO_THROW(data2 = m_operator->getOutputData(TestOperator::INPUT_2));
        CPPUNIT_ASSERT_EQUAL((unsigned int)(1), op.numExecutes());
        
        CPPUNIT_ASSERT_EQUAL(m_container, data1);
        CPPUNIT_ASSERT_EQUAL(m_container, data2);
    }
    
    void OperatorWrapperTest::setInputDataDelayed ( const unsigned int id )
    {
        usleep(1000);
        m_operator->setInputData(id, m_container);
    }

    void OperatorWrapperTest::setInputDataTest_2 ( void )
    {
        DataContainer* data1;
        DataContainer* data2;
        
        boost::thread t(boost::bind(&OperatorWrapperTest::setInputDataDelayed, this, _1), TestOperator::INPUT_1);
        CPPUNIT_ASSERT_NO_THROW(m_operator->setInputData(TestOperator::INPUT_2, m_container));
        
        CPPUNIT_ASSERT_NO_THROW(data1 = m_operator->getOutputData(TestOperator::INPUT_1));
        CPPUNIT_ASSERT_NO_THROW(data2 = m_operator->getOutputData(TestOperator::INPUT_2));
        
        CPPUNIT_ASSERT_EQUAL(m_container, data1);
        CPPUNIT_ASSERT_EQUAL(m_container, data2);
    }
    
    void OperatorWrapperTest::deactivateDelayed ()
    {
        usleep(1000);
        m_operator->startDeactivating();
        m_operator->joinDeactivating();
    }
        
    void OperatorWrapperTest::deactivateStartTest()
    {
        DataContainer* data1;
        
        CPPUNIT_ASSERT_NO_THROW(m_operator->setInputData(TestOperator::INPUT_1, m_container));
        
        boost::thread t(boost::bind(&OperatorWrapperTest::deactivateDelayed, this));
        //CPPUNIT_ASSERT_THROW(data1 = m_operator->getOutputData(TestOperator::INPUT_1), IsDeactivatingException);
        data1 = m_operator->getOutputData(TestOperator::INPUT_1);
        
        t.join();
        CPPUNIT_ASSERT_EQUAL((DataContainer*)(0), data1);
    }

    
    void OperatorWrapperTest::tearDown ( void )
    {
        delete m_operator;
        delete m_container;
    }
} 