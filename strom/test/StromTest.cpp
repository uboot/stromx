#include "StromTest.h"

#include <cppunit/TestAssert.h>

#include <strom/Strom.h>
#include <strom/Factory.h>
#include <strom/Operator.h>
#include <strom/Data.h>

CPPUNIT_TEST_SUITE_REGISTRATION (strom::StromTest);

namespace strom
{
    void StromTest::setUp ( void )
    {
        m_factory = new strom::Factory;
    }
    
    void StromTest::testRegisterStrom()
    {
        CPPUNIT_ASSERT_NO_THROW(registerStrom(m_factory));
        
        strom::Data* data = 0;
        CPPUNIT_ASSERT_NO_THROW(data = m_factory->newData("Strom", "UInt32"));
        CPPUNIT_ASSERT(data);
        delete data;
    }
    
    void StromTest::tearDown ( void )
    {
        delete m_factory;
    }
}
