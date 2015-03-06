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

#include "stromx/runtime/Exception.h"
#include "stromx/runtime/ZipFileOutput.h"
#include "stromx/runtime/ZipFileInput.h"
#include "stromx/runtime/test/ZipFileOutputTest.h"

CPPUNIT_TEST_SUITE_REGISTRATION (stromx::runtime::ZipFileOutputTest);

namespace stromx
{
    namespace runtime
    {
        void ZipFileOutputTest::testText()
        {
            ZipFileOutput output("ZipFileOutputTest_testText.zip");
            
            CPPUNIT_ASSERT_THROW(output.text(), WrongState);
            
            output.initialize("");
            output.text() << "Test";
            CPPUNIT_ASSERT_EQUAL(std::string("Test"), output.getText());
        }
                
        void ZipFileOutputTest::testFile()
        {
            ZipFileOutput output("ZipFileOutputTest_testFile.zip");
            
            CPPUNIT_ASSERT_THROW(output.file(), WrongState);
            
            // first file
            output.initialize("testFile1");
            CPPUNIT_ASSERT_THROW(output.file(), WrongState);
            output.openFile("bin", OutputProvider::BINARY);
            output.file() << 5;
            
            // try a second file
            output.initialize("testFile2");
            output.openFile("txt", OutputProvider::TEXT);
            output.file() << 6;
        }
        
        void ZipFileOutputTest::testFileDuplicate()
        {
            ZipFileOutput output("ZipFileOutputTest_testFileDuplicate.zip");
            
            // first file
            output.initialize("testFile");
            output.openFile("bin", OutputProvider::BINARY);
            output.file() << 5;
            
            // try a second file with the same name
            output.initialize("testFile");
            output.openFile("bin", OutputProvider::BINARY);
            output.file() << 6;
        }
        
        void ZipFileOutputTest::testGetFilename()
        {
            ZipFileOutput output("ZipFileOutputTest_testGetFilename.zip");
            
            CPPUNIT_ASSERT_THROW(output.getFilename(), WrongState);
            
            output.initialize("testGetFilename");
            CPPUNIT_ASSERT_EQUAL(std::string(""), output.getFilename());
            output.openFile("bin", OutputProvider::BINARY);
            CPPUNIT_ASSERT_EQUAL(std::string("testGetFilename.bin"), output.getFilename());
        }
        
        void ZipFileOutputTest::testNoAccess()
        {
            ZipFileOutput output("/root/test/ZipFileOutputTest_testNoAccess.zip");
            output.initialize("testFile");
            output.openFile("bin", OutputProvider::BINARY);
            output.file() << 5;
            
            CPPUNIT_ASSERT_THROW(output.close(), FileAccessFailed);
        }
    }
}