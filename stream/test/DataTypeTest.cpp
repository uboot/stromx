#include "DataTypeTest.h"

#include <stream/DataType.h>

#include <cppunit/TestAssert.h>

CPPUNIT_TEST_SUITE_REGISTRATION (stream::DataTypeTest);

namespace stream
{
    void DataTypeTest::testImageTypes()
    {
        CPPUNIT_ASSERT(DataType::RGB_IMAGE.is(DataType::IMAGE));
        CPPUNIT_ASSERT(DataType::RGB_24_IMAGE.is(DataType::IMAGE));
        CPPUNIT_ASSERT(DataType::RGB_24_IMAGE.is(DataType::RGB_IMAGE));
        
        CPPUNIT_ASSERT(! DataType::IMAGE.is(DataType::RGB_IMAGE));
        CPPUNIT_ASSERT(! DataType::IMAGE.is(DataType::RGB_24_IMAGE));
        CPPUNIT_ASSERT(! DataType::RGB_IMAGE.is(DataType::RGB_24_IMAGE));
        
        CPPUNIT_ASSERT(DataType::MONO_IMAGE.is(DataType::IMAGE));
        CPPUNIT_ASSERT(DataType::MONO_8_IMAGE.is(DataType::IMAGE));
        CPPUNIT_ASSERT(DataType::MONO_8_IMAGE.is(DataType::MONO_IMAGE));
    }

    void DataTypeTest::testIntTypes()
    {
        CPPUNIT_ASSERT(DataType::UINT.is(DataType::INT));
        CPPUNIT_ASSERT(DataType::UINT_8.is(DataType::UINT));
        CPPUNIT_ASSERT(DataType::UINT_16.is(DataType::UINT));
        CPPUNIT_ASSERT(DataType::UINT_32.is(DataType::UINT));
        
        CPPUNIT_ASSERT(! DataType::INT.is(DataType::UINT));
        CPPUNIT_ASSERT(! DataType::UINT.is(DataType::UINT_8));
        CPPUNIT_ASSERT(! DataType::UINT.is(DataType::UINT_16));
        CPPUNIT_ASSERT(! DataType::UINT.is(DataType::UINT_32));
    }
}