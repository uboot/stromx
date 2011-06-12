#include "ClipTest.h"

#include <Clip.h>

#include <cppunit/TestAssert.h>

CPPUNIT_TEST_SUITE_REGISTRATION (base::ClipTest);

namespace base
{
    void ClipTest::setUp ( void )
    {
        m_operator = new Clip();
    }
    
    void ClipTest::testExecute()
    {

    }

    void ClipTest::tearDown ( void )
    {
        delete m_operator;
    }
}