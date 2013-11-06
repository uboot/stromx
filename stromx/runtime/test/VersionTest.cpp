/* 
 *  Copyright 2013 Matthias Fuchs
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

#include "stromx/runtime/test/VersionTest.h"

#include <cppunit/TestAssert.h>
#include "stromx/runtime/Version.h"

CPPUNIT_TEST_SUITE_REGISTRATION (stromx::runtime::VersionTest);

namespace stromx
{
    namespace runtime
    {
        void VersionTest::testStreamIn()
        {
            Version version;
            
            std::istringstream in("1.2.3");
            in >> version;
            
            CPPUNIT_ASSERT_EQUAL(Version(1, 2, 3), version);
        }

        void VersionTest::testStreamOut()
        {
            Version version(1, 2, 3);
            
            std::ostringstream out;
            out << version;

            CPPUNIT_ASSERT_EQUAL(std::string("1.2.3"), out.str());
        }
    }
}