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
#include "TestOperator.h"
#include "ThreadTest.h"
#include "../impl/Network.h"
#include <stromx/core/Exception.h>
#include <stromx/core/Operator.h>
#include <stromx/core/Thread.h>


CPPUNIT_TEST_SUITE_REGISTRATION (stromx::core::ThreadTest);

namespace stromx
{
    namespace core
    {
        void ThreadTest::setUp()
        {
            m_network = new impl::Network();
            
            Operator* m_op1 = new Operator(new TestOperator);
            Operator* m_op2 = new Operator(new TestOperator);
            
            m_op1->initialize();
            m_op2->initialize();
            
            m_network->addOperator(m_op1);
            m_network->addOperator(m_op2);
            
            m_thread =  new Thread(m_network);
            m_thread->addNode(m_op1, TestOperator::INPUT_1);
            m_thread->addNode(m_op2, TestOperator::INPUT_1);
        }
        
        void ThreadTest::testAddOperator()
        {
            Operator* op = new Operator(new TestOperator);
            op->initialize();
            
            CPPUNIT_ASSERT_THROW(m_thread->addNode(op, TestOperator::INPUT_1), WrongArgument);
            
            m_network->addOperator(op);
            CPPUNIT_ASSERT_NO_THROW(m_thread->addNode(op, TestOperator::INPUT_1));
            CPPUNIT_ASSERT_EQUAL(op, m_thread->inputSequence()[2].op());
            CPPUNIT_ASSERT_EQUAL((unsigned int)(TestOperator::INPUT_1), m_thread->inputSequence()[2].id());
        }

        void ThreadTest::testInsertOperator()
        {
            Operator* op = new Operator(new TestOperator);
            op->initialize();
            
            CPPUNIT_ASSERT_THROW(m_thread->insertNode(1, op, TestOperator::INPUT_1), WrongArgument);
            
            m_network->addOperator(op);
            CPPUNIT_ASSERT_NO_THROW(m_thread->insertNode(1, op, TestOperator::INPUT_1));
            CPPUNIT_ASSERT_EQUAL(op, m_thread->inputSequence()[1].op());
            CPPUNIT_ASSERT_EQUAL((unsigned int)(TestOperator::INPUT_1), m_thread->inputSequence()[1].id());
        }

        void ThreadTest::testRemoveOperator()
        {
            CPPUNIT_ASSERT_THROW(m_thread->removeNode(3), WrongArgument);
            CPPUNIT_ASSERT_NO_THROW(m_thread->removeNode(1));
            CPPUNIT_ASSERT_EQUAL(1, int(m_thread->inputSequence().size()));
        }

        void ThreadTest::tearDown()
        {
            delete m_thread;
            delete m_network;
        }
    }
}
