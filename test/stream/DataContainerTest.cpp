#include "DataContainerTest.h"

#include <stream/DataContainer.h>
#include <stream/None.h>
#include <stream/Exception.h>


#include <cppunit/TestAssert.h>

CPPUNIT_TEST_SUITE_REGISTRATION (stream::DataContainerTest);

namespace stream
{
    void DataContainerTest::setUp ( void )
    {
        Data* data = new stream::None;
        m_container = new DataContainer(0, data);
    }

    void DataContainerTest::referenceTest ( void )
    {
        Data* data = new None;
        DataContainer* container = new DataContainer(0, data);
        
        CPPUNIT_ASSERT_NO_THROW(container->reference());
        CPPUNIT_ASSERT_NO_THROW(container->dereference()); 
        CPPUNIT_ASSERT_NO_THROW(container->dereference());
        CPPUNIT_ASSERT_THROW(container->dereference(), ReferenceCountException);
    }

    void DataContainerTest::ownershipTest()
    {
        CPPUNIT_ASSERT_THROW(m_container->obtainOwnership(0), ArgumentException);
    }

    void DataContainerTest::tearDown ( void )
    {
        delete m_container;
    }
}