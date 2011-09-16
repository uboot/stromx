#include "DataTest.h"

#include <cppunit/TestAssert.h>

#include "TestData.h"

#include <strom/Data.h>
#include <strom/None.h>

CPPUNIT_TEST_SUITE_REGISTRATION (strom::DataTest);

namespace strom
{
    void DataTest::testCastReference()
    {
        TestData testData;
        Data & data(testData);
        
        CPPUNIT_ASSERT_NO_THROW(data_cast<TestData &>(data));
        CPPUNIT_ASSERT_NO_THROW(data_cast<const TestData &>(data));
        
        CPPUNIT_ASSERT_THROW(data_cast<None &>(data), BadCast);
        CPPUNIT_ASSERT_THROW(data_cast<const None &>(data), BadCast);
    }
    
    void DataTest::testCastPointer()
    {
        TestData testData;
        Data* data = &testData;
        
        CPPUNIT_ASSERT_NO_THROW(data_cast<TestData *>(data));
        CPPUNIT_ASSERT_NO_THROW(data_cast<const TestData *>(data));
        
        CPPUNIT_ASSERT_EQUAL((None *)(0), data_cast<None *>(data));
        CPPUNIT_ASSERT_EQUAL((const None *)(0), data_cast<const None *>(data));
    }
}
