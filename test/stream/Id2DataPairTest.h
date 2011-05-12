#ifndef STREAM_ID2DATAPAIRTEST_H
#define STREAM_ID2DATAPAIRTEST_H

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

namespace stream
{
    class Id2DataMap;
    class DataContainer;
    
    class Id2DataPairTest : public CPPUNIT_NS :: TestFixture
    {
        CPPUNIT_TEST_SUITE (Id2DataPairTest);
        CPPUNIT_TEST (trySetTest);
        CPPUNIT_TEST (tryGetTest);
        CPPUNIT_TEST (setTest);
        CPPUNIT_TEST (getTest);
        CPPUNIT_TEST_SUITE_END ();

        public:
            Id2DataPairTest() : m_map(0), m_dataContainer(0) {}
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

#endif // STREAM_ID2DATAPAIRTEST_H