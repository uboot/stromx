#ifndef STREAM_THREADTEST_H
#define STREAM_THREADTEST_H

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include <stream/DataContainer.h>

namespace stream
{
    class SynchronizedOperatorKernel;
    class TestOperator;
    class OperatorNode;
    class ThreadImpl;
    class DataContainer;
    class InputNode;
    
    class ThreadImplTest : public CPPUNIT_NS :: TestFixture
    {
        CPPUNIT_TEST_SUITE (ThreadImplTest);
        CPPUNIT_TEST(testStart);
        CPPUNIT_TEST(testStop);
        CPPUNIT_TEST(testJoin);
        CPPUNIT_TEST(testAddOperator);
        CPPUNIT_TEST(testInsertOperator);
        CPPUNIT_TEST(testRemoveOperator);
        CPPUNIT_TEST_SUITE_END ();

    public:
        ThreadImplTest() : m_thread(0), m_operatorNode(0), m_node(0) {}
        
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
        DataContainer m_container;
        ThreadImpl* m_thread;
        InputNode* m_node;
        OperatorNode* m_operatorNode;
    };
}

#endif // STREAM_OUTPUTNODETEST_H