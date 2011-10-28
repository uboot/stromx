#include "ImageTest.h"


#include <stromx/base/Image.h>
#include <stromx/core/Exception.h>

#include <cppunit/TestAssert.h>

CPPUNIT_TEST_SUITE_REGISTRATION (base::ImageTest);

namespace base
{
    void ImageTest::setUp ( void )
    {
        m_image = 0;
    }
    
    void ImageTest::testOpenUnknownFile()
    {
        m_image = new Image();
        
        CPPUNIT_ASSERT_THROW(m_image->open("unknown.jpg"), core::FileAccessFailed);
    }
    
    void ImageTest::testImageFile()
    {
        CPPUNIT_ASSERT_NO_THROW(m_image = new Image("lenna.jpg"));
        CPPUNIT_ASSERT_EQUAL((unsigned int)(500), m_image->width());
        CPPUNIT_ASSERT_EQUAL((unsigned int)(512), m_image->height());
        CPPUNIT_ASSERT_EQUAL((unsigned int)(1500), m_image->stride());
        CPPUNIT_ASSERT_EQUAL(core::Image::BGR_24, m_image->pixelType());
        CPPUNIT_ASSERT(m_image->data());
    }
    
    void ImageTest::testOpenJpeg()
    {
        m_image = new Image();
        
        CPPUNIT_ASSERT_NO_THROW(m_image->open("lenna.jpg"));
        CPPUNIT_ASSERT_EQUAL((unsigned int)(500), m_image->width());
        CPPUNIT_ASSERT_EQUAL((unsigned int)(512), m_image->height());
        CPPUNIT_ASSERT_EQUAL((unsigned int)(1500), m_image->stride());
        CPPUNIT_ASSERT_EQUAL(core::Image::BGR_24, m_image->pixelType());
        CPPUNIT_ASSERT(m_image->data());
    }
    
    void ImageTest::testImageDefault()
    {
        CPPUNIT_ASSERT_NO_THROW(m_image = new Image());
        CPPUNIT_ASSERT_EQUAL((unsigned int)(0), m_image->width());
        CPPUNIT_ASSERT_EQUAL((unsigned int)(0), m_image->height());
        CPPUNIT_ASSERT_EQUAL((unsigned int)(0), m_image->stride());
        CPPUNIT_ASSERT_EQUAL(core::Image::NONE, m_image->pixelType());
        CPPUNIT_ASSERT_EQUAL((uint8_t*)(0), m_image->data());
    }
    
    void ImageTest::testImageRgb24()
    {
        CPPUNIT_ASSERT_NO_THROW(m_image = new Image(200, 100, core::Image::RGB_24));
        CPPUNIT_ASSERT_EQUAL((unsigned int)(200), m_image->width());
        CPPUNIT_ASSERT_EQUAL((unsigned int)(100), m_image->height());
        CPPUNIT_ASSERT_EQUAL((unsigned int)(600), m_image->stride());
        CPPUNIT_ASSERT_EQUAL(core::Image::RGB_24, m_image->pixelType());
        CPPUNIT_ASSERT(m_image->data());
        delete m_image;
        
        CPPUNIT_ASSERT_NO_THROW(m_image = new Image(0, 0, core::Image::RGB_24));
    }
    
    void ImageTest::testImageMono8()
    {
        CPPUNIT_ASSERT_NO_THROW(m_image = new Image(200, 100, core::Image::MONO_8));
        CPPUNIT_ASSERT_EQUAL((unsigned int)(200), m_image->width());
        CPPUNIT_ASSERT_EQUAL((unsigned int)(100), m_image->height());
        CPPUNIT_ASSERT_EQUAL((unsigned int)(200), m_image->stride());
        CPPUNIT_ASSERT_EQUAL(core::Image::MONO_8, m_image->pixelType());
        CPPUNIT_ASSERT(m_image->data());
        delete m_image;
        
        CPPUNIT_ASSERT_NO_THROW(m_image = new Image(0, 0, core::Image::MONO_8));
    }

    void ImageTest::testSaveJpeg()
    {
        m_image = new Image(200, 100, core::Image::RGB_24);
        CPPUNIT_ASSERT_NO_THROW(m_image->save("ImageTest_testSaveJpeg.jpg"));
        delete m_image;
        m_image = 0;
        m_image = new Image("ImageTest_testSaveJpeg.jpg");
        CPPUNIT_ASSERT_EQUAL((unsigned int)(200), m_image->width());
        CPPUNIT_ASSERT_EQUAL((unsigned int)(100), m_image->height());
    }
    
    void ImageTest::testSaveUnknownDirectory()
    {
        m_image = new Image(200, 100, core::Image::RGB_24);
        CPPUNIT_ASSERT_THROW(m_image->save("unknown_dir/ImageTest_testSaveJpeg.jpg"), core::FileAccessFailed);
    }
    
    void ImageTest::testSerialize()
    {
        m_image = new Image("lenna.jpg");

        CPPUNIT_ASSERT_THROW(m_image->serialize("ImageTest_testSerialize", "unknown_dir/"), core::SerializationError);
        
        CPPUNIT_ASSERT_EQUAL(std::string("ImageTest_testSerialize.png"), m_image->serialize("ImageTest_testSerialize", "./"));
        CPPUNIT_ASSERT_EQUAL((unsigned int)(500), m_image->width());
        CPPUNIT_ASSERT_EQUAL((unsigned int)(512), m_image->height());
    }
        
    void ImageTest::testDeserialize()
    {
        m_image = new Image();
        
        CPPUNIT_ASSERT_THROW(m_image->deserialize("unknown.png", "./"), core::DeserializationError);
        
        CPPUNIT_ASSERT_NO_THROW(m_image->deserialize("ImageTest_testSerialize.png", "./"));
    }

    void ImageTest::tearDown ( void )
    {
        delete m_image;
    }
}