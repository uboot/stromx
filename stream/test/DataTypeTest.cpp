#include "DataTypeTest.h"

#include <stream/DataType.h>

#include <cppunit/TestAssert.h>

CPPUNIT_TEST_SUITE_REGISTRATION (stream::DataTypeTest);

namespace stream
{
    void DataTypeTest::testImageTypes()
    {
        CPPUNIT_ASSERT(DataType(DataType::RGB_IMAGE).is(DataType(DataType::IMAGE)));
        CPPUNIT_ASSERT(DataType(DataType::RGB_24_IMAGE).is(DataType(DataType::IMAGE)));
        CPPUNIT_ASSERT(DataType(DataType::RGB_24_IMAGE).is(DataType(DataType::RGB_IMAGE)));
        
        CPPUNIT_ASSERT(! DataType(DataType::IMAGE).is(DataType(DataType::RGB_IMAGE)));
        CPPUNIT_ASSERT(! DataType(DataType::IMAGE).is(DataType(DataType::RGB_24_IMAGE)));
        CPPUNIT_ASSERT(! DataType(DataType::RGB_IMAGE).is(DataType(DataType::RGB_24_IMAGE)));
        
        CPPUNIT_ASSERT(DataType(DataType::MONO_IMAGE).is(DataType(DataType::IMAGE)));
        CPPUNIT_ASSERT(DataType(DataType::MONO_8_IMAGE).is(DataType(DataType::IMAGE)));
        CPPUNIT_ASSERT(DataType(DataType::MONO_8_IMAGE).is(DataType(DataType::MONO_IMAGE)));
    }

    void DataTypeTest::testIntTypes()
    {
        CPPUNIT_ASSERT(DataType(DataType::UINT).is(DataType(DataType::INT)));
        CPPUNIT_ASSERT(DataType(DataType::UINT_8).is(DataType(DataType::UINT)));
        CPPUNIT_ASSERT(DataType(DataType::UINT_16).is(DataType(DataType::UINT)));
        CPPUNIT_ASSERT(DataType(DataType::UINT_32).is(DataType(DataType::UINT)));
        
        CPPUNIT_ASSERT(! DataType(DataType::INT).is(DataType(DataType::UINT)));
        CPPUNIT_ASSERT(! DataType(DataType::UINT).is(DataType(DataType::UINT_8)));
        CPPUNIT_ASSERT(! DataType(DataType::UINT).is(DataType(DataType::UINT_16)));
        CPPUNIT_ASSERT(! DataType(DataType::UINT).is(DataType(DataType::UINT_32)));
    }
}