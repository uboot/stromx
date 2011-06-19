#include "CameraTest.h"

#include <base/Camera.h>
#include <base/Image.h>
#include <stream/DataContainer.h>
#include <stream/OperatorWrapper.h>

#include <cppunit/TestAssert.h>

CPPUNIT_TEST_SUITE_REGISTRATION (base::CameraTest);

using namespace stream;

namespace base
{
    void CameraTest::setUp ( void )
    {
        m_operator = new OperatorWrapper(new Camera());
        m_operator->activate();
        
        Image image("lenna.jpg");
        m_operator->setParameter(Camera::IMAGE, image);
    }
    
    void CameraTest::testExecute()
    {
        stream::DataContainer* result = m_operator->getOutputData(Camera::OUTPUT);
        
        const Image* image = dynamic_cast<const Image*>(result->getReadAccess());
        CPPUNIT_ASSERT(image);
        
        m_operator->clearOutputData(Camera::OUTPUT);
        result = m_operator->getOutputData(Camera::OUTPUT);
    }
    
    void CameraTest::tearDown ( void )
    {
        delete m_operator;
    }
}