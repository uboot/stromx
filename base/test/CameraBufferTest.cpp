#include "CameraBufferTest.h"

#include <cppunit/TestAssert.h>

#include <base/CameraBuffer.h>

CPPUNIT_TEST_SUITE_REGISTRATION (base::camera::CameraBufferTest);

using namespace stream;

namespace base
{
    namespace camera
    {
        void CameraBufferTest::setUp()
        {
            m_operator = new OperatorWrapper(new CameraBuffer());
        }

        void CameraBufferTest::testExecute()
        {
        }

        void CameraBufferTest::tearDown()
        {
            delete m_operator;
        }
    }
}
