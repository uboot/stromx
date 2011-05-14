#ifndef STREAM_ID2DATACOMPOSITETEST_H
#define STREAM_ID2DATACOMPOSITETEST_H

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

namespace stream
{
    class Id2DataMap;
    class DataContainer;
    
    class Id2DataCompositeTest : public CPPUNIT_NS :: TestFixture
    {
        CPPUNIT_TEST_SUITE (Id2DataCompositeTest);
        CPPUNIT_TEST (trySetTest);
        CPPUNIT_TEST (tryGetTest);
        CPPUNIT_TEST (setTest);
        CPPUNIT_TEST (getTest);
        CPPUNIT_TEST_SUITE_END ();

        public:
            Id2DataCompositeTest() : m_map(0), m_dataContainer(0) {}
            void setUp();
            void tearDown();

        protected:
            void trySetTest();
            void tryGetTest();
            void setTest();
            void getTest();
            
        private:
            Id2DataMap* m_map;
            DataContainer* m_dataContainer; 
    };
}

#endif // STREAM_ID2DATACOMPOSITETEST_H