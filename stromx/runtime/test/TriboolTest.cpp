/* 
 *  Copyright 2015 Matthias Fuchs
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
 *  See the License for the specific language governing permisVariantsions and
 *  limitations under the License.
 */

#include <cppunit/TestAssert.h>
#include "stromx/runtime/Version.h"
#include "stromx/runtime/test/DummyInput.h"
#include "stromx/runtime/test/DummyOutput.h"
#include "stromx/runtime/test/TriboolTest.h"

CPPUNIT_TEST_SUITE_REGISTRATION (stromx::runtime::TriboolTest);

namespace stromx
{
    namespace runtime
    {
        const Version TriboolTest::VERSION(STROMX_RUNTIME_VERSION_MAJOR, STROMX_RUNTIME_VERSION_MINOR, STROMX_RUNTIME_VERSION_PATCH);
        
        void TriboolTest::testSerializeUndefined()
        {
            DummyOutput out;
            Tribool value;
            value.serialize(out);
            CPPUNIT_ASSERT_EQUAL(std::string("-1"), out.value());
        }

        void TriboolTest::testDeserializeUndefined()
        {
            DummyInput in("-1");
            Tribool value;
            value.deserialize(in, VERSION);
            CPPUNIT_ASSERT(value.undefined());
        }
        
        void TriboolTest::testSerializeTrue()
        {
            DummyOutput out;
            Tribool value(true);
            value.serialize(out);
            CPPUNIT_ASSERT_EQUAL(std::string("1"), out.value());
        }

        void TriboolTest::testDeserializeTrue()
        {
            DummyInput in("1");
            Tribool value;
            value.deserialize(in, VERSION);
            CPPUNIT_ASSERT(! value.undefined());
            CPPUNIT_ASSERT(value);
        }
        
        void TriboolTest::testSerializeFalse()
        {
            DummyOutput out;
            Tribool value(false);
            value.serialize(out);
            CPPUNIT_ASSERT_EQUAL(std::string("0"), out.value());
        }

        void TriboolTest::testDeserializeFalse()
        {
            DummyInput in("0");
            Tribool value;
            value.deserialize(in, VERSION);
            CPPUNIT_ASSERT(! value.undefined());
            CPPUNIT_ASSERT(! value);
        }
    }
}
