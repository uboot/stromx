#include "ImageTest.h"

#include <Image.h>

#include <cppunit/TestAssert.h>

CPPUNIT_TEST_SUITE_REGISTRATION (base::ImageTest);

namespace base
{
    void ImageTest::setUp ( void )
    {
        m_image = 0;
    }
    
    void ImageTest::testImageFromJpeg()
    {
        CPPUNIT_ASSERT_NO_THROW(m_image = new Image("lenna.jpg"));
        CPPUNIT_ASSERT_EQUAL((unsigned int)(500), m_image->width());
        CPPUNIT_ASSERT_EQUAL((unsigned int)(512), m_image->height());
        CPPUNIT_ASSERT_EQUAL((unsigned int)(1500), m_image->stride());
        CPPUNIT_ASSERT_EQUAL(stream::Image::RGB_24, m_image->pixelType());
    }
    
    void ImageTest::testImageRgb24()
    {
        CPPUNIT_ASSERT_NO_THROW(m_image = new Image(200, 100, stream::Image::RGB_24));
        CPPUNIT_ASSERT_EQUAL((unsigned int)(200), m_image->width());
        CPPUNIT_ASSERT_EQUAL((unsigned int)(100), m_image->height());
        CPPUNIT_ASSERT_EQUAL((unsigned int)(600), m_image->stride());
        CPPUNIT_ASSERT_EQUAL(stream::Image::RGB_24, m_image->pixelType());
    }

    void ImageTest::testSaveJpeg()
    {
        m_image = new Image(200, 100, stream::Image::RGB_24);
        CPPUNIT_ASSERT_NO_THROW(m_image->save("test.jpg"));
        delete m_image;
        m_image = 0;
        CPPUNIT_ASSERT_NO_THROW(m_image = new Image("test.jpg"));
        CPPUNIT_ASSERT_EQUAL((unsigned int)(200), m_image->width());
        CPPUNIT_ASSERT_EQUAL((unsigned int)(100), m_image->height());
    }

    void ImageTest::tearDown ( void )
    {
        delete m_image;
    }
}