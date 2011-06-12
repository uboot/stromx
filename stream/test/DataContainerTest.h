#ifndef STREAM_DATACONTAINERTEST_H
#define STREAM_DATACONTAINERTEST_H

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

namespace stream
{
    class DataContainer;
    class Data;

    class DataContainerTest : public CPPUNIT_NS :: TestFixture
    {
        CPPUNIT_TEST_SUITE (DataContainerTest);
        CPPUNIT_TEST (testReference);
        CPPUNIT_TEST (testDereference);
        CPPUNIT_TEST (testGetReadAccess);
        CPPUNIT_TEST (testGetWriteAccess);
        CPPUNIT_TEST (testClearWriteAccess);
        CPPUNIT_TEST_SUITE_END ();

        public:
            DataContainerTest() : m_container(0) {}
            
            void setUp();
            void tearDown();

        protected:
            void testReference();
            void testDereference();
            void testGetReadAccess();
            void testGetWriteAccess();
            void testClearWriteAccess();
            
        private:
            void dereferenceDelayed();
            void clearWriteAccessDelayed();
            void referenceWithInterruptException();
            void getWriteAccessWithInterruptException();
            
            DataContainer* m_container;
            const Data* m_data;
    };
}

#endif // STREAM_DATACONTAINERTEST_H