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

#ifndef STROMX_CORE_XMLWRITERTEST_H
#define STROMX_CORE_XMLWRITERTEST_H

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>

namespace stromx
{
    namespace core
    {
        class Stream;
        
        class XmlWriterTest : public CPPUNIT_NS :: TestFixture
        {
            CPPUNIT_TEST_SUITE (XmlWriterTest);
            CPPUNIT_TEST(testWriteStream);
            CPPUNIT_TEST(testWriteStreamZip);
            CPPUNIT_TEST(testWriteParameters);
            CPPUNIT_TEST(testWriteParametersZip);
            CPPUNIT_TEST_SUITE_END ();

        public:
            XmlWriterTest() : m_stream(0) {}
            
            void setUp();
            void tearDown();

        protected:
            void testWriteStream();
            void testWriteStreamZip();
            void testWriteParameters();
            void testWriteParametersZip();
                
        private:
            Stream* m_stream;
        };
    }
}

#endif // STROMX_CORE_XMLWRITERTEST_H