#ifndef STREAM_INPUTNODETEST_H
#define STREAM_INPUTNODETEST_H

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include <stream/DataContainer.h>

namespace stream
{
    class Operator;
    class InputNode;
    class OutputNode;
    
    class InputNodeTest : public CPPUNIT_NS :: TestFixture
    {
        CPPUNIT_TEST_SUITE (InputNodeTest);
        CPPUNIT_TEST(testConnect);
        CPPUNIT_TEST(testDisconnect);
        CPPUNIT_TEST(testSetInputData);
        CPPUNIT_TEST_SUITE_END ();

    public:
        InputNodeTest() 
          : m_operatorWrapper(0),
            m_sourceOperatorWrapper(0),
            m_inputNode(0),
            m_sourceNode(0)
        {}
        
        void setUp();
        void tearDown();

    protected:
        void testConnect();
        void testDisconnect();
        void testSetInputData();
            
    private: 
        Operator* m_operatorWrapper;
        Operator* m_sourceOperatorWrapper;
        DataContainer m_container;
        InputNode* m_inputNode;
        OutputNode* m_sourceNode;
        
    };
}

#endif // STREAM_INPUTNODETEST_H