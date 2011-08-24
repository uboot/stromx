/* 
 *  Copyright 2011 Matthias Fuchs
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

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