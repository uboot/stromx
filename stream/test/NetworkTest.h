#ifndef STREAM_NETWORKTEST_H
#define STREAM_NETWORKTEST_H

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include <stream/None.h>

namespace stream
{
    class Network;
    class SynchronizedOperatorKernel;
    
    class NetworkTest : public CPPUNIT_NS :: TestFixture
    {
        CPPUNIT_TEST_SUITE (NetworkTest);
        CPPUNIT_TEST(testAddOperator);
        CPPUNIT_TEST(testRemoveOperator);
        CPPUNIT_TEST(testActivate);
        CPPUNIT_TEST(testDeactivate);
        CPPUNIT_TEST_SUITE_END ();

    public:
        NetworkTest() : m_network(0) {}
        
        void setUp();
        void tearDown();

    protected:
        void testAddOperator();
        void testRemoveOperator();
        void testActivate();
        void testDeactivate();
            
    private: 
        Network* m_network;
    };
}

#endif // STREAM_NETWORKTEST_H