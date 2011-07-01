#ifndef STREAM_ID2DATACOMPOSITETEST_H
#define STREAM_ID2DATACOMPOSITETEST_H

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include <stream/DataContainer.h>

namespace stream
{
    class Id2DataMap;
    
    class Id2DataCompositeTest : public CPPUNIT_NS :: TestFixture
    {
        CPPUNIT_TEST_SUITE (Id2DataCompositeTest);
        CPPUNIT_TEST (testTrySet);
        CPPUNIT_TEST (testTryGet);
        CPPUNIT_TEST (testSet);
        CPPUNIT_TEST (testGet);
        CPPUNIT_TEST_SUITE_END ();

        public:
            Id2DataCompositeTest() : m_map(0) {}
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

#endif // STREAM_ID2DATACOMPOSITETEST_H