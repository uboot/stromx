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
#include "XmlUtilitiesTest.h"
#include "../impl/XmlUtilities.h"

CPPUNIT_TEST_SUITE_REGISTRATION (stromx::core::XmlUtilitiesTest);

namespace stromx
{
    namespace core
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
    }
}
