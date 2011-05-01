#ifndef STREAM_DATACONTAINER_TEST_H
#define STREAM_DATACONTAINER_TEST_H

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>


class DataContainerTest : public CPPUNIT_NS :: TestFixture
{
    CPPUNIT_TEST_SUITE (DataContainerTest);
    CPPUNIT_TEST (referenceTest);
    CPPUNIT_TEST_SUITE_END ();

    public:
        void setUp (void);
        void tearDown (void);

    protected:
        void referenceTest (void);
};

#endif // STREAM_DATACONTAINER_TEST_H