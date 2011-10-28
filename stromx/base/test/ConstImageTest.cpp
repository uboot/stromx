#include "ConstImageTest.h"


#include <stromx/base/ConstImage.h>

#include <stromx/base/Image.h>
#include <stromx/core/DataContainer.h>
#include <stromx/core/ReadAccess.h>

#include <stromx/core/OperatorTester.h>

#include <cppunit/TestAssert.h>

CPPUNIT_TEST_SUITE_REGISTRATION (base::ConstImageTest);

using namespace core;

namespace base
{
    void ConstImageTest::setUp ( void )
    {
        m_operator = new core::OperatorTester(new ConstImage());
        m_operator->initialize();
        m_operator->activate();
        
        Image image("lenna.jpg");
        m_operator->setParameter(ConstImage::IMAGE, image);
    }
    
    void ConstImageTest::testExecute()
    {
        core::DataContainer result = m_operator->getOutputData(ConstImage::OUTPUT);
        
        ReadAccess<Image> access(result);
        const Image& image = access();
        
        image.save("ConstImageTest_testExecute.png");
    }
    
    void ConstImageTest::tearDown ( void )
    {
        delete m_operator;
    }
}