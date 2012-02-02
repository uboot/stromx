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

#ifndef STROMX_CORE_PRIMITIVETEST_H
#define STROMX_CORE_PRIMITIVETEST_H

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>
#include "../InputProvider.h"
#include "../OutputProvider.h"
#include "../Primitive.h"

namespace stromx
{
    namespace core
    {
        class PrimitiveTest : public CPPUNIT_NS :: TestFixture
        {
            CPPUNIT_TEST_SUITE (PrimitiveTest);
            CPPUNIT_TEST(testInt32);
            CPPUNIT_TEST(testUInt32);
            CPPUNIT_TEST(testInt8);
            CPPUNIT_TEST(testUInt8);
            CPPUNIT_TEST_SUITE_END ();

        public:
            PrimitiveTest() {}
            
            void setUp() {}
            void tearDown() {}

        protected:
            void testInt32();
            void testUInt32();
            void testInt8();
            void testUInt8();
                
        private: 
            class DummyInput : public InputProvider
            {
            public:
                DummyInput(const std::string & text)
                  : m_text(text),
                    m_file("")
                {}
                
                std::istream & text() { return m_text; }
                std::istream & openFile(const OpenMode mode) { return m_file; }
                const bool hasFile() const { return true; }
                std::istream & file() { return m_file; }
                
            private:
                std::istringstream m_text;
                std::istringstream m_file;
            };
            
            class DummyOutput : public OutputProvider
            {
            public:
                std::ostream & text() { return m_text; }
                std::ostream & openFile(const std::string & ext, const OpenMode mode) { return m_file; }
                std::ostream & file() { return m_file; }
                
                const std::string value() const { return m_text.str(); }
                
            private:
                std::ostringstream m_text;
                std::ostringstream m_file;
            };
            
            const static Version VERSION;
            
            Int8 m_int8;
            UInt8 m_uint8;
            Int32 m_int32;
            UInt32 m_uint32;
        };
    }
}

#endif // STROMX_CORE_PRIMITIVETEST_H