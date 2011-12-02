#include "BaseTest.h"

#include <stromx/core/Factory.h>
#include <stromx/core/Operator.h>
#include <stromx/core/Data.h>

#include <stromx/base/Base.h>

#include <cppunit/TestAssert.h>

CPPUNIT_TEST_SUITE_REGISTRATION (stromx::base::BaseTest);

namespace stromx
{
    namespace base
    {
        void BaseTest::setUp ( void )
        {
            m_factory = new core::Factory;
        }
        
        void BaseTest::testRegisterBase()
        {
            CPPUNIT_ASSERT_NO_THROW(stromxRegisterBase(*m_factory));
            
            core::Operator* kernel = 0;
            CPPUNIT_ASSERT_NO_THROW(kernel = m_factory->newOperator("Base", "Camera"));
            CPPUNIT_ASSERT(kernel);
            delete kernel;
            
            core::Data* data = 0;
            CPPUNIT_ASSERT_NO_THROW(data = m_factory->newData("Base", "Image"));
            CPPUNIT_ASSERT(data);
            delete data;
        }
        
        void BaseTest::tearDown ( void )
        {
            delete m_factory;
        }
    }
}
