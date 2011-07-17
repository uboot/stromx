#include "ConvertPixelTypeTest.h"

#include <base/ConvertPixelType.h>
#include <base/Image.h>
#include <stream/DataContainer.h>
#include <stream/OperatorWrapper.h>
#include <stream/Primitive.h>
#include <stream/ReadAccess.h>

#include <cppunit/TestAssert.h>

CPPUNIT_TEST_SUITE_REGISTRATION (base::ConvertPixelTypeTest);

using namespace stream;

namespace base
{
    void ConvertPixelTypeTest::setUp ( void )
    {
        m_operator = new OperatorWrapper(new ConvertPixelType());
        m_operator->initialize();
        m_operator->activate();
        DataContainer source(new Image("lenna.jpg"));
        m_operator->setInputData(ConvertPixelType::SOURCE, source);
    }
    
    void ConvertPixelTypeTest::testExecuteMono8()
    {
        m_operator->setParameter(ConvertPixelType::PIXEL_TYPE, Enum(stream::Image::MONO_8));
        DataContainer destination(new Image(512, 500, stream::Image::BAYERBG_8));
        m_operator->setInputData(ConvertPixelType::DESTINATION, destination);
        
        stream::DataContainer result = m_operator->getOutputData(ConvertPixelType::OUTPUT);
        
        ReadAccess access = ReadAccess(result);
        const Image* image = dynamic_cast<const Image*>(access());
        CPPUNIT_ASSERT(image);
        CPPUNIT_ASSERT_EQUAL(stream::Image::MONO_8, image->pixelType());
        CPPUNIT_ASSERT_EQUAL((unsigned int)(500), image->width());
        CPPUNIT_ASSERT_EQUAL((unsigned int)(512), image->height());
        
        image->save("ConverPixelTypeTest_testExecuteMono8.png");
    }
    
    void ConvertPixelTypeTest::tearDown ( void )
    {
        delete m_operator;
    }
}