#include "OperatorWrapperTest.h"

#include "TestOperator.h"

#include <stream/OperatorWrapper.h>
#include <stream/None.h>
#include <stream/DataContainer.h>

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

    void OperatorWrapperTest::setInputDataTest ( void )
    {
        DataContainer* data1;
        DataContainer* data2;
        
        CPPUNIT_ASSERT_NO_THROW(m_operator->setInputData(TestOperator::INPUT_1, m_container));
        CPPUNIT_ASSERT_NO_THROW(m_operator->setInputData(TestOperator::INPUT_2, m_container));
        CPPUNIT_ASSERT_NO_THROW(data1 = m_operator->getOutputData(TestOperator::INPUT_1));
        CPPUNIT_ASSERT_NO_THROW(data2 = m_operator->getOutputData(TestOperator::INPUT_2));
        CPPUNIT_ASSERT_EQUAL(m_container, data1);
        CPPUNIT_ASSERT_EQUAL(m_container, data2);
    }

    void OperatorWrapperTest::tearDown ( void )
    {
        delete m_operator;
        delete m_container;
    }
} 