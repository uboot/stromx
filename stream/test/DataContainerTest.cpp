#include "DataContainerTest.h"

#include <DataContainer.h>
#include <None.h>
#include <Exception.h>

#include <boost/thread.hpp>
#include <boost/bind.hpp>

#include <cppunit/TestAssert.h>

CPPUNIT_TEST_SUITE_REGISTRATION (stream::DataContainerTest);

namespace stream
{
    void DataContainerTest::setUp ( void )
    {
        Data* data = new stream::None;
        m_container = new DataContainer(data);
        m_data = data;
    }
    
    void DataContainerTest::testReference()
    {
        CPPUNIT_ASSERT_NO_THROW(m_container->reference());
        m_container->dereference();
        
        m_container->getWriteAccess();
        boost::thread t1(boost::bind(&DataContainerTest::clearWriteAccessDelayed, this));
        CPPUNIT_ASSERT_NO_THROW(m_container->reference());      
    }

    void DataContainerTest::testDereference ( void )
    {
        m_container->reference();
        CPPUNIT_ASSERT_NO_THROW(m_container->dereference());
        
        m_container->reference();
        m_container->reference();
        CPPUNIT_ASSERT_NO_THROW(m_container->dereference());
        CPPUNIT_ASSERT_NO_THROW(m_container->dereference()); 
        CPPUNIT_ASSERT_NO_THROW(m_container->dereference()); 
        m_container = 0;
    }

    void DataContainerTest::testGetReadAccess()
    {
        const Data* data = 0;
        CPPUNIT_ASSERT_NO_THROW(data = m_container->getReadAccess());
        CPPUNIT_ASSERT_EQUAL(m_data, data);
    }

    void DataContainerTest::testGetWriteAccess()
    {
        const Data* data = 0;
        CPPUNIT_ASSERT_NO_THROW(data = m_container->getWriteAccess());
        CPPUNIT_ASSERT_EQUAL(m_data, data);
        
        CPPUNIT_ASSERT_NO_THROW(m_container->getWriteAccess());
        m_container->clearWriteAccess();
        
        m_container->reference();
        boost::thread t1(boost::bind(&DataContainerTest::dereferenceDelayed, this));
        
        CPPUNIT_ASSERT_NO_THROW(m_container->getWriteAccess());
    }
    
    void DataContainerTest::testClearWriteAccess()
    {
        m_container->getWriteAccess();
        CPPUNIT_ASSERT_NO_THROW(m_container->clearWriteAccess());
        
        m_container->reference();
        m_container->dereference();
        m_container->getWriteAccess();
    }
    
    void DataContainerTest::dereferenceDelayed()
    {
        boost::this_thread::sleep(boost::posix_time::seconds(1));
        m_container->dereference();
    }
    
    void DataContainerTest::clearWriteAccessDelayed()
    {
        boost::this_thread::sleep(boost::posix_time::seconds(1));
        m_container->clearWriteAccess();
    }

    void DataContainerTest::tearDown ( void )
    {
        delete m_container;
        m_data = 0;
    }
}