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
#include <opencv2/core/core.hpp>
#include <stromx/runtime/DirectoryFileInput.h>
#include <stromx/runtime/DirectoryFileOutput.h>
#include <stromx/runtime/Exception.h>
#include <stromx/runtime/None.h>
#include "stromx/cvsupport/Config.h"
#include "stromx/cvsupport/Image.h"
#include "stromx/cvsupport/Utilities.h"
#include "stromx/cvsupport/test/ImageTest.h"

CPPUNIT_TEST_SUITE_REGISTRATION (stromx::cvsupport::ImageTest);

namespace stromx
{
    namespace cvsupport
    {
        const stromx::runtime::Version ImageTest::VERSION(0, 1, 0);
        
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

        void ImageTest::testOpenAs16BitColor()
        {
            CPPUNIT_ASSERT_NO_THROW(m_image = new Image("lenna.jpg", Image::DEPTH_16));
            CPPUNIT_ASSERT_EQUAL(runtime::Image::BGR_48, m_image->pixelType());
        }

        void ImageTest::testOpenAs16BitGrayscale()
        {
            CPPUNIT_ASSERT_NO_THROW(m_image = new Image("lenna_bw.jpg", Image::DEPTH_16));
            CPPUNIT_ASSERT_EQUAL(runtime::Image::MONO_16, m_image->pixelType());
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
            CPPUNIT_ASSERT_EQUAL(runtime::Variant::IMAGE, m_image->variant());
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
            CPPUNIT_ASSERT_EQUAL(runtime::Variant::BGR_24_IMAGE, m_image->variant());
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
            CPPUNIT_ASSERT_EQUAL(runtime::Variant::RGB_24_IMAGE, m_image->variant());
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
            CPPUNIT_ASSERT_EQUAL(runtime::Variant::RGB_24_IMAGE, m_image->variant());
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
            CPPUNIT_ASSERT_EQUAL(runtime::Variant::IMAGE, m_image->variant());
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
            CPPUNIT_ASSERT_EQUAL(runtime::Variant::MONO_8_IMAGE, m_image->variant());
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
            CPPUNIT_ASSERT_EQUAL(runtime::Variant::RGB_24_IMAGE, m_image->variant());
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
            CPPUNIT_ASSERT_EQUAL(runtime::Variant::MONO_8_IMAGE, m_image->variant());
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
        
        void ImageTest::testSave16Bit()
        {
            m_image = new Image("lenna.jpg");
            Image deepImage(m_image->width(), m_image->height(), Image::BGR_48);
            
            cv::Mat cvInImage = getOpenCvMat(*m_image);
            cv::Mat cvOutImage = getOpenCvMat(deepImage);
            cvInImage.convertTo(cvOutImage, CV_8U, 256);
            
            CPPUNIT_ASSERT_NO_THROW(m_image->save("ImageTest_testSave16Bit.png"));
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
        
        void ImageTest::testSerializeAfterInitializeImage()
        {
            m_image = new Image(200, 1, runtime::Image::MONO_8);
            m_image->initializeImage(5, 10, 15, m_image->data(), runtime::Image::RGB_24);
            
            {
                runtime::DirectoryFileOutput output(".");
                output.initialize("ImageTest_testSerializeAfterInitializeImage");
                m_image->serialize(output);
            }
            
            Image result("ImageTest_testSerializeAfterInitializeImage.png");
            CPPUNIT_ASSERT_EQUAL((unsigned int)(5), result.width());
            CPPUNIT_ASSERT_EQUAL((unsigned int)(10), result.height());
        }
        
        void ImageTest::testSerializeAfterInitializeMatrix()
        {
            m_image = new Image(10, 10, runtime::Image::RGB_24);
            m_image->initializeMatrix(10, 8, 30, m_image->data(), runtime::Matrix::INT_16);
            
            {
                runtime::DirectoryFileOutput output(".");
                output.initialize("ImageTest_testSerializeAfterInitializeMatrix");
                m_image->serialize(output);
            }
            
            Image result("ImageTest_testSerializeAfterInitializeMatrix.png");
            CPPUNIT_ASSERT_EQUAL((unsigned int)(10), result.rows());
            CPPUNIT_ASSERT_EQUAL((unsigned int)(8), result.cols());
        }
        
        void ImageTest::testSerializeEmpty()
        {
            m_image = new Image(100, 0, runtime::Image::RGB_24);

            runtime::DirectoryFileOutput output(".");
            output.initialize("ImageTest_testSerializeEmpty");
            
            CPPUNIT_ASSERT_NO_THROW(m_image->serialize(output));
            CPPUNIT_ASSERT_EQUAL(std::string("3 100 0"), output.getText());
        }
        
        void ImageTest::testSaveAfterInitializeImage()
        {
            m_image = new Image(100, 200, runtime::Image::RGB_24);
            m_image->initializeImage(10, 20, 30, m_image->data(), runtime::Image::RGB_24);
            m_image->save("ImageTest_testSaveAfterInitializeImage.png");
            
            Image result("ImageTest_testSaveAfterInitializeImage.png");
            CPPUNIT_ASSERT_EQUAL((unsigned int)(10), result.width());
            CPPUNIT_ASSERT_EQUAL((unsigned int)(20), result.height());
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
        
        void ImageTest::testIsVariant()
        {
            m_image = new Image("lenna.jpg");
            CPPUNIT_ASSERT(m_image->variant().isVariant(runtime::Variant::IMAGE));
            CPPUNIT_ASSERT(! m_image->variant().isVariant(runtime::Variant::NONE));
        }
        
        void ImageTest::testPtrCast()
        {
            m_image = new Image("lenna.jpg");
            CPPUNIT_ASSERT(runtime::data_cast<runtime::Data>(m_image));
            CPPUNIT_ASSERT(runtime::data_cast<runtime::Image>(m_image));
            CPPUNIT_ASSERT_EQUAL(runtime::data_cast<runtime::None>(m_image), static_cast<runtime::None*>(0));
        }
        
        void ImageTest::testRefCast()
        {
            m_image = new Image("lenna.jpg");
            CPPUNIT_ASSERT_NO_THROW(runtime::data_cast<runtime::Data>(*m_image));
            CPPUNIT_ASSERT_NO_THROW(runtime::data_cast<runtime::Image>(*m_image));
            CPPUNIT_ASSERT_THROW(runtime::data_cast<runtime::None>(*m_image), runtime::BadCast);
        }

        void ImageTest::tearDown ( void )
        {
            delete m_image;
        }
    }
}
