#include "BaseTest.h"

#include <strom/Factory.h>
#include <strom/Stream.h>
#include <strom/Operator.h>
#include <strom/Data.h>

#include <base/Base.h>

#include <cppunit/TestAssert.h>

CPPUNIT_TEST_SUITE_REGISTRATION (base::BaseTest);

namespace base
{
    void BaseTest::setUp ( void )
    {
        m_factory = new strom::Factory;
    }
    
    void BaseTest::testRegisterBase()
    {
        CPPUNIT_ASSERT_NO_THROW(registerBase(m_factory));
        
        strom::Operator* kernel = 0;
        CPPUNIT_ASSERT_NO_THROW(kernel = m_factory->newOperator("Base", "Camera"));
        CPPUNIT_ASSERT(kernel);
        delete kernel;
        
        strom::Data* data = 0;
        CPPUNIT_ASSERT_NO_THROW(data = m_factory->newData("Base", "Image"));
        CPPUNIT_ASSERT(data);
        delete data;
    }

    void BaseTest::testRegisterStream()
    {
        CPPUNIT_ASSERT_NO_THROW(registerStream(m_factory));
        
        strom::Data* data = 0;
        CPPUNIT_ASSERT_NO_THROW(data = m_factory->newData("Stream", "UInt32"));
        CPPUNIT_ASSERT(data);
        delete data;
    }
    
    void BaseTest::tearDown ( void )
    {
        delete m_factory;
    }
}