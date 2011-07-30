#include "InputNodeTest.h"

#include "TestOperator.h"

#include <stream/DataContainer.h>
#include <stream/SynchronizedOperatorKernel.h>
#include <stream/None.h>
#include <stream/InputNode.h>
#include <stream/OutputNode.h>
#include <stream/Exception.h>

#include <cppunit/TestAssert.h>

CPPUNIT_TEST_SUITE_REGISTRATION (stream::InputNodeTest);

namespace stream
{
    void InputNodeTest::setUp()
    {
        m_operatorWrapper = new SynchronizedOperatorKernel(new TestOperator());
        m_operatorWrapper->initialize();
        m_operatorWrapper->activate();
        m_sourceOperatorWrapper = new SynchronizedOperatorKernel(new TestOperator());
        m_sourceOperatorWrapper->initialize();
        m_sourceOperatorWrapper->activate();
        
        m_container = DataContainer(new stream::None);
        m_inputNode = new InputNode(m_operatorWrapper, TestOperator::INPUT_1);
        m_sourceNode = new OutputNode(m_sourceOperatorWrapper, TestOperator::OUTPUT_2);
    }
    
    void InputNodeTest::testConnect()
    {
        CPPUNIT_ASSERT_NO_THROW(m_inputNode->connect(m_sourceNode));
        CPPUNIT_ASSERT_THROW(m_inputNode->connect(m_sourceNode), InvalidStateException);
    }
    
void InputNodeTest::testDisconnect()
{
    CPPUNIT_ASSERT_NO_THROW(m_inputNode->disconnect());
    
    m_inputNode->connect(m_sourceNode);
    CPPUNIT_ASSERT_NO_THROW(m_inputNode->disconnect());
    CPPUNIT_ASSERT_NO_THROW(m_inputNode->connect(m_sourceNode));
}

    void InputNodeTest::testSetInputData()
    {
        m_inputNode->connect(m_sourceNode);
        m_sourceOperatorWrapper->setInputData(TestOperator::INPUT_1, m_container);
        m_sourceOperatorWrapper->setInputData(TestOperator::INPUT_2, m_container);
        CPPUNIT_ASSERT_NO_THROW(m_inputNode->setInputData());
    }

    void InputNodeTest::tearDown()
    {
        delete m_operatorWrapper;
        delete m_sourceOperatorWrapper;
        delete m_inputNode;
        delete m_sourceNode;
    }
    
}

