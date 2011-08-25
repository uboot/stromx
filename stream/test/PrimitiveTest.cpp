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

#include "PrimitiveTest.h"

#include <cppunit/TestAssert.h>

CPPUNIT_TEST_SUITE_REGISTRATION (stream::PrimitiveTest);

namespace stream
{
    void PrimitiveTest::testInt32()
    {
        CPPUNIT_ASSERT_EQUAL(int(0), int(m_int32));
        CPPUNIT_ASSERT(m_int32.is(DataVariant::INT_32));
        
        m_int32 = Int32(-235);
        CPPUNIT_ASSERT_EQUAL(std::string("-235"), m_int32.serialize("", ""));
        
        m_int32.deserialize("-233", "");
        CPPUNIT_ASSERT_EQUAL(Int32(-233), m_int32);
    }
    
    void PrimitiveTest::testUInt32()
    {
        CPPUNIT_ASSERT_EQUAL((unsigned int)(0), (unsigned int)(m_uint32));
        CPPUNIT_ASSERT_EQUAL((unsigned int)(0), (unsigned int)(UInt32::MIN));
        CPPUNIT_ASSERT_EQUAL((unsigned int)(UINT32_MAX), (unsigned int)(UInt32::MAX));
        CPPUNIT_ASSERT(m_uint32.is(DataVariant::UINT_32));
        
        m_uint32 = UInt32(235);
        CPPUNIT_ASSERT_EQUAL(std::string("235"), m_uint32.serialize("", ""));
        
        m_uint32.deserialize("233", "");
        CPPUNIT_ASSERT_EQUAL(UInt32(233), m_uint32);
    }
    
    void PrimitiveTest::testInt8()
    {
        CPPUNIT_ASSERT_EQUAL(int(0), int(m_int8));
        CPPUNIT_ASSERT(m_int8.is(DataVariant::INT_8));
        
        m_int8 = Int8(-35);
        CPPUNIT_ASSERT_EQUAL(std::string("-35"), m_int8.serialize("", ""));
        
        m_int8.deserialize("-33", "");
        CPPUNIT_ASSERT_EQUAL(Int8(-33), m_int8);
    }
    
    void PrimitiveTest::testUInt8()
    {
        CPPUNIT_ASSERT_EQUAL((unsigned int)(0), (unsigned int)(m_uint8));
        CPPUNIT_ASSERT_EQUAL((unsigned int)(0), (unsigned int)(UInt8::MIN));
        CPPUNIT_ASSERT_EQUAL((unsigned int)(UINT8_MAX), (unsigned int)(UInt8::MAX));
        CPPUNIT_ASSERT(m_uint8.is(DataVariant::UINT_8));
        
        m_uint8 = UInt8(235);
        CPPUNIT_ASSERT_EQUAL(std::string("235"), m_uint8.serialize("", ""));
        
        m_uint8.deserialize("233", "");
        CPPUNIT_ASSERT_EQUAL(UInt8(233), m_uint8);
    } 
}

