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
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

#ifndef STROMX_RUNTIME_FILETEST_H
#define STROMX_RUNTIME_FILETEST_H

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>
#include "stromx/runtime/InputProvider.h"
#include "stromx/runtime/OutputProvider.h"
#include "stromx/runtime/Version.h"

namespace stromx
{
    namespace runtime
    {
        class FileTest : public CPPUNIT_NS :: TestFixture
        {
            CPPUNIT_TEST_SUITE (FileTest);
            CPPUNIT_TEST(testCopyConstructor);
            CPPUNIT_TEST(testConstructorText);
            CPPUNIT_TEST(testConstructorBinary);
            CPPUNIT_TEST(testConstructorNoFile);
            CPPUNIT_TEST(testConstructorFileIsDirectory);
            CPPUNIT_TEST(testSerializeText);
            CPPUNIT_TEST(testDeserializeText);
            CPPUNIT_TEST(testSerializeBinary);
            CPPUNIT_TEST(testDeserializeBinary);
            CPPUNIT_TEST_SUITE_END ();

        public:
            FileTest() {}
            
            void setUp() {}
            void tearDown() {}

        protected:
            void testCopyConstructor();
            void testConstructorText();
            void testConstructorBinary();
            void testConstructorNoFile();
            void testConstructorFileIsDirectory();
            void testSerializeText();
            void testDeserializeText();
            void testSerializeBinary();
            void testDeserializeBinary();
                
        private: 
            class DummyInput : public InputProvider
            {
            public:
                explicit DummyInput(const std::string & text, const std::string & file)
                  : m_text(text),
                    m_file(file)
                {}
                
                std::istream & text() { return m_text; }
                std::istream & openFile(const OpenMode) { return m_file; }
                bool hasFile() const { return true; }
                std::istream & file() { return m_file; }
                
            private:
                std::istringstream m_text;
                std::istringstream m_file;
            };
            
            class DummyOutput : public OutputProvider
            {
            public:
                std::ostream & text() { return m_text; }
                std::ostream & openFile(const std::string &, const OpenMode) { return m_file; }
                std::ostream & file() { return m_file; }
                
                const std::string value() const { return m_text.str(); }
                const std::string fileValue() const { return m_file.str(); }
                
            private:
                std::ostringstream m_text;
                std::ostringstream m_file;
            };
            
            const static Version VERSION;
        };
    }
}

#endif // STROMX_RUNTIME_FILETEST_H