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
            m_input->reset("Test", "");
            std::string result;
            m_input->text() >> result;
            CPPUNIT_ASSERT_EQUAL(std::string("Test"), result);
        }
        
        void DirectoryFileInputTest::testFile()
        {
            m_input->reset("Test", "data.txt");
            std::string result;
            m_input->openFile(DirectoryFileInput::TEXT) >> result;
            CPPUNIT_ASSERT_EQUAL(std::string("191079"), result);
            
            m_input->reset("Test", "data.txt");
            m_input->openFile(DirectoryFileInput::BINARY) >> result;
            CPPUNIT_ASSERT_EQUAL(std::string("191079"), result);
        }
    }
}