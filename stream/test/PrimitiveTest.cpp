#include "PrimitiveTest.h"

#include <cppunit/TestAssert.h>

CPPUNIT_TEST_SUITE_REGISTRATION (stream::PrimitiveTest);

namespace stream
{
    void PrimitiveTest::testInt32()
    {
        CPPUNIT_ASSERT_EQUAL(int(0), int(m_int32));
        CPPUNIT_ASSERT(m_int32.is(DataType(DataType::INT_32)));
    }
    
    void PrimitiveTest::testUInt32()
    {
        CPPUNIT_ASSERT_EQUAL((unsigned int)(0), (unsigned int)(m_int32));
    }
    
}

