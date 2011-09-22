#include "ConstImageTest.h"

#include <base/ConstImage.h>
#include <base/Image.h>
#include <strom/DataContainer.h>
#include <strom/ReadAccess.h>

#include <strom/OperatorTester.h>

#include <cppunit/TestAssert.h>

CPPUNIT_TEST_SUITE_REGISTRATION (base::ConstImageTest);

using namespace strom;

namespace base
{
    void ConstImageTest::setUp ( void )
    {
        m_operator = new strom::OperatorTester(new ConstImage());
        m_operator->initialize();
        m_operator->activate();
        
        Image image("lenna.jpg");
        m_operator->setParameter(ConstImage::IMAGE, image);
    }
    
    void ConstImageTest::testExecute()
    {
        strom::DataContainer result = m_operator->getOutputData(ConstImage::OUTPUT);
        
        ReadAccess<Image> access(result);
        const Image& image = access();
        
        image.save("ConstImageTest_testExecute.png");
    }
    
    void ConstImageTest::tearDown ( void )
    {
        delete m_operator;
    }
}