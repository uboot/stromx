#ifndef STREAM_DATACONTAINERTEST_H
#define STREAM_DATACONTAINERTEST_H

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

namespace stream
{
    class DataContainer;
    
    class DataContainerTest : public CPPUNIT_NS :: TestFixture
    {
        CPPUNIT_TEST_SUITE (DataContainerTest);
        CPPUNIT_TEST (testDestroy);
        CPPUNIT_TEST (testDestroyDelayed);
        CPPUNIT_TEST (testComparison);
        CPPUNIT_TEST (testEmpty);
        CPPUNIT_TEST_SUITE_END ();

        public:
            DataContainerTest() {}
            
            void setUp() {}
            void tearDown() {}

        protected:
            void testDestroy();
            void testDestroyDelayed();
            void testComparison();
            void testEmpty();
        private:
            void destroyDelayed(DataContainer & container);
    };
}

#endif // STREAM_DATACONTAINERTEST_H