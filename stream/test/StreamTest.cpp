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

#include "StreamTest.h"

#include <stream/Network.h>
#include <stream/Stream.h>
#include <stream/Thread.h>

#include <cppunit/TestAssert.h> 
#include <stream/Exception.h>

CPPUNIT_TEST_SUITE_REGISTRATION (stream::StreamTest);

namespace stream
{
    void StreamTest::setUp()
    {
        m_network = new Network();
        m_stream = new Stream(m_network);
    }

    void StreamTest::tearDown()
    {
        delete m_stream;
    }

    void StreamTest::testInit()
    {
        CPPUNIT_ASSERT_THROW(new Stream(0),ArgumentException);
    }
    
    void StreamTest::testAddThread()
    {
//         CPPUNIT_ASSERT_THROW(m_stream->addThread(0),ArgumentException);
//         
//         Thread* const thr = new Thread();
//         CPPUNIT_ASSERT_NO_THROW(m_stream->addThread(thr));
//         CPPUNIT_ASSERT_EQUAL((unsigned int)(1), (unsigned int)(m_stream->threads().size()));
//         CPPUNIT_ASSERT_EQUAL(thr, m_stream->threads()[0]);
//         
//         CPPUNIT_ASSERT_THROW(m_stream->addThread(thr),ArgumentException);
    }
    
    void StreamTest::testRemoveThread()
    {
//         CPPUNIT_ASSERT_THROW(m_stream->removeThread(0),ArgumentException);
//         
//         Thread* const thr1 = new Thread();
//         Thread* const thr2 = new Thread();
//         m_stream->addThread(thr1);
//        
//         CPPUNIT_ASSERT_THROW(m_stream->removeThread(thr2),ArgumentException);
//         CPPUNIT_ASSERT_NO_THROW(m_stream->removeThread(thr1));
//         CPPUNIT_ASSERT_EQUAL((unsigned int)(0), (unsigned int)(m_stream->threads().size()));
//         
//         delete thr2;
    }
}