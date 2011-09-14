#include "ConvertPixelTypeTest.h"

#include <base/ConvertPixelType.h>
#include <base/Image.h>

#include <stream/DataContainer.h>
#include <stream/Primitive.h>
#include <stream/ReadAccess.h>

#include <stream/OperatorTester.h>

#include <cppunit/TestAssert.h>

CPPUNIT_TEST_SUITE_REGISTRATION (base::ConvertPixelTypeTest);

using namespace stream;

namespace base
{
    void ConvertPixelTypeTest::setUp ( void )
    {
        m_operator = new stream::OperatorTester(new ConvertPixelType());
        m_operator->initialize();
        m_operator->activate();
        Image* image = new Image("lenna.jpg");
        image->initialize(499, 511, image->stride(), image->data(), image->pixelType());
        DataContainer source(image);
        m_operator->setInputData(ConvertPixelType::SOURCE, source);
    }
    
    void ConvertPixelTypeTest::testExecuteMono8()
    {
        m_operator->setParameter(ConvertPixelType::PIXEL_TYPE, Enum(stream::Image::MONO_8));
        DataContainer destination(new Image(512, 500, stream::Image::BAYERBG_8));
        m_operator->setInputData(ConvertPixelType::DESTINATION, destination);
        
        stream::DataContainer result = m_operator->getOutputData(ConvertPixelType::OUTPUT);
        
        ReadAccess<Image> access(result);
        const Image& image = access();
        CPPUNIT_ASSERT_EQUAL(stream::Image::MONO_8, image.pixelType());
        CPPUNIT_ASSERT_EQUAL((unsigned int)(499), image.width());
        CPPUNIT_ASSERT_EQUAL((unsigned int)(511), image.height());
        
        image.save("ConverPixelTypeTest_testExecuteMono8.png");
    }
    
    void ConvertPixelTypeTest::testExecuteBayerBg8()
    {
        m_operator->setParameter(ConvertPixelType::PIXEL_TYPE, Enum(stream::Image::BAYERBG_8));
        DataContainer destination(new Image(512, 500, stream::Image::MONO_8));
        m_operator->setInputData(ConvertPixelType::DESTINATION, destination);
        
        stream::DataContainer result = m_operator->getOutputData(ConvertPixelType::OUTPUT);
        
        ReadAccess<Image> access(result);
        const Image& image = access();
        CPPUNIT_ASSERT_EQUAL(stream::Image::BAYERBG_8, image.pixelType());
        CPPUNIT_ASSERT_EQUAL((unsigned int)(499), image.width());
        CPPUNIT_ASSERT_EQUAL((unsigned int)(511), image.height());
        
        image.save("ConverPixelTypeTest_testExecuteBayerBg8.png");
    }
    
    void ConvertPixelTypeTest::testExecuteBayerRgb24()
    {
        m_operator->setParameter(ConvertPixelType::PIXEL_TYPE, Enum(stream::Image::RGB_24));
        DataContainer destination(new Image(512, 500, stream::Image::BGR_24));
        m_operator->setInputData(ConvertPixelType::DESTINATION, destination);
        
        stream::DataContainer result = m_operator->getOutputData(ConvertPixelType::OUTPUT);
        
        ReadAccess<Image> access(result);
        const Image& image = access();
        CPPUNIT_ASSERT_EQUAL(stream::Image::RGB_24, image.pixelType());
        CPPUNIT_ASSERT_EQUAL((unsigned int)(499), image.width());
        CPPUNIT_ASSERT_EQUAL((unsigned int)(511), image.height());
        
        image.save("ConverPixelTypeTest_testExecuteBayerRgb24.png");
    }
    
    void ConvertPixelTypeTest::tearDown ( void )
    {
        delete m_operator;
    }
}