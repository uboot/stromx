#include "InputNodeTest.h"

#include "TestOperator.h"

#include <DataContainer.h>
#include <OperatorWrapper.h>
#include <None.h>
#include <InputNode.h>
#include <OutputNode.h>
#include <Exception.h>

#include <cppunit/TestAssert.h>

CPPUNIT_TEST_SUITE_REGISTRATION (stream::InputNodeTest);

namespace stream
{
    void InputNodeTest::setUp()
    {
        m_operatorWrapper = new OperatorWrapper(new TestOperator(0));
        m_operatorWrapper->activate();
        m_sourceOperatorWrapper = new OperatorWrapper(new TestOperator(0));
        m_sourceOperatorWrapper->activate();
        
        m_container = new DataContainer(new stream::None);
        m_inputNode = new InputNode(m_operatorWrapper, TestOperator::INPUT_1);
        m_sourceNode = new OutputNode(m_sourceOperatorWrapper, TestOperator::OUTPUT_2);
    }
    
    void InputNodeTest::testConnect()
    {
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
        delete m_container;
        delete m_inputNode;
        delete m_sourceNode;
    }
    
}

