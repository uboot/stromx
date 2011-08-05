#include "NetworkTest.h"

#include "TestOperator.h"

#include <stream/Network.h>
#include <stream/Exception.h>
#include <stream/SynchronizedOperatorKernel.h>
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
        OperatorKernel* kernel = new TestOperator();
        Operator* op1 = new Operator(kernel);
        
        // add an unitialized operator should throw an exception
        CPPUNIT_ASSERT_THROW(m_network->addOperator(op1), InvalidArgument);
        
        op1->initialize();
        CPPUNIT_ASSERT_NO_THROW(m_network->addOperator(op1));
        CPPUNIT_ASSERT_EQUAL((unsigned int)(1), (unsigned int)(m_network->operators().size()));
        CPPUNIT_ASSERT_EQUAL(op1, m_network->operators()[0]);
        
        // can not add the same operator again
        Operator* op2 = new Operator(kernel);
        CPPUNIT_ASSERT_THROW(m_network->addOperator(op2), InvalidArgument);
        
        // The pointer op2 should be deleted her, however this results in an error because
        // op has already been deleted. In other words this is deliberate memory leak.
    }
    
    void NetworkTest::testRemoveOperator()
    {
        CPPUNIT_ASSERT_THROW(m_network->removeOperator(0), InvalidArgument);
        
        Operator* op = new Operator(new TestOperator());
        op->initialize();
        m_network->addOperator(op);
        
        Operator* testOp = new Operator(new TestOperator());
        CPPUNIT_ASSERT_THROW(m_network->removeOperator(testOp), InvalidArgument);
        delete testOp;
        
        CPPUNIT_ASSERT_NO_THROW(m_network->removeOperator(op));
        CPPUNIT_ASSERT_EQUAL((unsigned int)(0), (unsigned int)(m_network->operators().size()));
    }

    void NetworkTest::testActivate()
    {
        CPPUNIT_ASSERT_NO_THROW(m_network->activate());
        CPPUNIT_ASSERT_EQUAL(Network::ACTIVE, m_network->status());
        
        CPPUNIT_ASSERT_THROW(m_network->activate(), InvalidState);
    }

    void NetworkTest::testDeactivate()
    {
        CPPUNIT_ASSERT_NO_THROW(m_network->deactivate());
        
        m_network->activate();
        CPPUNIT_ASSERT_NO_THROW(m_network->deactivate());
        CPPUNIT_ASSERT_EQUAL(Network::INACTIVE, m_network->status());
    }

    void NetworkTest::tearDown()
    {
        delete m_network;
    }
}
