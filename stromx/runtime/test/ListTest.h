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
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

#ifndef STROMX_RUNTIME_LISTTEST_H
#define STROMX_RUNTIME_LISTTEST_H

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>
#include "stromx/runtime/List.h"

namespace stromx
{
    namespace runtime
    {
        class ListTest : public CPPUNIT_NS :: TestFixture
        {
            CPPUNIT_TEST_SUITE (ListTest);
            CPPUNIT_TEST(testVariant);
            CPPUNIT_TEST(testConstructor);
            CPPUNIT_TEST(testContent);
            CPPUNIT_TEST(testConstContent);
            CPPUNIT_TEST(testDestructor);
            CPPUNIT_TEST(testTypedList);
            CPPUNIT_TEST_SUITE_END ();

        public:
            ListTest() : m_list(0) {}

            void setUp();
            void tearDown();
            
        protected:
            void testVariant();
            void testConstructor();
            void testContent();
            void testConstContent();
            void testDestructor();
            void testTypedList();
            
        private:
            List* m_list;
        };
    }
}

#endif // STROMX_RUNTIME_LISTTEST_H