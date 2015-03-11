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
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

#include "stromx/runtime/ParameterGroup.h"
#include "stromx/runtime/test/ParameterTest.h"

CPPUNIT_TEST_SUITE_REGISTRATION (stromx::runtime::ParameterTest);

namespace stromx
{
    namespace runtime
    {
        void ParameterTest::testParameterGroup()
        {
            ParameterGroup group(0);
            
            CPPUNIT_ASSERT_EQUAL((const Parameter*)(0), group.group());
            CPPUNIT_ASSERT_EQUAL(0, int(group.members().size()));
            
            Parameter param(1, Variant::BOOL, &group);
            
            CPPUNIT_ASSERT_EQUAL((const Parameter*)(&group), param.group());
            CPPUNIT_ASSERT_EQUAL(1, int(group.members().size()));
            CPPUNIT_ASSERT_EQUAL((const Parameter*)(&param), group.members()[0]);
        }
    }
}