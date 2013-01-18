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
#include "../Config.h"
#include "../Image.h"
#include <stromx/runtime/Exception.h>
#include <stromx/runtime/DirectoryFileInput.h>
#include <stromx/runtime/DirectoryFileOutput.h>
#include <opencv2/core/core.hpp>

CPPUNIT_TEST_SUITE_REGISTRATION (stromx::example::ImageTest);

namespace stromx
{
    namespace example
    {
        const stromx::runtime::Version ImageTest::VERSION(STROMX_EXAMPLE_VERSION_MAJOR, STROMX_EXAMPLE_VERSION_MINOR, STROMX_EXAMPLE_VERSION_PATCH);
        
        void ImageTest::setUp ( void )
        {
            m_image = 0;
        }
        
        void ImageTest::testOpenUnknownFile()
        {
            m_image = new Image();
            
            CPPUNIT_ASSERT_THROW(m_image->open("unknown.jpg"), runtime::FileAccessFailed);
        }
        
        void ImageTest::testImageFile()
        {
            CPPUNIT_ASSERT_NO_THROW(m_image = new Image("lenna.jpg"));
            CPPUNIT_ASSERT_EQUAL((unsigned int)(500), m_image->width());
            CPPUNIT_ASSERT_EQUAL((unsigned int)(512), m_image->height());
            CPPUNIT_ASSERT_EQUAL((unsigned int)(1500), m_image->stride());
            CPPUNIT_ASSERT_EQUAL(runtime::Image::BGR_24, m_image->pixelType());
            CPPUNIT_ASSERT(m_image->data());
        }
        
        void ImageTest::testOpenJpeg()
        {
            m_image = new Image();
            
            CPPUNIT_ASSERT_NO_THROW(m_image->open("lenna.jpg"));
            CPPUNIT_ASSERT_EQUAL((unsigned int)(500), m_image->width());
            CPPUNIT_ASSERT_EQUAL((unsigned int)(512), m_image->height());
            CPPUNIT_ASSERT_EQUAL((unsigned int)(1500), m_image->stride());
            CPPUNIT_ASSERT_EQUAL(runtime::Image::BGR_24, m_image->pixelType());
            CPPUNIT_ASSERT(m_image->data());
        }
        
        void ImageTest::testOpenAsColor()
        {
            CPPUNIT_ASSERT_NO_THROW(m_image = new Image("lenna_bw.jpg", Image::COLOR));
            CPPUNIT_ASSERT_EQUAL(runtime::Image::BGR_24, m_image->pixelType());
        }

        void ImageTest::testOpenAsGrayscale()
        {
            CPPUNIT_ASSERT_NO_THROW(m_image = new Image("lenna.jpg", Image::GRAYSCALE));
            CPPUNIT_ASSERT_EQUAL(runtime::Image::MONO_8, m_image->pixelType());
        }

        void ImageTest::testOpenUnchanged()
        {
            CPPUNIT_ASSERT_NO_THROW(m_image = new Image("lenna_bw.jpg"));
            CPPUNIT_ASSERT_EQUAL(runtime::Image::MONO_8, m_image->pixelType());
        }
        
        void ImageTest::testImageDefault()
        {
            CPPUNIT_ASSERT_NO_THROW(m_image = new Image());
            CPPUNIT_ASSERT_EQUAL((unsigned int)(0), m_image->cols());
            CPPUNIT_ASSERT_EQUAL((unsigned int)(0), m_image->rows());
            CPPUNIT_ASSERT_EQUAL(runtime::Matrix::NONE, m_image->valueType());
            CPPUNIT_ASSERT_EQUAL((unsigned int)(0), m_image->width());
            CPPUNIT_ASSERT_EQUAL((unsigned int)(0), m_image->height());
            CPPUNIT_ASSERT_EQUAL((unsigned int)(0), m_image->stride());
            CPPUNIT_ASSERT_EQUAL(runtime::Image::NONE, m_image->pixelType());
            CPPUNIT_ASSERT_EQUAL((uint8_t*)(0), m_image->data());
            CPPUNIT_ASSERT_EQUAL(runtime::DataVariant::IMAGE, m_image->variant());
        }
        
        void ImageTest::testImageEmpty()
        {
            CPPUNIT_ASSERT_NO_THROW(m_image = new Image(0, 100, Image::BGR_24));
            CPPUNIT_ASSERT_EQUAL((unsigned int)(0), m_image->cols());
            CPPUNIT_ASSERT_EQUAL((unsigned int)(100), m_image->rows());
            CPPUNIT_ASSERT_EQUAL(runtime::Matrix::UINT_8, m_image->valueType());
            CPPUNIT_ASSERT_EQUAL((unsigned int)(0), m_image->width());
            CPPUNIT_ASSERT_EQUAL((unsigned int)(100), m_image->height());
            CPPUNIT_ASSERT_EQUAL((unsigned int)(0), m_image->stride());
            CPPUNIT_ASSERT_EQUAL(runtime::Image::BGR_24, m_image->pixelType());
            CPPUNIT_ASSERT_EQUAL((uint8_t*)(0), m_image->data());
            CPPUNIT_ASSERT_EQUAL(runtime::DataVariant::BGR_24_IMAGE, m_image->variant());
        }

        void ImageTest::testImageCopyConstructor()
        {
            Image image(200, 100, Image::RGB_24);
            
            CPPUNIT_ASSERT_NO_THROW(m_image = new Image(image));
            CPPUNIT_ASSERT_EQUAL((unsigned int)(600), m_image->cols());
            CPPUNIT_ASSERT_EQUAL((unsigned int)(100), m_image->rows());
            CPPUNIT_ASSERT_EQUAL(runtime::Matrix::UINT_8, m_image->valueType());
            CPPUNIT_ASSERT_EQUAL((unsigned int)(200), m_image->width());
            CPPUNIT_ASSERT_EQUAL((unsigned int)(100), m_image->height());
            CPPUNIT_ASSERT_EQUAL((unsigned int)(600), m_image->stride());
            CPPUNIT_ASSERT_EQUAL(runtime::Image::RGB_24, m_image->pixelType());
            CPPUNIT_ASSERT(m_image->data());
            CPPUNIT_ASSERT_EQUAL(runtime::DataVariant::RGB_24_IMAGE, m_image->variant());
        }

        void ImageTest::testImageEmptyCopyConstructor()
        {
            Image image(0, 100, Image::RGB_24);
            
            CPPUNIT_ASSERT_NO_THROW(m_image = new Image(image));
            CPPUNIT_ASSERT_EQUAL((unsigned int)(0), m_image->cols());
            CPPUNIT_ASSERT_EQUAL((unsigned int)(100), m_image->rows());
            CPPUNIT_ASSERT_EQUAL(runtime::Matrix::UINT_8, m_image->valueType());
            CPPUNIT_ASSERT_EQUAL((unsigned int)(0), m_image->width());
            CPPUNIT_ASSERT_EQUAL((unsigned int)(100), m_image->height());
            CPPUNIT_ASSERT_EQUAL((unsigned int)(0), m_image->stride());
            CPPUNIT_ASSERT_EQUAL(runtime::Image::RGB_24, m_image->pixelType());
            CPPUNIT_ASSERT_EQUAL((uint8_t*)(0), m_image->data());
            CPPUNIT_ASSERT_EQUAL(runtime::DataVariant::RGB_24_IMAGE, m_image->variant());
        }

        void ImageTest::testImageDefaultCopyConstructor()
        {
            Image image;
            
            CPPUNIT_ASSERT_NO_THROW(m_image = new Image(image));
            CPPUNIT_ASSERT_EQUAL((unsigned int)(0), m_image->cols());
            CPPUNIT_ASSERT_EQUAL((unsigned int)(0), m_image->rows());
            CPPUNIT_ASSERT_EQUAL(runtime::Matrix::NONE, m_image->valueType());
            CPPUNIT_ASSERT_EQUAL((unsigned int)(0), m_image->width());
            CPPUNIT_ASSERT_EQUAL((unsigned int)(0), m_image->height());
            CPPUNIT_ASSERT_EQUAL((unsigned int)(0), m_image->stride());
            CPPUNIT_ASSERT_EQUAL(runtime::Image::NONE, m_image->pixelType());
            CPPUNIT_ASSERT_EQUAL((uint8_t*)(0), m_image->data());
            CPPUNIT_ASSERT_EQUAL(runtime::DataVariant::IMAGE, m_image->variant());
        }
        
        void ImageTest::testImageCvImageConstructor()
        {
            cv::Mat cvImage(100, 200, CV_8UC1);
            
            CPPUNIT_ASSERT_NO_THROW(m_image = new Image(cvImage));
            CPPUNIT_ASSERT_EQUAL((unsigned int)(200), m_image->cols());
            CPPUNIT_ASSERT_EQUAL((unsigned int)(100), m_image->rows());
            CPPUNIT_ASSERT_EQUAL(runtime::Matrix::UINT_8, m_image->valueType());
            CPPUNIT_ASSERT_EQUAL((unsigned int)(200), m_image->width());
            CPPUNIT_ASSERT_EQUAL((unsigned int)(100), m_image->height());
            CPPUNIT_ASSERT_EQUAL((unsigned int)(200), m_image->stride());
            CPPUNIT_ASSERT_EQUAL(runtime::Image::MONO_8, m_image->pixelType());
            CPPUNIT_ASSERT(m_image->data());
            CPPUNIT_ASSERT_EQUAL(runtime::DataVariant::MONO_8_IMAGE, m_image->variant());
        }
        
        void ImageTest::testImageRgb24()
        {
            CPPUNIT_ASSERT_NO_THROW(m_image = new Image(200, 100, runtime::Image::RGB_24));
            CPPUNIT_ASSERT_EQUAL((unsigned int)(600), m_image->cols());
            CPPUNIT_ASSERT_EQUAL((unsigned int)(100), m_image->rows());
            CPPUNIT_ASSERT_EQUAL(runtime::Matrix::UINT_8, m_image->valueType());
            CPPUNIT_ASSERT_EQUAL((unsigned int)(200), m_image->width());
            CPPUNIT_ASSERT_EQUAL((unsigned int)(100), m_image->height());
            CPPUNIT_ASSERT_EQUAL((unsigned int)(600), m_image->stride());
            CPPUNIT_ASSERT_EQUAL(runtime::Image::RGB_24, m_image->pixelType());
            CPPUNIT_ASSERT(m_image->data());
            CPPUNIT_ASSERT_EQUAL(runtime::DataVariant::RGB_24_IMAGE, m_image->variant());
            delete m_image;
            
            CPPUNIT_ASSERT_NO_THROW(m_image = new Image(0, 0, runtime::Image::RGB_24));
        }
        
        void ImageTest::testImageMono8()
        {
            CPPUNIT_ASSERT_NO_THROW(m_image = new Image(200, 100, runtime::Image::MONO_8));
            CPPUNIT_ASSERT_EQUAL((unsigned int)(200), m_image->cols());
            CPPUNIT_ASSERT_EQUAL((unsigned int)(100), m_image->rows());
            CPPUNIT_ASSERT_EQUAL(runtime::Matrix::UINT_8, m_image->valueType());
            CPPUNIT_ASSERT_EQUAL((unsigned int)(200), m_image->width());
            CPPUNIT_ASSERT_EQUAL((unsigned int)(100), m_image->height());
            CPPUNIT_ASSERT_EQUAL((unsigned int)(200), m_image->stride());
            CPPUNIT_ASSERT_EQUAL(runtime::Image::MONO_8, m_image->pixelType());
            CPPUNIT_ASSERT(m_image->data());
            CPPUNIT_ASSERT_EQUAL(runtime::DataVariant::MONO_8_IMAGE, m_image->variant());
            delete m_image;
            
            CPPUNIT_ASSERT_NO_THROW(m_image = new Image(0, 0, runtime::Image::MONO_8));
        }

        void ImageTest::testSaveJpeg()
        {
            m_image = new Image(200, 100, runtime::Image::RGB_24);
            CPPUNIT_ASSERT_NO_THROW(m_image->save("ImageTest_testSaveJpeg.jpg"));
            delete m_image;
            m_image = 0;
            m_image = new Image("ImageTest_testSaveJpeg.jpg");
            CPPUNIT_ASSERT_EQUAL((unsigned int)(200), m_image->width());
            CPPUNIT_ASSERT_EQUAL((unsigned int)(100), m_image->height());
        }
        
        void ImageTest::testSaveUnknownDirectory()
        {
            m_image = new Image(200, 100, runtime::Image::RGB_24);
            CPPUNIT_ASSERT_THROW(m_image->save("unknown_dir/ImageTest_testSaveJpeg.jpg"), runtime::FileAccessFailed);
        }
        
        void ImageTest::testSerialize()
        {
            m_image = new Image("lenna.jpg");

            runtime::DirectoryFileOutput output(".");
            output.initialize("ImageTest_testSerialize");
            
            CPPUNIT_ASSERT_NO_THROW(m_image->serialize(output));
            CPPUNIT_ASSERT_EQUAL(std::string("5 500 512"), output.getText());
        }
        
        void ImageTest::testSerializeEmpty()
        {
            m_image = new Image(100, 0, runtime::Image::RGB_24);

            runtime::DirectoryFileOutput output(".");
            output.initialize("ImageTest_testSerializeEmpty");
            
            CPPUNIT_ASSERT_NO_THROW(m_image->serialize(output));
            CPPUNIT_ASSERT_EQUAL(std::string("3 100 0"), output.getText());
        }
            
        void ImageTest::testDeserialize()
        {
            m_image = new Image();
            
            runtime::DirectoryFileInput input(".");
            input.initialize("", "lenna.jpg");
            CPPUNIT_ASSERT_NO_THROW(m_image->deserialize(input, VERSION));
            CPPUNIT_ASSERT_EQUAL(Image::BGR_24, m_image->pixelType());
            
            m_image->save("ImageTest_testDeserialize.png");
        }
        
        void ImageTest::testDeserializeWithType()
        {
            m_image = new Image();
            
            runtime::DirectoryFileInput input(".");
            input.initialize("3", "lenna.jpg");
            CPPUNIT_ASSERT_NO_THROW(m_image->deserialize(input, VERSION));
            CPPUNIT_ASSERT_EQUAL(Image::RGB_24, m_image->pixelType());
            
            m_image->save("ImageTest_testDeserializeWithType.png");
        }
                
        void ImageTest::testDeserializeEmpty()
        {
            m_image = new Image();
            
            runtime::DirectoryFileInput input(".");
            input.initialize("3 100 0", "");
            CPPUNIT_ASSERT_NO_THROW(m_image->deserialize(input, VERSION));
            CPPUNIT_ASSERT_EQUAL((unsigned int)(100), m_image->width());
            CPPUNIT_ASSERT_EQUAL((unsigned int)(0), m_image->height());
            CPPUNIT_ASSERT_EQUAL(runtime::Image::RGB_24, m_image->pixelType());
        }
                
        void ImageTest::testResizeBuffer()
        {
            m_image = new Image();
            CPPUNIT_ASSERT_NO_THROW(m_image->resize(300, 200, runtime::Image::BGR_24));
            CPPUNIT_ASSERT_EQUAL((unsigned int)(300), m_image->width());
            CPPUNIT_ASSERT_EQUAL((unsigned int)(200), m_image->height());
            CPPUNIT_ASSERT_EQUAL(runtime::Image::BGR_24, m_image->pixelType());
            CPPUNIT_ASSERT_EQUAL(runtime::DataVariant::BGR_24_IMAGE, m_image->variant());
        }

        void ImageTest::testResizeDimension()
        {
            m_image = new Image();
            CPPUNIT_ASSERT_NO_THROW(m_image->resize(1024));
            CPPUNIT_ASSERT_EQUAL((unsigned int)(1024), m_image->bufferSize());
            CPPUNIT_ASSERT_EQUAL((unsigned int)(1024), m_image->width());
            CPPUNIT_ASSERT_EQUAL((unsigned int)(1), m_image->height());
            CPPUNIT_ASSERT_EQUAL(runtime::Image::NONE, m_image->pixelType());
            CPPUNIT_ASSERT_EQUAL(runtime::DataVariant::IMAGE, m_image->variant());
        }

        void ImageTest::tearDown ( void )
        {
            delete m_image;
        }
    }
}
