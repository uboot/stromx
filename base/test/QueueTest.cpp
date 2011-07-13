#include "QueueTest.h"

#include <base/Queue.h>
#include <stream/DataContainer.h>
#include <stream/OperatorWrapper.h>
#include <stream/Primitive.h>

#include <cppunit/TestAssert.h>

CPPUNIT_TEST_SUITE_REGISTRATION (base::QueueTest);

using namespace stream;

namespace base
{
    void QueueTest::setUp ( void )
    {
        m_operator = new OperatorWrapper(new Queue());
        m_operator->initialize();
        m_operator->activate();
    }
    
    void QueueTest::testExecute()
    {
    }    

    void QueueTest::tearDown ( void )
    {
        delete m_operator;
    }
}