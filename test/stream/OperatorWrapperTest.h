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
        CPPUNIT_TEST (setInputDataTest);
        CPPUNIT_TEST (getOutputDataTest);
        CPPUNIT_TEST_SUITE_END ();

    public:
        OperatorWrapperTest() {}
        
        void setUp();
        void tearDown();

    protected:
        void setInputDataTest();
        void getOutputDataTest();
            
    private:
        void setInputDataDelayed(const unsigned int id); 
        void getOutputDataWithInterrupt(const unsigned id);
        
        OperatorWrapper* m_operator;
        DataContainer* m_container;
            
    };
}

#endif // STREAM_OPERATORWRAPPERTEST_H