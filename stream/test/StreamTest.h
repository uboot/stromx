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

#ifndef STREAM_STREAMTEST_H
#define STREAM_STREAMTEST_H

#include <vector>

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

namespace stream
{
    class Stream;
    class Network;
    
    class StreamTest : public CPPUNIT_NS :: TestFixture
    {
        CPPUNIT_TEST_SUITE (StreamTest);
        CPPUNIT_TEST(testInit);
        CPPUNIT_TEST(testAddThread);
        CPPUNIT_TEST(testRemoveThread);
        CPPUNIT_TEST_SUITE_END ();

    public:
        StreamTest() {}
        
        void setUp();
        void tearDown();

    protected:
        void testInit();
        void testAddThread();
        void testRemoveThread();
            
    private:
        Stream* m_stream;
        Network* m_network;
    };
}

#endif // STREAM_STREAMTEST_H 