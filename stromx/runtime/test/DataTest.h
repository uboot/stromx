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

#ifndef STROMX_RUNTIME_DATATEST_H
#define STROMX_RUNTIME_DATATEST_H

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>

namespace stromx
{
    namespace runtime
    {
        class DataTest : public CPPUNIT_NS :: TestFixture
        {
            CPPUNIT_TEST_SUITE (DataTest);
            CPPUNIT_TEST(testCastReference);
            CPPUNIT_TEST(testInvalidCastReference);
            CPPUNIT_TEST(testCastPointer);
            CPPUNIT_TEST(testInvalidCastPointer);
            CPPUNIT_TEST(testCastNullPointer);
            CPPUNIT_TEST_SUITE_END ();

        public:
            DataTest() {}
            
            void setUp() {}
            void tearDown() {}

        protected:
            void testCastReference();
            void testInvalidCastReference();
            void testCastPointer();
            void testInvalidCastPointer();
            void testCastNullPointer();
        };
    }
}

#endif // STROMX_RUNTIME_DATATEST_H