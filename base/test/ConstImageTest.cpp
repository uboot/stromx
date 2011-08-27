#include "ConstImageTest.h"

#include <base/ConstImage.h>
#include <base/Image.h>
#include <stream/DataContainer.h>
#include <stream/ReadAccess.h>

#include <stream/impl/SynchronizedOperatorKernel.h>

#include <cppunit/TestAssert.h>

CPPUNIT_TEST_SUITE_REGISTRATION (base::ConstImageTest);

using namespace stream;

namespace base
{
    void ConstImageTest::setUp ( void )
    {
        m_operator = new stream::impl::SynchronizedOperatorKernel(new ConstImage());
        m_operator->initialize();
        m_operator->activate();
        
        Image image("lenna.jpg");
        m_operator->setParameter(ConstImage::IMAGE, image);
    }
    
    void ConstImageTest::testExecute()
    {
        stream::DataContainer result = m_operator->getOutputData(ConstImage::OUTPUT);
        
        ReadAccess access = ReadAccess(result);
        const Image& image = dynamic_cast<const Image&>(access());
        
        image.save("ConstImageTest_testExecute.png");
    }
    
    void ConstImageTest::tearDown ( void )
    {
        delete m_operator;
    }
}