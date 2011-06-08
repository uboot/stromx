#include "ImageTest.h"

#include <Image.h>

#include <cppunit/TestAssert.h>

CPPUNIT_TEST_SUITE_REGISTRATION (base::ImageTest);

namespace base
{
    void ImageTest::setUp ( void )
    {
        m_image = new Image("lenna.jpg");
    }

    void ImageTest::testSave()
    {
    }

    void ImageTest::tearDown ( void )
    {
    }
}