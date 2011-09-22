#include "ConvertPixelTypeTest.h"

#include <base/ConvertPixelType.h>
#include <base/Image.h>

#include <strom/DataContainer.h>
#include <strom/Primitive.h>
#include <strom/ReadAccess.h>

#include <strom/OperatorTester.h>

#include <cppunit/TestAssert.h>

CPPUNIT_TEST_SUITE_REGISTRATION (base::ConvertPixelTypeTest);

using namespace strom;

namespace base
{
    void ConvertPixelTypeTest::setUp ( void )
    {
        m_operator = new strom::OperatorTester(new ConvertPixelType());
        m_operator->initialize();
        m_operator->activate();
        Image* image = new Image("lenna.jpg");
        image->initialize(499, 511, image->stride(), image->data(), image->pixelType());
        DataContainer source(image);
        m_operator->setInputData(ConvertPixelType::SOURCE, source);
    }
    
    void ConvertPixelTypeTest::testExecuteMono8()
    {
        m_operator->setParameter(ConvertPixelType::PIXEL_TYPE, Enum(strom::Image::MONO_8));
        DataContainer destination(new Image(512, 500, strom::Image::BAYERBG_8));
        m_operator->setInputData(ConvertPixelType::DESTINATION, destination);
        
        strom::DataContainer result = m_operator->getOutputData(ConvertPixelType::OUTPUT);
        
        ReadAccess<Image> access(result);
        const Image& image = access();
        CPPUNIT_ASSERT_EQUAL(strom::Image::MONO_8, image.pixelType());
        CPPUNIT_ASSERT_EQUAL((unsigned int)(499), image.width());
        CPPUNIT_ASSERT_EQUAL((unsigned int)(511), image.height());
        
        image.save("ConverPixelTypeTest_testExecuteMono8.png");
    }
    
    void ConvertPixelTypeTest::testExecuteBayerBg8()
    {
        m_operator->setParameter(ConvertPixelType::PIXEL_TYPE, Enum(strom::Image::BAYERBG_8));
        DataContainer destination(new Image(512, 500, strom::Image::MONO_8));
        m_operator->setInputData(ConvertPixelType::DESTINATION, destination);
        
        strom::DataContainer result = m_operator->getOutputData(ConvertPixelType::OUTPUT);
        
        ReadAccess<Image> access(result);
        const Image& image = access();
        CPPUNIT_ASSERT_EQUAL(strom::Image::BAYERBG_8, image.pixelType());
        CPPUNIT_ASSERT_EQUAL((unsigned int)(499), image.width());
        CPPUNIT_ASSERT_EQUAL((unsigned int)(511), image.height());
        
        image.save("ConverPixelTypeTest_testExecuteBayerBg8.png");
    }
    
    void ConvertPixelTypeTest::testExecuteBayerRgb24()
    {
        m_operator->setParameter(ConvertPixelType::PIXEL_TYPE, Enum(strom::Image::RGB_24));
        DataContainer destination(new Image(512, 500, strom::Image::BGR_24));
        m_operator->setInputData(ConvertPixelType::DESTINATION, destination);
        
        strom::DataContainer result = m_operator->getOutputData(ConvertPixelType::OUTPUT);
        
        ReadAccess<Image> access(result);
        const Image& image = access();
        CPPUNIT_ASSERT_EQUAL(strom::Image::RGB_24, image.pixelType());
        CPPUNIT_ASSERT_EQUAL((unsigned int)(499), image.width());
        CPPUNIT_ASSERT_EQUAL((unsigned int)(511), image.height());
        
        image.save("ConverPixelTypeTest_testExecuteBayerRgb24.png");
    }
    
    void ConvertPixelTypeTest::tearDown ( void )
    {
        delete m_operator;
    }
}