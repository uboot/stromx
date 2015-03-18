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

#include <cppunit/TestAssert.h>
#include <stromx/runtime/DataContainer.h>
#include <stromx/runtime/OperatorException.h>
#include <stromx/runtime/OperatorTester.h>
#include <stromx/runtime/Primitive.h>
#include <stromx/runtime/ReadAccess.h>
#include "stromx/cvsupport/ReadDirectory.h"
#include "stromx/cvsupport/Image.h"
#include "stromx/cvsupport/test/ReadDirectoryTest.h"

#include <boost/filesystem.hpp>

CPPUNIT_TEST_SUITE_REGISTRATION (stromx::cvsupport::ReadDirectoryTest);

namespace
{
    const static boost::filesystem::path TEST_DIR = "./ReadDirectoryTest";
    const static boost::filesystem::path EMPTY_DIR = TEST_DIR / boost::filesystem::path("empty_dir");
}

namespace stromx
{
    using namespace runtime;

    namespace cvsupport
    {
        void ReadDirectoryTest::setUp ( void )
        {
            const_cast<std::string &>(ReadDirectory::BASE_DIRECTORY) = TEST_DIR.string();
            m_operator = new runtime::OperatorTester(new ReadDirectory());
        }
        
        void ReadDirectoryTest::testInitialize()
        {
            m_operator->initialize();
            
            // expect one test directory plus ReadDirectory::NO_DIRECTORY
            CPPUNIT_ASSERT_EQUAL(std::size_t(2),
                m_operator->info().parameter(ReadDirectory::DIRECTORY).descriptions().size());
            CPPUNIT_ASSERT_EQUAL(std::string("directory"),
                m_operator->info().parameter(ReadDirectory::DIRECTORY).descriptions()[1].title());
        }
        
        void ReadDirectoryTest::testActivateNoDirectory()
        {
            m_operator->initialize();
            
            CPPUNIT_ASSERT_THROW(m_operator->activate(), OperatorError);
        }
        
        void ReadDirectoryTest::testActivateValidDirectory()
        {
            m_operator->initialize();
            m_operator->setParameter(ReadDirectory::DIRECTORY, Enum(1));
            
            CPPUNIT_ASSERT_NO_THROW(m_operator->activate());
        }
        
        void ReadDirectoryTest::testExecute()
        {
            m_operator->initialize();
            m_operator->setParameter(ReadDirectory::DIRECTORY, Enum(1));
            m_operator->activate();
            
            DataContainer data;
            data = m_operator->getOutputData(ReadDirectory::OUTPUT);
            CPPUNIT_ASSERT_EQUAL((unsigned int)(868),
                                 ReadAccess<runtime::Image>(data).get().width());
            
            m_operator->clearOutputData(ReadDirectory::OUTPUT);
            data = m_operator->getOutputData(ReadDirectory::OUTPUT);
            CPPUNIT_ASSERT_EQUAL((unsigned int)(500),
                                 ReadAccess<runtime::Image>(data).get().width());
            
            m_operator->clearOutputData(ReadDirectory::OUTPUT);
            data = m_operator->getOutputData(ReadDirectory::OUTPUT);
            CPPUNIT_ASSERT_EQUAL((unsigned int)(868),
                                 ReadAccess<runtime::Image>(data).get().width());
        }
        
        void ReadDirectoryTest::testExecuteEmptyDirectory()
        {
            boost::filesystem::create_directory(EMPTY_DIR);
            m_operator->initialize();
            m_operator->setParameter(ReadDirectory::DIRECTORY, Enum(2));
            m_operator->activate();
            
            CPPUNIT_ASSERT_THROW(m_operator->getOutputData(ReadDirectory::OUTPUT), OperatorError);
        }
        
        void ReadDirectoryTest::tearDown ( void )
        {
            delete m_operator;
            boost::filesystem::remove(EMPTY_DIR);
        }
    }
}
