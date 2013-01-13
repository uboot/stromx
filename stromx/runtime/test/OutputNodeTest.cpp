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

#include <cppunit/TestAssert.h>
#include "OutputNodeTest.h"
#include "TestOperator.h"
#include "../DataContainer.h"
#include "../Exception.h"
#include "../None.h"
#include "../Operator.h"
#include "../impl/InputNode.h"
#include "../impl/OutputNode.h"

CPPUNIT_TEST_SUITE_REGISTRATION (stromx::runtime::OutputNodeTest);

namespace stromx
{
    namespace runtime
    {
        using namespace impl;
        
        void OutputNodeTest::setUp()
        {
            m_operatorWrapper = new Operator(new TestOperator());
            m_operatorWrapper->initialize();
            m_operatorWrapper->activate();
            m_container = DataContainer(new runtime::None);
            m_operatorWrapper->setInputData(TestOperator::INPUT_1, m_container);
            m_operatorWrapper->setInputData(TestOperator::INPUT_2, m_container);
            
            m_outputNode = new OutputNode(m_operatorWrapper, TestOperator::OUTPUT_1);
        }

        void OutputNodeTest::tearDown()
        {
            delete m_operatorWrapper;
            delete m_outputNode;
        }
        
        void OutputNodeTest::testGetOutputData()
        {
            DataContainer data;
            CPPUNIT_ASSERT_NO_THROW(data = m_outputNode->getOutputData());
            CPPUNIT_ASSERT_EQUAL(m_container, data);
        }
        
        void OutputNodeTest::testReset()
        {
            InputNode inputNode1(m_operatorWrapper, 0);
            InputNode inputNode2(m_operatorWrapper, 1);
            
            m_outputNode->addConnectedInput(&inputNode1);
            m_outputNode->addConnectedInput(&inputNode2);
            
            DataContainer data;
            data = m_outputNode->getOutputData();
            
            m_outputNode->reset();
            data = m_outputNode->getOutputData();
            data = m_outputNode->getOutputData();
        }
        
        void OutputNodeTest::testTwoInputs()
        {
            InputNode inputNode1(m_operatorWrapper, 0);
            InputNode inputNode2(m_operatorWrapper, 1);
            
            m_outputNode->addConnectedInput(&inputNode1);
            m_outputNode->addConnectedInput(&inputNode2);
            
            DataContainer data;
            data = m_outputNode->getOutputData();
            data = m_outputNode->getOutputData();
            
            CPPUNIT_ASSERT_EQUAL(m_container, data);
        }
        
        void OutputNodeTest::testRemoveConnectedInputs()
        {
            InputNode inputNode(m_operatorWrapper, 0);
            
            m_outputNode->addConnectedInput(&inputNode);
            CPPUNIT_ASSERT_NO_THROW(m_outputNode->removeConnectedInput(&inputNode));
            CPPUNIT_ASSERT_NO_THROW(m_outputNode->removeConnectedInput(&inputNode));
        }
        
        void OutputNodeTest::testAddConnectedInputs()
        {
            InputNode inputNode(m_operatorWrapper, 0);
            
            CPPUNIT_ASSERT_NO_THROW(m_outputNode->addConnectedInput(&inputNode));
            CPPUNIT_ASSERT_EQUAL(size_t(1), m_outputNode->connectedInputs().size());
            
            CPPUNIT_ASSERT_THROW(m_outputNode->addConnectedInput(0), WrongArgument);
        }
    }
}

