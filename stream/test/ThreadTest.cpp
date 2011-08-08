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
        
        Operator* op1 = new Operator(new TestOperator);
        Operator* op2 = new Operator(new TestOperator);
        
        op1->initialize();
        op2->initialize();
        
        m_network->addOperator(op1);
        m_network->addOperator(op2);
        
        m_thread =  new Thread(m_network);
    }
    
    void ThreadTest::testAddOperator()
    {
        Operator* op = new Operator(new TestOperator);
        op->initialize();
        
        CPPUNIT_ASSERT_THROW(m_thread->addNode(op, TestOperator::INPUT_1), InvalidArgument);
        
        m_network->addOperator(op);
        CPPUNIT_ASSERT_NO_THROW(m_thread->addNode(op, TestOperator::INPUT_1));
        CPPUNIT_ASSERT_EQUAL(op, m_thread->nodeSequence()[0].op());
        CPPUNIT_ASSERT_EQUAL((unsigned int)(TestOperator::INPUT_1), m_thread->nodeSequence()[0].id());
    }

    void ThreadTest::testInsertOperator()
    {

    }

    void ThreadTest::testRemoveOperator()
    {

}

    void ThreadTest::tearDown()
    {
        delete m_thread;
        delete m_network;
    }
}
