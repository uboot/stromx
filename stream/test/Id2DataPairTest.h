#ifndef STREAM_ID2DATAPAIRTEST_H
#define STREAM_ID2DATAPAIRTEST_H

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include <stream/DataContainer.h>

namespace stream
{
    class Id2DataMap;
    class DataContainer;
    
    class Id2DataPairTest : public CPPUNIT_NS :: TestFixture
    {
        CPPUNIT_TEST_SUITE (Id2DataPairTest);
        CPPUNIT_TEST (testTrySet);
        CPPUNIT_TEST (testTryGet);
        CPPUNIT_TEST (testSet);
        CPPUNIT_TEST (testGet);
        CPPUNIT_TEST_SUITE_END ();

        public:
            Id2DataPairTest() : m_map(0) {}
            void setUp();
            void tearDown();

        protected:
            void testTrySet();
            void testTryGet();
            void testSet();
            void testGet();
            
        private:
            Id2DataMap* m_map;
            DataContainer m_dataContainer; 
    };
}

#endif // STREAM_ID2DATAPAIRTEST_H