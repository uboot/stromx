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
        CPPUNIT_TEST (testDereference);
        CPPUNIT_TEST (testGetWriteAccess);
        CPPUNIT_TEST_SUITE_END ();

        public:
            DataContainerTest() : m_container(0) {}
            
            void setUp();
            void tearDown();

        protected:
            void testDereference();
            void testGetWriteAccess();
            
        private:
            DataContainer* m_container;
    };
}

#endif // STREAM_DATACONTAINERTEST_H