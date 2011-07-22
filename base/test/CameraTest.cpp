#include "CameraTest.h"

#include <base/Camera.h>
#include <base/Image.h>

#include <stream/DataContainer.h>
#include <stream/Trigger.h>
#include <stream/OperatorWrapper.h>

#include <cppunit/TestAssert.h>

CPPUNIT_TEST_SUITE_REGISTRATION (base::CameraTest);

using namespace stream;

namespace base
{
    void CameraTest::setUp ( void )
    {
        Image image("lenna.jpg");
        
        m_operator = new OperatorWrapper(new Camera());
        m_operator->initialize();
        m_operator->setParameter(Camera::IMAGE, image);
        m_operator->activate();
    }
    
    void CameraTest::testExecute()
    {
        boost::this_thread::sleep(boost::posix_time::seconds(1));
        m_operator->setParameter(Camera::TRIGGER, stream::Trigger());
        DataContainer image = m_operator->getOutputData(Camera::OUTPUT);
        DataContainer index = m_operator->getOutputData(Camera::INDEX);
    }
    
    void CameraTest::tearDown ( void )
    {
        delete m_operator;
    }
}