#include "NetworkTest.h"

#include "TestOperator.h"

#include <stream/Network.h>
#include <stream/Exception.h>
#include <stream/impl/SynchronizedOperatorKernel.h>
#include <stream/Operator.h>

#include <cppunit/TestAssert.h>
#include <boost/thread/thread.hpp>

CPPUNIT_TEST_SUITE_REGISTRATION (stream::NetworkTest);

namespace stream
{
    class Operator;
    
    void NetworkTest::setUp()
    {
        m_network = new Network();
    }

    void NetworkTest::testAddOperator()
    {
        //add nullpointer should throw an exception
        CPPUNIT_ASSERT_THROW(m_network->addOperator(0), WrongArgument);
        
        OperatorKernel* kernel = new TestOperator();
        Operator* op1 = new Operator(kernel);
        // add an unitialized operator should throw an exception
        CPPUNIT_ASSERT_THROW(m_network->addOperator(op1), WrongArgument);
        
        op1->initialize();
        // add initialized operator should not throw an exception;
        CPPUNIT_ASSERT_NO_THROW(m_network->addOperator(op1));
        // size of vector containing all operators belonging to the network should be +1 
        CPPUNIT_ASSERT_EQUAL((unsigned int)(1), (unsigned int)(m_network->operators().size()));
        
        CPPUNIT_ASSERT_EQUAL(op1, m_network->operators()[0]);
        
        // can not add the same operator again
        Operator* op2 = new Operator(kernel);
        CPPUNIT_ASSERT_THROW(m_network->addOperator(op2), WrongArgument);
        
        // The pointer op2 should be deleted her, however this results in an error because
        // op has already been deleted. In other words this is deliberate memory leak.
    }
    
    void NetworkTest::testRemoveOperator()
    {
        CPPUNIT_ASSERT_THROW(m_network->removeOperator(0), WrongArgument);
        
        Operator* op = new Operator(new TestOperator());
        op->initialize();
        m_network->addOperator(op);
        
        Operator* testOp = new Operator(new TestOperator());
        CPPUNIT_ASSERT_THROW(m_network->removeOperator(testOp), WrongArgument);
        delete testOp;
        
        CPPUNIT_ASSERT_NO_THROW(m_network->removeOperator(op));
        CPPUNIT_ASSERT_EQUAL((unsigned int)(0), (unsigned int)(m_network->operators().size()));
    }

    void NetworkTest::testActivate()
    {
        Operator* op1 = new Operator(new TestOperator());
        Operator* op2 = new Operator(new TestOperator());
        op1->initialize();
        op2->initialize();
        m_network->addOperator(op1);
        m_network->addOperator(op2);
        CPPUNIT_ASSERT_NO_THROW(m_network->activate());
        CPPUNIT_ASSERT_EQUAL(Operator::ACTIVE,op1->status());
        CPPUNIT_ASSERT_EQUAL(Operator::ACTIVE,op2->status());
    }

    void NetworkTest::testDeactivate()
    {
        CPPUNIT_ASSERT_NO_THROW(m_network->deactivate());
        Operator* op1 = new Operator(new TestOperator());
        Operator* op2 = new Operator(new TestOperator());
        op1->initialize();
        op2->initialize();
        m_network->addOperator(op1);
        m_network->addOperator(op2);
        m_network->activate();
        CPPUNIT_ASSERT_NO_THROW(m_network->deactivate());
    }

    void NetworkTest::tearDown()
    {
        delete m_network;
        
    }
}
