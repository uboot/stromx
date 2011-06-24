#include "DataContainerTest.h"

#include <stream/DataContainer.h>
#include <stream/Exception.h>

#include "TestData.h"

#include <boost/thread.hpp>
#include <boost/bind.hpp>

#include <cppunit/TestAssert.h>


CPPUNIT_TEST_SUITE_REGISTRATION (stream::DataContainerTest);

namespace stream
{
    void DataContainerTest::testDestroy()
    {
        {
            DataContainer copy;
            {
                DataContainer container(new TestData());
                copy = container;
                CPPUNIT_ASSERT(! TestData::wasDestructed);
            }
            
            CPPUNIT_ASSERT(! TestData::wasDestructed);
        }
        
        CPPUNIT_ASSERT(TestData::wasDestructed);
    }
    
    void DataContainerTest::testDestroyDelayed()
    {
        {
            DataContainer container(new TestData());
            boost::thread t(boost::bind(&DataContainerTest::destroyDelayed, this, _1), container);
            CPPUNIT_ASSERT(! TestData::wasDestructed);
            
            t.join();
            CPPUNIT_ASSERT(! TestData::wasDestructed);
        }
        CPPUNIT_ASSERT(TestData::wasDestructed);
    }
    
    void DataContainerTest::destroyDelayed(DataContainer& container)
    {
        boost::this_thread::sleep(boost::posix_time::seconds(1));
    }
    

}