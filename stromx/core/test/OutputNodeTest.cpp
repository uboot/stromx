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

#include "OutputNodeTest.h"

#include "TestOperator.h"

#include <stromx/core/DataContainer.h>
#include <stromx/core/Operator.h>
#include <stromx/core/None.h>
#include <stromx/core/impl/OutputNode.h>
#include <stromx/core/impl/InputNode.h>
#include <stromx/core/Exception.h>

#include <cppunit/TestAssert.h>

CPPUNIT_TEST_SUITE_REGISTRATION (core::OutputNodeTest);

namespace core
{
    using namespace impl;
    
    void OutputNodeTest::setUp()
    {
        m_operatorWrapper = new Operator(new TestOperator());
        m_operatorWrapper->initialize();
        m_operatorWrapper->activate();
        m_container = DataContainer(new core::None);
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
        CPPUNIT_ASSERT_NO_THROW(m_outputNode->removeConnectedInput(m_inputNode));
    }
    
    void OutputNodeTest::testAddConnectedInputs()
    {
        CPPUNIT_ASSERT_NO_THROW(m_outputNode->addConnectedInput(m_inputNode));
        CPPUNIT_ASSERT_EQUAL(size_t(1), m_outputNode->connectedInputs().size());
        
        CPPUNIT_ASSERT_THROW(m_outputNode->addConnectedInput(0), WrongArgument);
    }
}

