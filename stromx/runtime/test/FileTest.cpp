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
 *  See the License for the specific language governing permisVariantsions and
 *  limitations under the License.
 */

#include "stromx/runtime/test/FileTest.h"

#include <cppunit/TestAssert.h>
#include "stromx/runtime/Config.h"
#include "stromx/runtime/File.h"

CPPUNIT_TEST_SUITE_REGISTRATION (stromx::runtime::FileTest);

namespace stromx
{
    namespace runtime
    {
        const Version FileTest::VERSION(0, 1, 0);

        void FileTest::testCopyConstructor()
        {
            File file("stream.xml", File::TEXT);
            File copy(file);
            
            CPPUNIT_ASSERT_EQUAL(std::string("stream.xml"), copy.path());
            CPPUNIT_ASSERT_EQUAL(File::TEXT, copy.mode());
            CPPUNIT_ASSERT_EQUAL(Variant::TEXT_FILE, copy.variant());
            CPPUNIT_ASSERT_EQUAL(std::string("xml"), copy.extension());
        }

        void FileTest::testConstructorText()
        {
            File file("stream.xml", File::TEXT);
            
            CPPUNIT_ASSERT_EQUAL(std::string("stream.xml"), file.path());
            CPPUNIT_ASSERT_EQUAL(File::TEXT, file.mode());
            CPPUNIT_ASSERT_EQUAL(Variant::TEXT_FILE, file.variant());
            CPPUNIT_ASSERT_EQUAL(std::string("xml"), file.extension());
        }

        void FileTest::testConstructorBinary()
        {
            File file("double_matrix.npy", File::BINARY);
            
            CPPUNIT_ASSERT_EQUAL(std::string("double_matrix.npy"), file.path());
            CPPUNIT_ASSERT_EQUAL(File::BINARY, file.mode());
            CPPUNIT_ASSERT_EQUAL(Variant::BINARY_FILE, file.variant());
            CPPUNIT_ASSERT_EQUAL(std::string("npy"), file.extension());
        }

        void FileTest::testConstructorNoFile()
        {
            CPPUNIT_ASSERT_THROW(File("nonsense.ini", File::TEXT), FileAccessFailed);
        }

        void FileTest::testConstructorFileIsDirectory()
        {
            CPPUNIT_ASSERT_THROW(File("directory", File::TEXT), FileAccessFailed);
        }

        void FileTest::testSerializeText()
        {
            DummyOutput out;
            File file("stream.xml", File::TEXT);
            file.serialize(out);
            
            std::ifstream refFile("stream.xml");
            std::string refContent((std::istreambuf_iterator<char>(refFile)),
                                    std::istreambuf_iterator<char>());
            
            CPPUNIT_ASSERT_EQUAL(std::string("text xml"), out.value());
            CPPUNIT_ASSERT_EQUAL(refContent, out.fileValue());
        }

        void FileTest::testSerializeBinary()
        {
            DummyOutput out;
            File file("double_matrix.npy", File::BINARY);
            file.serialize(out);
            
            std::ifstream refFile("double_matrix.npy", std::ifstream::binary);
            std::string refContent((std::istreambuf_iterator<char>(refFile)),
                                    std::istreambuf_iterator<char>());
            
            CPPUNIT_ASSERT_EQUAL(std::string("binary npy"), out.value());
            CPPUNIT_ASSERT_EQUAL(refContent, out.fileValue());
        }

        void FileTest::testDeserializeText()
        {
            std::ifstream refFile("stream.xml");
            std::string refContent((std::istreambuf_iterator<char>(refFile)),
                                    std::istreambuf_iterator<char>());
            DummyInput in("text xml", refContent);
            
            File file;
            file.deserialize(in, VERSION);
            
            std::ifstream stream(file.path().c_str());
            std::string content((std::istreambuf_iterator<char>(stream)),
                                 std::istreambuf_iterator<char>());
                                 
            CPPUNIT_ASSERT_EQUAL(std::string("xml"), file.extension());
            CPPUNIT_ASSERT_EQUAL(File::TEXT, file.mode());
            CPPUNIT_ASSERT_EQUAL(refContent, content);
        }

        void FileTest::testDeserializeBinary()
        {
            std::ifstream refFile("double_matrix.npy", std::ifstream::binary);
            std::string refContent((std::istreambuf_iterator<char>(refFile)),
                                    std::istreambuf_iterator<char>());
            DummyInput in("binary npy", refContent);
            
            File file;
            file.deserialize(in, VERSION);
            
            std::ifstream stream(file.path().c_str(), std::ifstream::binary);
            std::string content((std::istreambuf_iterator<char>(stream)),
                                 std::istreambuf_iterator<char>());
                                 
            CPPUNIT_ASSERT_EQUAL(std::string("npy"), file.extension());
            CPPUNIT_ASSERT_EQUAL(File::BINARY, file.mode());
            CPPUNIT_ASSERT_EQUAL(refContent, content);
        }
    }
}
