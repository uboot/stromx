#ifndef STREAM_THREADTEST_H
#define STREAM_THREADTEST_H

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include <stream/DataContainer.h>

namespace stream
{
    class OperatorWrapper;
    class TestOperator;
    class OperatorNode;
    class Thread;
    class DataContainer;
    
    class ThreadTest : public CPPUNIT_NS :: TestFixture
    {
        CPPUNIT_TEST_SUITE (ThreadTest);
        CPPUNIT_TEST(testStart);
        CPPUNIT_TEST(testStop);
        CPPUNIT_TEST(testJoin);
        CPPUNIT_TEST(testAddOperator);
        CPPUNIT_TEST(testInsertOperator);
        CPPUNIT_TEST(testRemoveOperator);
        CPPUNIT_TEST_SUITE_END ();

    public:
        ThreadTest() : m_thread(0), m_newOperator(0) {}
        
        void setUp();
        void tearDown();

    protected:
        void testAddOperator();
        void testInsertOperator();
        void testRemoveOperator();
        
        void testStart();
        void testStop();
        void testJoin();
            
    private: 
        std::vector<TestOperator*> m_operators;
        std::vector<OperatorNode*> m_operatorNodes;
        Thread* m_thread;
        DataContainer m_container;
        OperatorNode* m_newOperator;
    };
}

#endif // STREAM_OUTPUTNODETEST_H