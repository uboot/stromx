#include "OutputNodeTest.h"

#include "TestOperator.h"

#include <DataContainer.h>
#include <OperatorWrapper.h>
#include <None.h>
#include <OutputNode.h>
#include <Exception.h>

#include <cppunit/TestAssert.h>

CPPUNIT_TEST_SUITE_REGISTRATION (stream::OutputNodeTest);

namespace stream
{
    void OutputNodeTest::setUp()
    {
        m_operatorWrapper = new OperatorWrapper(new TestOperator());
        m_operatorWrapper->activate();
        m_container = new DataContainer(new stream::None);
        m_outputNode = new OutputNode(m_operatorWrapper, TestOperator::OUTPUT_1);
        
    }

    void OutputNodeTest::tearDown()
    {
        delete m_operatorWrapper;
        delete m_container;
        delete m_outputNode;
    }
    
    void OutputNodeTest::testGetOutputData()
    {
        m_outputNode->incrementConnectedInputs();
        m_operatorWrapper->setInputData(TestOperator::INPUT_1, m_container);
        m_operatorWrapper->setInputData(TestOperator::INPUT_2, m_container);
        
        DataContainer* data = 0;
        CPPUNIT_ASSERT_NO_THROW(data = m_outputNode->getOutputData());
        CPPUNIT_ASSERT_EQUAL(m_container, data);
    }
    
    void OutputNodeTest::testDecrementConnectedInputs()
    {
        m_outputNode->incrementConnectedInputs();
        CPPUNIT_ASSERT_NO_THROW(m_outputNode->decrementConnectedInputs());
        CPPUNIT_ASSERT_THROW(m_outputNode->decrementConnectedInputs(), InvalidStateException);
    }
    
    void OutputNodeTest::testIncrementConnectedInputs()
    {
        CPPUNIT_ASSERT_NO_THROW(m_outputNode->incrementConnectedInputs());
    }
}

