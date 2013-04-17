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

#include <cppunit/TestAssert.h>
#include "stromx/runtime/Data.h"
#include "stromx/runtime/None.h"
#include "stromx/runtime/test/DataTest.h"
#include "stromx/runtime/test/TestData.h"

CPPUNIT_TEST_SUITE_REGISTRATION (stromx::runtime::DataTest);

namespace stromx
{
    namespace runtime
    {
        void DataTest::testCastReference()
        {
            TestData testData;
            Data & data(testData);
            const Data & constData(testData);
            
            CPPUNIT_ASSERT_NO_THROW(data_cast<TestData>(data));
            CPPUNIT_ASSERT_NO_THROW(data_cast<TestData>(constData));
        }
        
        void DataTest::testInvalidCastReference()
        {
            TestData testData;
            Data & data(testData);
            const Data & constData(testData);
            
            CPPUNIT_ASSERT_THROW(data_cast<None>(data), BadCast);
            CPPUNIT_ASSERT_THROW(data_cast<None>(constData), BadCast);
        }
        
        void DataTest::testCastPointer()
        {
            TestData testData;
            Data* data = &testData;
            const Data* constData= &testData;
            
            CPPUNIT_ASSERT_NO_THROW(data_cast<TestData>(data));
            CPPUNIT_ASSERT_NO_THROW(data_cast<TestData>(constData));
        }
        
        void DataTest::testInvalidCastPointer()
        {
            TestData testData;
            Data* data = &testData;
            const Data* constData= &testData;
            
            CPPUNIT_ASSERT_EQUAL((None *)(0), data_cast<None>(data));
            CPPUNIT_ASSERT_EQUAL((const None *)(0), data_cast<None>(constData));
        }
        
        void DataTest::testCastNullPointer()
        {
            Data* data = 0;
            const Data* constData= 0;
            
            CPPUNIT_ASSERT_EQUAL((None *)(0), data_cast<None>(data));
            CPPUNIT_ASSERT_EQUAL((const None *)(0), data_cast<None>(constData));
        }
    }
}
