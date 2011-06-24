#ifndef STREAM_OPERATORWRAPPERTEST_H
#define STREAM_OPERATORWRAPPERTEST_H

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include <stream/DataContainer.h>

namespace stream
{
    class OperatorWrapper;
    class TestOperator;

    class OperatorWrapperTest : public CPPUNIT_NS :: TestFixture
    {
        CPPUNIT_TEST_SUITE (OperatorWrapperTest);
        CPPUNIT_TEST (testSetInputData);
        CPPUNIT_TEST (testGetOutputData);
        CPPUNIT_TEST (testClearOutputData);
        CPPUNIT_TEST (testActivate);
        CPPUNIT_TEST (testDeactivate);
        CPPUNIT_TEST (testGetParameter);
        CPPUNIT_TEST (testSetParameter);
        CPPUNIT_TEST_SUITE_END ();

    public:
        OperatorWrapperTest() : m_operatorWrapper(0), m_testOperator(0) {}
        
        void setUp();
        void tearDown();

    protected:
        void testSetInputData();
        void testGetOutputData();
        void testClearOutputData();
        void testActivate();
        void testDeactivate();
        void testGetParameter();
        void testSetParameter();
            
    private:
        void setInputDataDelayed(const unsigned int id); 
        void clearOutputDataDelayed(const unsigned int id); 
        void getOutputDataWithInterrupt(const unsigned id);
        void setInputDataWithInterrupt(const unsigned int id);
        
        OperatorWrapper* m_operatorWrapper;
        TestOperator* m_testOperator;
        DataContainer m_container;
    };
}

#endif // STREAM_OPERATORWRAPPERTEST_H