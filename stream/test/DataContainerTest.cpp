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
    
    void DataContainerTest::testComparison()
    {
        Data* data = new TestData();
        
        DataContainer container1(data);
        DataContainer container2 = container1;
        CPPUNIT_ASSERT(container1 == container2);
        
        DataContainer container3;
        DataContainer container4;
        CPPUNIT_ASSERT(container3 == container4);
        CPPUNIT_ASSERT(container1 != container3);
    }
    
    void DataContainerTest::testEmpty()
    {
        DataContainer container;
        CPPUNIT_ASSERT(container.empty());
        
        Data* data = new TestData();
        container = DataContainer(data);
        CPPUNIT_ASSERT(! container.empty());
        
        container = DataContainer();
        CPPUNIT_ASSERT(container.empty());
    }


}