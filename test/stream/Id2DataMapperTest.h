#ifndef STREAM_ID2DATAMAPPERTEST_H
#define STREAM_ID2DATAMAPPERTEST_H

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

namespace stream
{
    class DataContainer;

    class Id2DataMapperTest : public CPPUNIT_NS :: TestFixture
    {
        CPPUNIT_TEST_SUITE (Id2DataMapperTest);
        CPPUNIT_TEST (setTest);
        CPPUNIT_TEST_SUITE_END ();

        public:
            void setUp();
            void tearDown();

        protected:
            void setTest();
            
    };
}

#endif // STREAM_ID2DATAMAPPERTEST_H