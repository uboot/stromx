#ifndef STREAM_OUTPUTNODETEST_H
#define STREAM_OUTPUTNODETEST_H

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include <stream/DataContainer.h>

namespace stream
{
    class SynchronizedOperatorKernel;
    class OutputNode;
    class InputNode;
    
    class OutputNodeTest : public CPPUNIT_NS :: TestFixture
    {
        CPPUNIT_TEST_SUITE (OutputNodeTest);
        CPPUNIT_TEST(testGetOutputData);
        CPPUNIT_TEST(testAddConnectedInputs);
        CPPUNIT_TEST(testRemoveConnectedInputs);
        CPPUNIT_TEST_SUITE_END ();

    public:
        OutputNodeTest() : m_operatorWrapper(0), m_outputNode(0) {}
        
        void setUp();
        void tearDown();

    protected:
        void testGetOutputData();;
        void testAddConnectedInputs();
        void testRemoveConnectedInputs();
            
    private: 
        SynchronizedOperatorKernel* m_operatorWrapper;
        DataContainer m_container;
        OutputNode* m_outputNode;
        InputNode* m_inputNode;
    };
}

#endif // STREAM_OUTPUTNODETEST_H