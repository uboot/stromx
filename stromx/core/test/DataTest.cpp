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
#include "DataTest.h"
#include <stromx/core/Data.h>
#include <stromx/core/None.h>
#include "TestData.h"

CPPUNIT_TEST_SUITE_REGISTRATION (stromx::core::DataTest);

namespace stromx
{
    namespace core
    {
        void DataTest::testCastReference()
        {
            TestData testData;
            Data & data(testData);
            
            CPPUNIT_ASSERT_NO_THROW(data_cast<TestData &>(data));
            CPPUNIT_ASSERT_NO_THROW(data_cast<const TestData &>(data));
            
            CPPUNIT_ASSERT_THROW(data_cast<None &>(data), BadCast);
            CPPUNIT_ASSERT_THROW(data_cast<const None &>(data), BadCast);
        }
        
        void DataTest::testCastPointer()
        {
            TestData testData;
            Data* data = &testData;
            
            CPPUNIT_ASSERT_NO_THROW(data_cast<TestData *>(data));
            CPPUNIT_ASSERT_NO_THROW(data_cast<const TestData *>(data));
            
            CPPUNIT_ASSERT_EQUAL((None *)(0), data_cast<None *>(data));
            CPPUNIT_ASSERT_EQUAL((const None *)(0), data_cast<const None *>(data));
        }
    }
}
