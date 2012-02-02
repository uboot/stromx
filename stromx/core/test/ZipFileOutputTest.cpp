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

#include "ZipFileOutputTest.h"

#include "../ZipFileOutput.h"
#include "../Exception.h"

CPPUNIT_TEST_SUITE_REGISTRATION (stromx::core::ZipFileOutputTest);

namespace stromx
{
    namespace core
    {
        void ZipFileOutputTest::setUp()
        {
            m_output = new ZipFileOutput("ZipFileOutputTest.zip");
        }

        void ZipFileOutputTest::tearDown()
        {
            delete m_output;
        }

        void ZipFileOutputTest::testText()
        {
            CPPUNIT_ASSERT_THROW(m_output->text(), WrongState);
            
            m_output->initialize("");
            m_output->text() << "Test";
            CPPUNIT_ASSERT_EQUAL(std::string("Test"), m_output->getText());
        }
                
        void ZipFileOutputTest::testFile()
        {
            CPPUNIT_ASSERT_THROW(m_output->file(), WrongState);
            
            m_output->initialize("DirectoryFileOutputText_testFile");
            CPPUNIT_ASSERT_THROW(m_output->file(), WrongState);
            m_output->openFile("bin", OutputProvider::BINARY);
            m_output->file() << 5;
        }
        
        void ZipFileOutputTest::testGetFilename()
        {
            CPPUNIT_ASSERT_THROW(m_output->getFilename(), WrongState);
            
            m_output->initialize("DirectoryFileOutputText_testGetFilename");
            CPPUNIT_ASSERT_EQUAL(std::string(""), m_output->getFilename());
            m_output->openFile("bin", OutputProvider::BINARY);
            CPPUNIT_ASSERT_EQUAL(std::string("DirectoryFileOutputText_testGetFilename.bin"), m_output->getFilename());
        }
    }
}