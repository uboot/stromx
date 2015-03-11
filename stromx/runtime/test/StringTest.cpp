/* 
 *  Copyright 2012 Matthias Fuchs
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
#include "stromx/runtime/Config.h"
#include "stromx/runtime/None.h"
#include "stromx/runtime/test/StringTest.h"
#include "stromx/runtime/Version.h"

CPPUNIT_TEST_SUITE_REGISTRATION (stromx::runtime::StringTest);

namespace stromx
{
    namespace runtime
    {
        const Version StringTest::VERSION(0, 1, 0);
        
        void StringTest::testStdStringConstructor()
        {
            std::string testString("test");
            String str(testString);
            CPPUNIT_ASSERT_EQUAL(testString, std::string(str));
        }
        
        void StringTest::testCharConstructor()
        {
            std::string testString("test");
            String str(testString.c_str());
            CPPUNIT_ASSERT_EQUAL(testString, std::string(str));
        }

        void StringTest::testSerialize()
        {
            DummyOutput out;
            String str("test");
            str.serialize(out);
            CPPUNIT_ASSERT_EQUAL(std::string("test"), out.value());
        }

        void StringTest::testDeserialize()
        {
            DummyInput in("test");
            String str;
            str.deserialize(in, VERSION);
            CPPUNIT_ASSERT_EQUAL(std::string("test"), std::string(str));
        }

        void StringTest::testCastToStdString()
        {
            String str("test");
            std::string stdString(str);
            CPPUNIT_ASSERT_EQUAL(std::string("test"), stdString);
        }
        
        void StringTest::testEquality()
        {
            CPPUNIT_ASSERT(String("test") == String("test"));
        }

        void StringTest::testInequality()
        {
            CPPUNIT_ASSERT(String("test1") != String("test2"));
        }
        
        void StringTest::testClone()
        {
            String str("test");
            Data* clone = str.clone();
            
            CPPUNIT_ASSERT_EQUAL(std::string("test"), std::string(*data_cast<String>(clone)));
            delete clone;
        }   
        
        void StringTest::testIsVariant()
        {
            String str("test");
            CPPUNIT_ASSERT(str.variant().isVariant(runtime::Variant::STRING));
            CPPUNIT_ASSERT(! str.variant().isVariant(runtime::Variant::NONE));
        }
        
        void StringTest::testPtrCast()
        {
            String str("test");
            CPPUNIT_ASSERT(runtime::data_cast<runtime::Data>(&str));
            CPPUNIT_ASSERT(runtime::data_cast<runtime::String>(&str));
            CPPUNIT_ASSERT_EQUAL(runtime::data_cast<runtime::None>(&str), static_cast<runtime::None*>(0));
        }
        
        void StringTest::testRefCast()
        {
            String str("test");
            CPPUNIT_ASSERT_NO_THROW(runtime::data_cast<runtime::Data>(str));
            CPPUNIT_ASSERT_NO_THROW(runtime::data_cast<runtime::String>(str));
            CPPUNIT_ASSERT_THROW(runtime::data_cast<runtime::None>(str), runtime::BadCast);
        }
    }
}
