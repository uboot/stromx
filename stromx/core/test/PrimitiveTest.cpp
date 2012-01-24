/* 
 *  Copyright 2011 Matthias Fuchs
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use thisVariant file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  disVarianttributed under the License isVariant disVarianttributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permisVariantsions and
 *  limitations under the License.
 */

#include <cppunit/TestAssert.h>
#include "PrimitiveTest.h"

CPPUNIT_TEST_SUITE_REGISTRATION (stromx::core::PrimitiveTest);

namespace stromx
{
    namespace core
    {
        void PrimitiveTest::testInt32()
        {
            CPPUNIT_ASSERT_EQUAL(int(0), int(m_int32));
            CPPUNIT_ASSERT(m_int32.isVariant(DataVariant::INT_32));
            
            DummyOutput out;
            m_int32 = Int32(-235);
            m_int32.serialize(out);
            CPPUNIT_ASSERT_EQUAL(std::string("-235"), out.value());
            
            DummyInput in("-233");
            m_int32.deserialize(in);
            CPPUNIT_ASSERT_EQUAL(Int32(-233), m_int32);
        }
        
        void PrimitiveTest::testUInt32()
        {
            CPPUNIT_ASSERT_EQUAL((unsigned int)(0), (unsigned int)(m_uint32));
            CPPUNIT_ASSERT_EQUAL((unsigned int)(0), (unsigned int)(UInt32::MIN));
            CPPUNIT_ASSERT_EQUAL((unsigned int)(UINT32_MAX), (unsigned int)(UInt32::MAX));
            CPPUNIT_ASSERT(m_uint32.isVariant(DataVariant::UINT_32));
            
            DummyOutput out;
            m_uint32 = UInt32(235);
            m_uint32.serialize(out);
            CPPUNIT_ASSERT_EQUAL(std::string("235"), out.value());
            
            DummyInput in("233");
            m_uint32.deserialize(in);
            CPPUNIT_ASSERT_EQUAL(UInt32(233), m_uint32);
        }
        
        void PrimitiveTest::testInt8()
        {
            CPPUNIT_ASSERT_EQUAL(int(0), int(m_int8));
            CPPUNIT_ASSERT(m_int8.isVariant(DataVariant::INT_8));
            
            DummyOutput out;
            m_int8 = Int8(-35);
            m_int8.serialize(out);
            CPPUNIT_ASSERT_EQUAL(std::string("-35"), out.value());
            
            DummyInput in("-33");
            m_int8.deserialize(in);
            CPPUNIT_ASSERT_EQUAL(Int8(-33), m_int8);
        }
        
        void PrimitiveTest::testUInt8()
        {
            CPPUNIT_ASSERT_EQUAL((unsigned int)(0), (unsigned int)(m_uint8));
            CPPUNIT_ASSERT_EQUAL((unsigned int)(0), (unsigned int)(UInt8::MIN));
            CPPUNIT_ASSERT_EQUAL((unsigned int)(UINT8_MAX), (unsigned int)(UInt8::MAX));
            CPPUNIT_ASSERT(m_uint8.isVariant(DataVariant::UINT_8));
            
            DummyOutput out;
            m_uint8 = UInt8(235);
            m_uint8.serialize(out);
            CPPUNIT_ASSERT_EQUAL(std::string("235"), out.value());
            
            DummyInput in("233");
            m_uint8.deserialize(in);
            CPPUNIT_ASSERT_EQUAL(UInt8(233), m_uint8);
        } 
    }
}
