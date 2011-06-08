#include "NetworkTest.h"

#include "TestOperator.h"

#include <Network.h>
#include <Exception.h>
#include <OperatorWrapper.h>
#include <OperatorNode.h>

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
        Operator* op = new TestOperator();
        OperatorNode* node = 0;
        
        CPPUNIT_ASSERT_NO_THROW(node = m_network->addOperator(op));
        CPPUNIT_ASSERT_EQUAL((unsigned int)(1), (unsigned int)(m_network->operators().size()));
        CPPUNIT_ASSERT_EQUAL(node, m_network->operators()[0]);
        
        // can not add the same operator again
        CPPUNIT_ASSERT_THROW(node = m_network->addOperator(op), ArgumentException);
    }
    
    void NetworkTest::testRemoveOperator()
    {
        CPPUNIT_ASSERT_THROW(m_network->removeOperator(0), ArgumentException);
        
        Operator* op = new TestOperator();
        OperatorNode* node = m_network->addOperator(op);
        
        OperatorNode* testNode = new OperatorNode(new OperatorWrapper(new TestOperator));
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
