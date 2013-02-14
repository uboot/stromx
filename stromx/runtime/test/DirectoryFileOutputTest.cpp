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

#include "stromx/runtime/DirectoryFileOutput.h"
#include "stromx/runtime/Exception.h"
#include "stromx/runtime/test/DirectoryFileOutputTest.h"

CPPUNIT_TEST_SUITE_REGISTRATION (stromx::runtime::DirectoryFileOutputTest);

namespace stromx
{
    namespace runtime
    {
        void DirectoryFileOutputTest::setUp()
        {
            m_output = new DirectoryFileOutput(".");
        }

        void DirectoryFileOutputTest::tearDown()
        {
            delete m_output;
        }

        void DirectoryFileOutputTest::testText()
        {
            CPPUNIT_ASSERT_THROW(m_output->text(), WrongState);
            
            m_output->initialize("");
            m_output->text() << "Test";
            CPPUNIT_ASSERT_EQUAL(std::string("Test"), m_output->getText());
        }
                
        void DirectoryFileOutputTest::testFile()
        {
            CPPUNIT_ASSERT_THROW(m_output->file(), WrongState);
            
            // first file
            m_output->initialize("DirectoryFileOutputText_testFile1");
            CPPUNIT_ASSERT_THROW(m_output->file(), WrongState);
            m_output->openFile("bin", OutputProvider::BINARY);
            m_output->file() << 5;
            
            // try a second file
            m_output->initialize("DirectoryFileOutputText_testFile2");
            m_output->openFile("txt", OutputProvider::TEXT);
            m_output->file() << 6;
        }
        
        void DirectoryFileOutputTest::testFileDuplicate()
        {
            // first file
            m_output->initialize("DirectoryFileOutputText_testFileDuplicate");
            m_output->openFile("bin", OutputProvider::BINARY);
            m_output->file() << 5;
            
            // try a second file with the same name
            m_output->initialize("DirectoryFileOutputText_testFileDuplicate");
            m_output->openFile("bin", OutputProvider::BINARY);
            m_output->file() << 6;
        }
        
        void DirectoryFileOutputTest::testGetFilename()
        {
            CPPUNIT_ASSERT_THROW(m_output->getFilename(), WrongState);
            
            m_output->initialize("DirectoryFileOutputText_testGetFilename");
            CPPUNIT_ASSERT_EQUAL(std::string(""), m_output->getFilename());
            m_output->openFile("bin", OutputProvider::BINARY);
            CPPUNIT_ASSERT_EQUAL(std::string("DirectoryFileOutputText_testGetFilename.bin"), m_output->getFilename());
        }
    }
}