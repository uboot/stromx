#include "ThreadTest.h"

#include <cppunit/TestAssert.h>

#include <stream/Thread.h>
#include <stream/Operator.h>
#include <stream/Exception.h>

#include "TestOperator.h"

CPPUNIT_TEST_SUITE_REGISTRATION (stream::ThreadTest);

namespace stream
{
    void ThreadTest::setUp()
    {
        m_network = new Network();
        
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
        CPPUNIT_ASSERT_EQUAL(op, m_thread->nodeSequence()[2].op());
        CPPUNIT_ASSERT_EQUAL((unsigned int)(TestOperator::INPUT_1), m_thread->nodeSequence()[2].id());
    }

    void ThreadTest::testInsertOperator()
    {
        Operator* op = new Operator(new TestOperator);
        op->initialize();
        
        CPPUNIT_ASSERT_THROW(m_thread->insertNode(1, op, TestOperator::INPUT_1), WrongArgument);
        
        m_network->addOperator(op);
        CPPUNIT_ASSERT_NO_THROW(m_thread->insertNode(1, op, TestOperator::INPUT_1));
        CPPUNIT_ASSERT_EQUAL(op, m_thread->nodeSequence()[1].op());
        CPPUNIT_ASSERT_EQUAL((unsigned int)(TestOperator::INPUT_1), m_thread->nodeSequence()[1].id());
    }

    void ThreadTest::testRemoveOperator()
    {
        CPPUNIT_ASSERT_THROW(m_thread->removeNode(3), WrongArgument);
        CPPUNIT_ASSERT_NO_THROW(m_thread->removeNode(1));
        CPPUNIT_ASSERT_EQUAL(1, int(m_thread->nodeSequence().size()));
    }

    void ThreadTest::tearDown()
    {
        delete m_thread;
        delete m_network;
    }
}
