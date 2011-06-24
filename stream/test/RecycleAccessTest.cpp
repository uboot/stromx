#include "RecycleAccessTest.h"

#include "TestData.h"

#include <stream/DataContainer.h>
#include <stream/RecycleAccess.h>
#include <stream/Exception.h>

#include <cppunit/TestAssert.h>

#include <boost/thread.hpp>
#include <boost/bind.hpp>

CPPUNIT_TEST_SUITE_REGISTRATION (stream::RecycleAccessTest);

namespace stream
{
    void RecycleAccessTest::testRecycle()
    {
        Data* data = new TestData();
        
        RecycleAccess access;
        
        {
            DataContainer container = DataContainer(data);
            access = RecycleAccess(container);
        }
        
        CPPUNIT_ASSERT_EQUAL(data, access());
        CPPUNIT_ASSERT(! TestData::wasDestructed);
        CPPUNIT_ASSERT_THROW(access(), InvalidStateException);
        
        delete data;
    }
    
    void RecycleAccessTest::testCopiedRecycle()
    {
        Data* data = new TestData();
        
        RecycleAccess access;
        
        {
            DataContainer container = DataContainer(data);
            RecycleAccess localAccess = RecycleAccess(container);
            access = localAccess;
        }
        
        CPPUNIT_ASSERT_EQUAL(data, access());
        CPPUNIT_ASSERT(! TestData::wasDestructed);
        CPPUNIT_ASSERT_THROW(access(), InvalidStateException);
        
        delete data;
    }
    
    void RecycleAccessTest::testNoRecycle()
    {
        Data* data = new TestData();
        
        {
            RecycleAccess access;
            DataContainer container = DataContainer(data);
            access = RecycleAccess(container);
        }
        
        CPPUNIT_ASSERT(TestData::wasDestructed);
    }
    
    void RecycleAccessTest::testDelayedRecycle()
    {
        Data* data = new TestData();
        RecycleAccess access;
        
        {
            DataContainer container = DataContainer(data);
            access = RecycleAccess(container);
            boost::thread t(boost::bind(&RecycleAccessTest::destroyDelayed, this, _1), container);
        }
        
        Data* recycledData = access();
        CPPUNIT_ASSERT_EQUAL(data, recycledData);
    }
    
    void RecycleAccessTest::destroyDelayed(DataContainer& container)
    {
        boost::this_thread::sleep(boost::posix_time::seconds(1));
    }

}
