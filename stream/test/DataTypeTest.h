#ifndef STREAM_DATATYPETEST_H
#define STREAM_DATATYPETEST_H

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

namespace stream
{
    class DataTypeTest : public CPPUNIT_NS :: TestFixture
    {
        CPPUNIT_TEST_SUITE (DataTypeTest);
        CPPUNIT_TEST (testIntTypes);
        CPPUNIT_TEST (testImageTypes);
        CPPUNIT_TEST_SUITE_END ();

        public:
            void setUp() {}
            void tearDown() {}

        protected:
            void testIntTypes();
            void testImageTypes();
    };
}

#endif // STREAM_DATATYPETEST_H