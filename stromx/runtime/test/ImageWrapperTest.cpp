/* 
*  Copyright 2013 Matthias Fuchs
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

#include "stromx/runtime/test/ImageWrapperTest.h"

#include <cppunit/TestAssert.h>
#include <opencv2/core/core.hpp>
#include "stromx/runtime/ImageWrapper.h"

CPPUNIT_TEST_SUITE_REGISTRATION (stromx::runtime::ImageWrapperTest);

namespace
{
    using namespace stromx::runtime;
    class ImageImpl : public stromx::runtime::ImageWrapper
    {
    public:
        ImageImpl() : m_data(0) {}
        
        ImageImpl(const unsigned int rows, const unsigned int cols, const PixelType pixelType) 
          : m_data(0) 
        {
            allocate(rows, cols, pixelType);
        }
        const Version & version() const { return VERSION; }
        const std::string & type() const { return TYPE; }
        const std::string & package() const { return PACKAGE; }
        
        Data* clone() const { return new ImageImpl(); }
        
    protected:
        void allocate(const unsigned int width, const unsigned int height, const PixelType pixelType)
        {
            if(m_data)
            {
                delete [] m_data;
                m_data = 0;
            }
            
            unsigned int pixelSize = Image::pixelSize(pixelType);
            unsigned int numChannels = Image::numChannels(pixelType);
            unsigned int stride = width * pixelSize * numChannels;
            unsigned int bufferSize = height * stride;
            m_data = new uint8_t[bufferSize];
            
            setBuffer(m_data, bufferSize);
            initializeImage(width, height, stride, m_data, pixelType);
        }
        
    private:
        static const std::string TYPE;
        static const std::string PACKAGE;
        static const Version VERSION;
        uint8_t* m_data;
    };
    
    const std::string ImageImpl::TYPE = "MatrixImpl";
    const std::string ImageImpl::PACKAGE = "Test";
    const Version ImageImpl::VERSION = Version(0, 1, 0);
}

namespace stromx
{
    namespace runtime
    {
        void ImageWrapperTest::setUp ( void )
        {
            m_image = 0;
        }
               
        void ImageWrapperTest::testResizeLength()
        {
            m_image = new ImageImpl();
            CPPUNIT_ASSERT_NO_THROW(m_image->resize(300, 200, runtime::Image::BGR_24));
            CPPUNIT_ASSERT_EQUAL((unsigned int)(300), m_image->width());
            CPPUNIT_ASSERT_EQUAL((unsigned int)(200), m_image->height());
            CPPUNIT_ASSERT_EQUAL(runtime::Image::BGR_24, m_image->pixelType());
            CPPUNIT_ASSERT_EQUAL(runtime::Variant::BGR_24_IMAGE, m_image->variant());
        }

        void ImageWrapperTest::testResizeDimension()
        {
            m_image = new ImageImpl();
            CPPUNIT_ASSERT_NO_THROW(m_image->resize(1024));
            CPPUNIT_ASSERT_EQUAL((unsigned int)(1024), m_image->bufferSize());
            CPPUNIT_ASSERT_EQUAL((unsigned int)(1024), m_image->width());
            CPPUNIT_ASSERT_EQUAL((unsigned int)(1), m_image->height());
            CPPUNIT_ASSERT_EQUAL(runtime::Image::NONE, m_image->pixelType());
            CPPUNIT_ASSERT_EQUAL(runtime::Variant::IMAGE, m_image->variant());
        }       
        
        void ImageWrapperTest::tearDown ( void )
        {
            delete m_image;
        }
    }
}
