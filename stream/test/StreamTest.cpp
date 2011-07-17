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
        delete m_network;
        delete m_stream;
    }

    void StreamTest::testInit()
    {
        CPPUNIT_ASSERT_THROW(new Stream(0),ArgumentException);
    }


    void StreamTest::testNetwork()
    {
        CPPUNIT_ASSERT_EQUAL(m_stream->network(),m_network);
        CPPUNIT_ASSERT(m_stream->network() != 0);
    }
//     
//     void testAddThread()
//     {
//     }
//     
//     void testRemoveThread()
//     {
//     }
//     
//     void testThreads()
//     {
//     }
}