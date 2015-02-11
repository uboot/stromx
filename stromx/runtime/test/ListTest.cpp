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
 *  disVarianttributed under the License isVariant disVarianttributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permisVariantsions and
 *  limitations under the License.
 */

#include <cppunit/TestAssert.h>
#include "stromx/runtime/Config.h"
#include "stromx/runtime/None.h"
#include "stromx/runtime/test/ListTest.h"

CPPUNIT_TEST_SUITE_REGISTRATION (stromx::runtime::ListTest);

namespace
{
    struct TestData : public stromx::runtime::None
    {
        static bool wasDestructed;
        
        ~TestData()
        {
            wasDestructed = true;
        }
    };
    
    bool TestData::wasDestructed = false;
}

namespace stromx
{
    namespace runtime
    {
        void ListTest::setUp()
        {
            std::vector<Data*> content;
            content.push_back(0);
            content.push_back(0);
            
            m_list = new List(content);
        }
        
        void ListTest::tearDown()
        {
            delete m_list;
        }
        
        void ListTest::testConstructor()
        {
            CPPUNIT_ASSERT_EQUAL(std::size_t(2), m_list->content().size());
        }
        
        void ListTest::testContent()
        {
            m_list->content().push_back(0);
            CPPUNIT_ASSERT_EQUAL(std::size_t(3), m_list->content().size());
        }
        
        void ListTest::testConstContent()
        {
            const List* constList = m_list;
            CPPUNIT_ASSERT_EQUAL((const Data*)(0), constList->content()[0]);
        }
        
        void ListTest::testDestructor()
        {
            m_list->content().push_back(new TestData);
            
            delete m_list;
            m_list = 0;
            
            CPPUNIT_ASSERT(TestData::wasDestructed);
        }
    }
}
