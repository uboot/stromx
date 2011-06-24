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
        m_operator->activate();
        m_image = DataContainer(new Image("lenna.jpg"));
        m_operator->setInputData(ConvertPixelType::INPUT, m_image);
    }
    
    void ConvertPixelTypeTest::testExecute()
    {
        m_operator->setParameter(ConvertPixelType::PIXEL_TYPE, Enum(stream::Image::MONO_8));
        
        {
            stream::DataContainer result = m_operator->getOutputData(ConvertPixelType::OUTPUT);

            ReadAccess access = ReadAccess(result);
            const Image* image = dynamic_cast<const Image*>(access());
            CPPUNIT_ASSERT(image);
            CPPUNIT_ASSERT_EQUAL(stream::Image::MONO_8, image->pixelType());
        }
        
        m_operator->clearOutputData(ConvertPixelType::OUTPUT);
        
        m_operator->setInputData(ConvertPixelType::INPUT, m_image);
        stream::DataContainer result = m_operator->getOutputData(ConvertPixelType::OUTPUT);
    }
    
    void ConvertPixelTypeTest::tearDown ( void )
    {
        delete m_operator;
    }
}