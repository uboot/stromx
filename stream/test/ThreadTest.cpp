#include "ThreadTest.h"

#include <cppunit/TestAssert.h>

#include <stream/Thread.h>
#include <stream/Operator.h>

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
