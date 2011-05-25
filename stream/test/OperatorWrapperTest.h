#ifndef STREAM_OPERATORWRAPPERTEST_H
#define STREAM_OPERATORWRAPPERTEST_H

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

namespace stream
{
    class DataContainer;
    class OperatorWrapper;
    class TestOperator;

    class OperatorWrapperTest : public CPPUNIT_NS :: TestFixture
    {
        CPPUNIT_TEST_SUITE (OperatorWrapperTest);
        CPPUNIT_TEST (setInputDataTest);
        CPPUNIT_TEST (getOutputDataTest);
        CPPUNIT_TEST (clearOutputDataTest);
        CPPUNIT_TEST (activateTest);
        CPPUNIT_TEST (deactivateTest);
        CPPUNIT_TEST_SUITE_END ();

    public:
        OperatorWrapperTest() {}
        
        void setUp();
        void tearDown();

    protected:
        void setInputDataTest();
        void getOutputDataTest();
        void clearOutputDataTest();
        void activateTest();
        void deactivateTest();
            
    private:
        void setInputDataDelayed(const unsigned int id); 
        void clearOutputDataDelayed(const unsigned int id); 
        void getOutputDataWithInterrupt(const unsigned id);
        void setInputDataWithInterrupt(const unsigned int id);
        
        OperatorWrapper* m_operator;
        TestOperator* m_testOperator;
        DataContainer* m_container;
            
    };
}

#endif // STREAM_OPERATORWRAPPERTEST_H