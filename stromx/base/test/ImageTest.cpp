/* 
*  Copyright 2011 Matthias Fuchs
*
*  Licensed under the Apache License, Version 2.0 (the "License");
*  you may not use this file except in compliance with the License.
*  You may obtain a copy of the License at
*
*      http://www.apache.org/licenses/LICENSE-2.0
*
*  Unless required by applicable law or agreed to in writing, software
*  distributed under the License is distributed on an "AS IS" BASIS,
*  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
*  See the License for the specific language governing permissions and
*  limitations under the License.
*/

#include <cppunit/TestAssert.h>
#include "ImageTest.h"
#include "../Image.h"
#include <stromx/core/Exception.h>

CPPUNIT_TEST_SUITE_REGISTRATION (stromx::base::ImageTest);

namespace stromx
{
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
        
        void ImageTest::testOpenAsColor()
        {
            CPPUNIT_ASSERT_NO_THROW(m_image = new Image("lenna_bw.jpg", Image::COLOR));
            CPPUNIT_ASSERT_EQUAL(core::Image::BGR_24, m_image->pixelType());
        }

        void ImageTest::testOpenAsGrayscale()
        {
            CPPUNIT_ASSERT_NO_THROW(m_image = new Image("lenna.jpg", Image::GRAYSCALE));
            CPPUNIT_ASSERT_EQUAL(core::Image::MONO_8, m_image->pixelType());
        }

        void ImageTest::testOpenUnchanged()
        {
            CPPUNIT_ASSERT_NO_THROW(m_image = new Image("lenna_bw.jpg"));
            CPPUNIT_ASSERT_EQUAL(core::Image::MONO_8, m_image->pixelType());
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
        
        void ImageTest::testResizeBuffer()
        {
            m_image = new Image();
            CPPUNIT_ASSERT_NO_THROW(m_image->resize(300, 200, core::Image::BGR_24));
            CPPUNIT_ASSERT_EQUAL((unsigned int)(300), m_image->width());
            CPPUNIT_ASSERT_EQUAL((unsigned int)(200), m_image->height());
        }

        void ImageTest::testResizeDimension()
        {
            m_image = new Image();
            CPPUNIT_ASSERT_NO_THROW(m_image->resize(1024));
            CPPUNIT_ASSERT_EQUAL((unsigned int)(1024), m_image->size());
        }

        void ImageTest::tearDown ( void )
        {
            delete m_image;
        }
    }
}
