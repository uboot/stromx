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

#ifndef STREAM_PRIMITIVETEST_H
#define STREAM_PRIMITIVETEST_H

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include <stream/Primitive.h>

namespace stream
{
    class PrimitiveTest : public CPPUNIT_NS :: TestFixture
    {
        CPPUNIT_TEST_SUITE (PrimitiveTest);
        CPPUNIT_TEST(testInt32);
        CPPUNIT_TEST(testUInt32);
        CPPUNIT_TEST(testInt8);
        CPPUNIT_TEST(testUInt8);
        CPPUNIT_TEST_SUITE_END ();

    public:
        PrimitiveTest() {}
        
        void setUp() {}
        void tearDown() {}

    protected:
        void testInt32();
        void testUInt32();
        void testInt8();
        void testUInt8();
            
    private: 
        Int8 m_int8;
        UInt8 m_uint8;
        Int32 m_int32;
        UInt32 m_uint32;
    };
}

#endif // STREAM_PRIMITIVETEST_H