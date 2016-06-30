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
#include "stromx/runtime/Exception.h"
#include "stromx/runtime/OperatorTester.h"
#include "stromx/runtime/Thread.h"
#include "stromx/runtime/impl/Network.h"
#include "stromx/runtime/test/TestOperator.h"
#include "stromx/runtime/test/ThreadTest.h"

CPPUNIT_TEST_SUITE_REGISTRATION (stromx::runtime::ThreadTest);

namespace stromx
{
    namespace runtime
    {
        void ThreadTest::setUp()
        {
            m_network = new impl::Network();
            
            m_op1 = new OperatorTester(new TestOperator);
            m_op2 = new OperatorTester(new TestOperator);
            
            m_op1->initialize();
            m_op2->initialize();
            
            m_network->addOperator(m_op1);
            m_network->addOperator(m_op2);
            
            m_thread = new Thread(m_network);
            m_thread->addInput(m_op1, TestOperator::INPUT_1);
            m_thread->addInput(m_op2, TestOperator::INPUT_1);
        }
        
        void ThreadTest::testAddInput()
        {
            OperatorTester* op = new OperatorTester(new TestOperator);
            op->initialize();
            
            CPPUNIT_ASSERT_THROW(m_thread->addInput(op, TestOperator::INPUT_1), WrongArgument);
            
            m_network->addOperator(op);
            CPPUNIT_ASSERT_NO_THROW(m_thread->addInput(op, TestOperator::INPUT_1));
            CPPUNIT_ASSERT_EQUAL(static_cast<const Operator*>(op), m_thread->inputSequence()[2].op());
            CPPUNIT_ASSERT_EQUAL((unsigned int)(TestOperator::INPUT_1), m_thread->inputSequence()[2].id());
        }

        void ThreadTest::testInsertInput()
        {
            OperatorTester* op = new OperatorTester(new TestOperator);
            op->initialize();
            
            CPPUNIT_ASSERT_THROW(m_thread->insertInput(1, op, TestOperator::INPUT_1), WrongArgument);
            
            m_network->addOperator(op);
            CPPUNIT_ASSERT_NO_THROW(m_thread->insertInput(1, op, TestOperator::INPUT_1));
            CPPUNIT_ASSERT_EQUAL(static_cast<const Operator*>(op), m_thread->inputSequence()[1].op());
            CPPUNIT_ASSERT_EQUAL((unsigned int)(TestOperator::INPUT_1), m_thread->inputSequence()[1].id());
        }

        void ThreadTest::testRemoveInputPosition()
        {
            CPPUNIT_ASSERT_THROW(m_thread->removeInput(3), WrongArgument);
            CPPUNIT_ASSERT_NO_THROW(m_thread->removeInput(1));
            CPPUNIT_ASSERT_EQUAL(1, int(m_thread->inputSequence().size()));
            CPPUNIT_ASSERT_EQUAL(static_cast<const Operator*>(m_op1), m_thread->inputSequence()[0].op());
        }
        
        void ThreadTest::testRemoveInputOpId()
        {
            CPPUNIT_ASSERT_NO_THROW(m_thread->removeInput(m_op1, 1));
            CPPUNIT_ASSERT_THROW(m_thread->removeInput(0, 0), WrongArgument);
            CPPUNIT_ASSERT_NO_THROW(m_thread->removeInput(m_op1, TestOperator::INPUT_1));
            CPPUNIT_ASSERT_EQUAL(1, int(m_thread->inputSequence().size()));
            CPPUNIT_ASSERT_EQUAL(static_cast<const Operator*>(m_op2), m_thread->inputSequence()[0].op());
        }
        
        void ThreadTest::testRemoveOperator()
        {
            CPPUNIT_ASSERT_NO_THROW(m_thread->removeOperator(m_op1));
            CPPUNIT_ASSERT_EQUAL((unsigned int)(1), (unsigned int)(m_thread->inputSequence().size()));
            
            CPPUNIT_ASSERT_NO_THROW(m_thread->removeOperator(m_op1));
            CPPUNIT_ASSERT_EQUAL((unsigned int)(1), (unsigned int)(m_thread->inputSequence().size()));
        }

        void ThreadTest::tearDown()
        {
            delete m_thread;
            delete m_network;
        }
    }
}
