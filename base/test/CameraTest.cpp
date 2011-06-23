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
    }
    
    void CameraTest::testExecute()
    {
    }
    
    void CameraTest::tearDown ( void )
    {
        delete m_operator;
    }
}