#include "ReadAccessTest.h"

#include "TestData.h"

#include <stream/DataContainer.h>
#include <stream/ReadAccess.h>
#include <stream/WriteAccess.h>

#include <cppunit/TestAssert.h>

CPPUNIT_TEST_SUITE_REGISTRATION (stream::ReadAccessTest);

namespace stream
{
    void ReadAccessTest::testReadAccess()
    {
        Data* data = new TestData;
        {
            DataContainer container(data);
            ReadAccess access1(container);
            CPPUNIT_ASSERT_EQUAL(static_cast<const Data*>(data), access1());
            
            ReadAccess access2(container);
            CPPUNIT_ASSERT_EQUAL(static_cast<const Data*>(data), access2());
        }
        
        CPPUNIT_ASSERT(TestData::wasDestructed);  
    }
    
    void ReadAccessTest::testReleaseReadAccess()
    {
        Data* data = new TestData;
        DataContainer container(data);
        {
            ReadAccess access(container);
        }
        
        WriteAccess access(container);
        CPPUNIT_ASSERT_EQUAL(data, access());
    }
}
