#ifndef STREAM_OPERATORWRAPPERTEST_H
#define STREAM_OPERATORWRAPPERTEST_H

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include <stream/DataContainer.h>

namespace stream
{
    class SynchronizedOperatorKernel;
    class TestOperator;

    class SynchronizedOperatorKernelTest : public CPPUNIT_NS :: TestFixture
    {
        CPPUNIT_TEST_SUITE (SynchronizedOperatorKernelTest);
        CPPUNIT_TEST (testSetInputData);
        CPPUNIT_TEST (testGetOutputData);
        CPPUNIT_TEST (testClearOutputData);
        CPPUNIT_TEST (testInitialize);
        CPPUNIT_TEST (testActivate);
        CPPUNIT_TEST (testDeactivate);
        CPPUNIT_TEST (testGetParameter);
        CPPUNIT_TEST (testSetParameter);
        CPPUNIT_TEST (testGetParameterStatusNone);
        CPPUNIT_TEST (testSetParameterStatusNone);
        CPPUNIT_TEST_SUITE_END ();

    public:
        SynchronizedOperatorKernelTest() : m_operatorWrapper(0), m_testOperator(0) {}
        
        void setUp();
        void tearDown();

    protected:
        void testSetInputData();
        void testGetOutputData();
        void testClearOutputData();
        void testInitialize();
        void testActivate();
        void testDeactivate();
        void testGetParameter();
        void testSetParameter();
        void testGetParameterStatusNone();
        void testSetParameterStatusNone();
            
    private:
        void setInputDataDelayed(const unsigned int id); 
        void clearOutputDataDelayed(const unsigned int id); 
        void getOutputDataWithInterrupt(const unsigned id);
        void setInputDataWithInterrupt(const unsigned int id);
        
        SynchronizedOperatorKernel* m_operatorWrapper;
        TestOperator* m_testOperator;
        DataContainer m_container;
    };
}

#endif // STREAM_OPERATORWRAPPERTEST_H