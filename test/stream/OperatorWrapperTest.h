#ifndef STREAM_OPERATORWRAPPERTEST_H
#define STREAM_OPERATORWRAPPERTEST_H

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

namespace stream
{
    class DataContainer;
    class OperatorWrapper;

    class OperatorWrapperTest : public CPPUNIT_NS :: TestFixture
    {
        CPPUNIT_TEST_SUITE (OperatorWrapperTest);
        CPPUNIT_TEST (setInputDataTest_1);
        CPPUNIT_TEST (setInputDataTest_2);
        CPPUNIT_TEST (deactivateStartTest);
        CPPUNIT_TEST_SUITE_END ();

    public:
        OperatorWrapperTest() {}
        
        void setUp();
        void tearDown();

    protected:
        void setInputDataTest_1();
        void setInputDataTest_2();
        void deactivateStartTest();
            
    private:
        void setInputDataDelayed(const unsigned int id); 
        void deactivateDelayed(); 
        
        OperatorWrapper* m_operator;
        DataContainer* m_container;
            
    };
}

#endif // STREAM_OPERATORWRAPPERTEST_H