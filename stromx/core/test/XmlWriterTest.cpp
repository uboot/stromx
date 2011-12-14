/* 
 *  Copyright 2011 Thomas Fidler
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
#include "TestUtilities.h"
#include "XmlWriterTest.h"
#include "../Exception.h"
#include "../Stream.h"
#include "../XmlWriter.h"

CPPUNIT_TEST_SUITE_REGISTRATION (stromx::core::XmlWriterTest);

namespace stromx
{
    namespace core
    {
        void XmlWriterTest::setUp()
        {
            m_stream = TestUtilities::buildTestStream();
        }
        
        void XmlWriterTest::testWrite()
        {
            //Attempt to write to filesystem without neccessary permission
            CPPUNIT_ASSERT_THROW(XmlWriter().write("/root/test/XmlWriterTest_testWrite.xml", *m_stream), FileAccessFailed);
            //Attempt to write to filesystem where access is guaranteed (hopefully: it is written to the subdirectory where
            //the test was initiated
            XmlWriter().write("XmlWriterTest_testWrite.xml", *m_stream);
        }

        void XmlWriterTest::tearDown()
        {
            delete m_stream;
        }
    }
}
