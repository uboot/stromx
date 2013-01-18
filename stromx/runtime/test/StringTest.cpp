/* 
 *  Copyright 2012 Matthias Fuchs
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
#include "../Config.h"
#include "StringTest.h"

CPPUNIT_TEST_SUITE_REGISTRATION (stromx::runtime::StringTest);

namespace stromx
{
    namespace runtime
    {
        const Version StringTest::VERSION(STROMX_RUNTIME_VERSION_MAJOR, STROMX_RUNTIME_VERSION_MINOR, STROMX_RUNTIME_VERSION_PATCH);
        
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
    }
}
