#include "OutputNodeTest.h"

#include "TestOperator.h"

#include <stream/DataContainer.h>
#include <stream/SynchronizedOperatorKernel.h>
#include <stream/None.h>
#include <stream/OutputNode.h>
#include <stream/InputNode.h>
#include <stream/Exception.h>

#include <cppunit/TestAssert.h>

CPPUNIT_TEST_SUITE_REGISTRATION (stream::OutputNodeTest);

namespace stream
{
    void OutputNodeTest::setUp()
    {
        m_operatorWrapper = new SynchronizedOperatorKernel(new TestOperator());
        m_operatorWrapper->initialize();
        m_operatorWrapper->activate();
        m_container = DataContainer(new stream::None);
        m_outputNode = new OutputNode(m_operatorWrapper, TestOperator::OUTPUT_1);
        m_inputNode = new InputNode(m_operatorWrapper, 0);
    }

    void OutputNodeTest::tearDown()
    {
        delete m_operatorWrapper;
        delete m_outputNode;
    }
    
    void OutputNodeTest::testGetOutputData()
    {
        m_operatorWrapper->setInputData(TestOperator::INPUT_1, m_container);
        m_operatorWrapper->setInputData(TestOperator::INPUT_2, m_container);
        
        DataContainer data;
        CPPUNIT_ASSERT_NO_THROW(data = m_outputNode->getOutputData());
        CPPUNIT_ASSERT_EQUAL(m_container, data);
    }
    
    void OutputNodeTest::testRemoveConnectedInputs()
    {
        m_outputNode->addConnectedInput(m_inputNode);
        CPPUNIT_ASSERT_NO_THROW(m_outputNode->removeConnectedInput(m_inputNode));
        CPPUNIT_ASSERT_THROW(m_outputNode->removeConnectedInput(m_inputNode), InvalidArgument);
    }
    
    void OutputNodeTest::testAddConnectedInputs()
    {
        CPPUNIT_ASSERT_NO_THROW(m_outputNode->addConnectedInput(m_inputNode));
        CPPUNIT_ASSERT_EQUAL(size_t(1), m_outputNode->connectedInputs().size());
        
        CPPUNIT_ASSERT_THROW(m_outputNode->addConnectedInput(0), InvalidArgument);
    }
}

