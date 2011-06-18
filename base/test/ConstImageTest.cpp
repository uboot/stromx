#include "ConstImageTest.h"

#include <base/ConstImage.h>
#include <base/Image.h>
#include <stream/DataContainer.h>
#include <stream/OperatorWrapper.h>

#include <cppunit/TestAssert.h>

CPPUNIT_TEST_SUITE_REGISTRATION (base::ConstImageTest);

using namespace stream;

namespace base
{
    void ConstImageTest::setUp ( void )
    {
        m_operator = new OperatorWrapper(new ConstImage());
        m_operator->activate();
        
        Image image("lenna.jpg");
        m_operator->setParameter(ConstImage::IMAGE, image);
    }
    
    void ConstImageTest::testExecute()
    {
        stream::DataContainer* result = m_operator->getOutputData(ConstImage::OUTPUT);
        
        const Image* image = dynamic_cast<const Image*>(result->getReadAccess());
        CPPUNIT_ASSERT(image);
        
        m_operator->clearOutputData(ConstImage::OUTPUT);
        result = m_operator->getOutputData(ConstImage::OUTPUT);
    }
    
    void ConstImageTest::tearDown ( void )
    {
        delete m_operator;
    }
}