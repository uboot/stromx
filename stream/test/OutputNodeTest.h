#ifndef STREAM_OUTPUTNODETEST_H
#define STREAM_OUTPUTNODETEST_H

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

namespace stream
{
    class OutputNodeTest : public CPPUNIT_NS :: TestFixture
    {
        CPPUNIT_TEST_SUITE (OutputNodeTest);
        CPPUNIT_TEST_SUITE_END ();

        public:
            void setUp();
            void tearDown();

        protected:
            
        private:
    };
}

#endif // STREAM_OUTPUTNODETEST_H