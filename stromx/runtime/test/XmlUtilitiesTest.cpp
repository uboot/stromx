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
#include "stromx/runtime/impl/XmlUtilities.h"
#include "stromx/runtime/test/XmlUtilitiesTest.h"
#include "stromx/runtime/Version.h"

CPPUNIT_TEST_SUITE_REGISTRATION (stromx::runtime::XmlUtilitiesTest);

namespace stromx
{
    namespace runtime
    {
        using namespace impl;
        
        void XmlUtilitiesTest::testComputePath()
        {
            CPPUNIT_ASSERT_EQUAL(std::string("."), XmlUtilities::computePath("test.xml"));
            CPPUNIT_ASSERT_EQUAL(std::string("/"), XmlUtilities::computePath("/test.xml"));
            CPPUNIT_ASSERT_EQUAL(std::string("dir"), XmlUtilities::computePath("dir/test.xml"));
            CPPUNIT_ASSERT_EQUAL(std::string("/dir"), XmlUtilities::computePath("/dir/test.xml"));
        }
        
        void XmlUtilitiesTest::testComputeName()
        {
            CPPUNIT_ASSERT_EQUAL(std::string("test.xml"), XmlUtilities::computeName("test.xml"));
            CPPUNIT_ASSERT_EQUAL(std::string("test.xml"), XmlUtilities::computeName("/test.xml"));
            CPPUNIT_ASSERT_EQUAL(std::string("test.xml"), XmlUtilities::computeName("dir/test.xml"));
            CPPUNIT_ASSERT_EQUAL(std::string("test.xml"), XmlUtilities::computeName("/dir/test.xml"));
        }
        
        void XmlUtilitiesTest::testStripExtension()
        {
            CPPUNIT_ASSERT_EQUAL(std::string("test"), XmlUtilities::stripExtension("test.xml"));
            CPPUNIT_ASSERT_EQUAL(std::string(""), XmlUtilities::stripExtension(".xml"));
            CPPUNIT_ASSERT_EQUAL(std::string("abc"), XmlUtilities::stripExtension("abc"));
            CPPUNIT_ASSERT_EQUAL(std::string("abc"), XmlUtilities::stripExtension("abc.txt"));
        }
        
        void XmlUtilitiesTest::testComputeExtension()
        {
            CPPUNIT_ASSERT_EQUAL(std::string("xml"), XmlUtilities::computeExtension("test.xml"));
            CPPUNIT_ASSERT_EQUAL(std::string("xml"), XmlUtilities::computeExtension(".xml"));
            CPPUNIT_ASSERT_EQUAL(std::string(""), XmlUtilities::computeExtension("abc"));
            CPPUNIT_ASSERT_EQUAL(std::string("txt"), XmlUtilities::computeExtension("abc.txt"));
        }
                
        void XmlUtilitiesTest::testConvertToVersion()
        {
            Version expected(1, 2, 3);
            Version converted = XmlUtilities::convertToVersion("1.2.3");
            
            CPPUNIT_ASSERT_EQUAL(expected.major(), converted.major());
            CPPUNIT_ASSERT_EQUAL(expected.minor(), converted.minor());
            CPPUNIT_ASSERT_EQUAL(expected.revision(), converted.revision());
        }
    }
}
