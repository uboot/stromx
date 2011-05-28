#ifndef STREAM_NETWORKTEST_H
#define STREAM_NETWORKTEST_H

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include "None.h"

namespace stream
{
    class Network;
    class OperatorWrapper;
    class TestOperator;
    
    class NetworkTest : public CPPUNIT_NS :: TestFixture
    {
        CPPUNIT_TEST_SUITE (ThreadTest);
        CPPUNIT_TEST(testAddOperator);
        CPPUNIT_TEST(testRemoveOperator);
        CPPUNIT_TEST(testActivate);
        CPPUNIT_TEST(testDeactivate);
        CPPUNIT_TEST_SUITE_END ();

    public:
        ThreadTest() : m_network(0) {}
        
        void setUp();
        void tearDown();

    protected:
        void testAddOperator();
        void testRemoveOperator();
        void testActivate();
        void testDeactivate();
            
    private: 
        Network* m_network;
        std::vector<TestOperator*> m_operators;
    };
}

#endif // STREAM_OUTPUTNODETEST_H