#include "NetworkTest.h"

#include "TestOperator.h"

#include <stream/Network.h>
#include <stream/Exception.h>
#include <stream/SynchronizedOperatorKernel.h>
#include <stream/OperatorNode.h>

#include <cppunit/TestAssert.h>
#include <boost/thread/thread.hpp>

CPPUNIT_TEST_SUITE_REGISTRATION (stream::NetworkTest);

namespace stream
{
    class OperatorNode;
    
    void NetworkTest::setUp()
    {
        m_network = new Network();
    }

    void NetworkTest::testAddOperator()
    {
        OperatorKernel* op = new TestOperator();
        SynchronizedOperatorKernel* wrapper1 = new SynchronizedOperatorKernel(op);
        wrapper1->initialize();
        OperatorNode* node = 0;
        
        CPPUNIT_ASSERT_NO_THROW(node = m_network->addOperator(wrapper1));
        CPPUNIT_ASSERT_EQUAL((unsigned int)(1), (unsigned int)(m_network->operators().size()));
        CPPUNIT_ASSERT_EQUAL(node, m_network->operators()[0]);
        
        // can not add the same operator again
        SynchronizedOperatorKernel* wrapper2 = new SynchronizedOperatorKernel(op);
        CPPUNIT_ASSERT_THROW(node = m_network->addOperator(wrapper2), ArgumentException);
        
        // The pointer wrapper2 should be deleted her, however this results in an error because
        // op has already been deleted. In other words this is deliberate memory leak.
    }
    
    void NetworkTest::testRemoveOperator()
    {
        CPPUNIT_ASSERT_THROW(m_network->removeOperator(0), ArgumentException);
        
        SynchronizedOperatorKernel* op = new SynchronizedOperatorKernel(new TestOperator());
        op->initialize();
        OperatorNode* node = m_network->addOperator(op);
        
        SynchronizedOperatorKernel* wrapper = new SynchronizedOperatorKernel(new TestOperator);
        wrapper->initialize();
        OperatorNode* testNode = new OperatorNode(wrapper);
        CPPUNIT_ASSERT_THROW(m_network->removeOperator(testNode), ArgumentException);
        delete testNode;
        
        CPPUNIT_ASSERT_NO_THROW(m_network->removeOperator(node));
        CPPUNIT_ASSERT_EQUAL((unsigned int)(0), (unsigned int)(m_network->operators().size()));
    }

    void NetworkTest::testActivate()
    {
        CPPUNIT_ASSERT_NO_THROW(m_network->activate());
        CPPUNIT_ASSERT_EQUAL(Network::ACTIVE, m_network->status());
        
        CPPUNIT_ASSERT_THROW(m_network->activate(), InvalidStateException);
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
