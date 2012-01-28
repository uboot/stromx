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

#include "DirectoryFileInputTest.h"

#include "../Exception.h"
#include "../DirectoryFileInput.h"

CPPUNIT_TEST_SUITE_REGISTRATION (stromx::core::DirectoryFileInputTest);

namespace stromx
{
    namespace core
    {
        void DirectoryFileInputTest::setUp()
        {
            m_input = new DirectoryFileInput(".");
        }

        void DirectoryFileInputTest::tearDown()
        {
            delete m_input;
        }

        void DirectoryFileInputTest::testText()
        {
            std::string result;
            CPPUNIT_ASSERT_THROW(m_input->text(), WrongState);
            
            m_input->initialize("Test1", "");
            m_input->text() >> result;
            CPPUNIT_ASSERT_EQUAL(std::string("Test1"), result);
            
            m_input->initialize("Test2", "");
            m_input->text() >> result;
            CPPUNIT_ASSERT_EQUAL(std::string("Test2"), result);
        }
        
        void DirectoryFileInputTest::testFile()
        {
            std::string result;
            CPPUNIT_ASSERT_THROW(m_input->openFile(DirectoryFileInput::TEXT), WrongState);
            
            m_input->initialize("Test", "data.txt");
            CPPUNIT_ASSERT_THROW(m_input->file(), WrongState);
            m_input->openFile(DirectoryFileInput::TEXT) >> result;
            CPPUNIT_ASSERT_EQUAL(std::string("191079"), result);
            
            m_input->initialize("Test", "data.txt");
            m_input->openFile(DirectoryFileInput::BINARY) >> result;
            CPPUNIT_ASSERT_EQUAL(std::string("191079"), result);
        }
        
        void DirectoryFileInputTest::testHasFile()
        {
            m_input->initialize("Test", "data.txt");
            CPPUNIT_ASSERT_EQUAL(true, m_input->hasFile());
            
            m_input->initialize("Test", "");
            CPPUNIT_ASSERT_EQUAL(false, m_input->hasFile());
            CPPUNIT_ASSERT_THROW(m_input->openFile(DirectoryFileInput::TEXT), NoInputFile);
        }
    }
}