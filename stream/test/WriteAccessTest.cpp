#include "WriteAccessTest.h"

#include "TestData.h"

#include <stream/DataContainer.h>
#include <stream/WriteAccess.h>
#include <stream/Exception.h>

#include <cppunit/TestAssert.h>

#include <boost/thread.hpp>
#include <boost/bind.hpp>

CPPUNIT_TEST_SUITE_REGISTRATION (stream::WriteAccessTest);

namespace stream
{
    void WriteAccessTest::testWriteAccess()
    {
        Data* data = new TestData;
        {
            DataContainer container(data);
            WriteAccess access(container);
            CPPUNIT_ASSERT_EQUAL(data, &access());
        }
        
        CPPUNIT_ASSERT(TestData::wasDestructed);  
    }
    
    void WriteAccessTest::testWriteAccessEmpty()
    {
        DataContainer container;
        CPPUNIT_ASSERT_THROW(WriteAccess access(container), ArgumentException);
    }
    
    void WriteAccessTest::testReleaseWriteAccess()
    {
        Data* data = new TestData;
        DataContainer container(data);
        {
            WriteAccess access(container);
        }
        
        WriteAccess access(container);
        CPPUNIT_ASSERT_EQUAL(data, &access());
    }
    
    void WriteAccessTest::testWriteAccessDelayed()
    {
        m_data = new TestData;
        DataContainer container(m_data);
        
        {
            boost::thread t(boost::bind(&WriteAccessTest::releaseDelayed, this, _1), WriteAccess(container));
        }
        
        WriteAccess access(container);
        CPPUNIT_ASSERT_EQUAL(m_data, &access());
    }
    
    void WriteAccessTest::releaseDelayed(WriteAccess& access)
    {
        boost::this_thread::sleep(boost::posix_time::seconds(1));
        CPPUNIT_ASSERT_EQUAL(m_data, &access());
    }
        
    void WriteAccessTest::writeAccessInterrupt(DataContainer& container)
    {
        CPPUNIT_ASSERT_THROW(WriteAccess access(container), InterruptException);
    }
    
    void WriteAccessTest::testWriteAccessInterrupt()
    {
        {
            DataContainer container = DataContainer(new TestData());
            WriteAccess access(container);
            boost::thread t(boost::bind(&WriteAccessTest::writeAccessInterrupt, this, _1), container);
            
            t.interrupt();
            boost::this_thread::sleep(boost::posix_time::seconds(1));
            t.join();
        }
        
        CPPUNIT_ASSERT(TestData::wasDestructed);
    }
}
