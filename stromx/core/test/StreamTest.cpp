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

#include <cppunit/TestAssert.h> 
#include "StreamTest.h"
#include "TestUtilities.h"
#include "../Exception.h"
#include "../Stream.h"
#include "../Thread.h"
#include "../impl/Network.h"

CPPUNIT_TEST_SUITE_REGISTRATION (stromx::core::StreamTest);

namespace stromx
{
    namespace core
    {
        void StreamTest::setUp()
        {
            m_stream = TestUtilities::buildTestStream();
        }

        void StreamTest::tearDown()
        {            
            if(m_stream)
            {
                m_stream->stop();
                m_stream->join();
            }
            
            delete m_stream;
        }
        
        void StreamTest::testAddThread()
        {
            Thread* thr = 0;
            CPPUNIT_ASSERT_NO_THROW(thr = m_stream->addThread());
            CPPUNIT_ASSERT_EQUAL((unsigned int)(2), (unsigned int)(m_stream->threads().size()));
            CPPUNIT_ASSERT_EQUAL(thr, m_stream->threads()[1]);
        }
        
        void StreamTest::testRemoveThread()
        {
            CPPUNIT_ASSERT_THROW(m_stream->removeThread(0), WrongArgument);
            
            Thread* thr = m_stream->threads()[0];
           
            CPPUNIT_ASSERT_NO_THROW(m_stream->removeThread(thr));
            CPPUNIT_ASSERT_EQUAL((unsigned int)(0), (unsigned int)(m_stream->threads().size()));
            
            CPPUNIT_ASSERT_THROW(m_stream->removeThread(thr), WrongArgument);
        }
        
        void StreamTest::testPause()
        {
            CPPUNIT_ASSERT_THROW(m_stream->pause(), WrongState);
            
            m_stream->start();
            
            CPPUNIT_ASSERT_NO_THROW(m_stream->pause());
            CPPUNIT_ASSERT_EQUAL(Stream::PAUSED, m_stream->status());
            CPPUNIT_ASSERT_THROW(m_stream->pause(), WrongState);
        }

        void StreamTest::testResume()
        {
            CPPUNIT_ASSERT_THROW(m_stream->resume(), WrongState);
            
            m_stream->start();
            m_stream->pause();
            
            CPPUNIT_ASSERT_NO_THROW(m_stream->resume());
            CPPUNIT_ASSERT_EQUAL(Stream::ACTIVE, m_stream->status());
            CPPUNIT_ASSERT_THROW(m_stream->resume(), WrongState);
        }
    }
}