#ifndef STREAM_OUTPUTNODETEST_H
#define STREAM_OUTPUTNODETEST_H

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>


namespace stream
{
    class OperatorWrapper;
    class OutputNode;
    class InputNode;
    class DataContainer;
    
    class OutputNodeTest : public CPPUNIT_NS :: TestFixture
    {
        CPPUNIT_TEST_SUITE (OutputNodeTest);
        CPPUNIT_TEST(testGetOutputData);
        CPPUNIT_TEST(testAddConnectedInputs);
        CPPUNIT_TEST(testRemoveConnectedInputs);
        CPPUNIT_TEST_SUITE_END ();

    public:
        OutputNodeTest() : m_operatorWrapper(0), m_container(0), m_outputNode(0) {}
        
        void setUp();
        void tearDown();

    protected:
        void testGetOutputData();;
        void testAddConnectedInputs();
        void testRemoveConnectedInputs();
            
    private: 
        OperatorWrapper* m_operatorWrapper;
        DataContainer* m_container;
        OutputNode* m_outputNode;
        InputNode* m_inputNode;
    };
}

#endif // STREAM_OUTPUTNODETEST_H