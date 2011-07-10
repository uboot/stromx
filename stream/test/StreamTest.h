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

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

namespace stream
{
    class Stream;
    class Network;
   /* class OperatorWrapper;
    class TestOperator;
    class OperatorNode;
    class Thread;
    class DataContainer;
    class InputNode;*/
    
    class StreamTest : public CPPUNIT_NS :: TestFixture
    {
        CPPUNIT_TEST_SUITE (StreamTest);
        CPPUNIT_TEST(testNetwork);
//         CPPUNIT_TEST(testAddThread);
//         CPPUNIT_TEST(testRemoveThread);
//         CPPUNIT_TEST(testThreads);
        CPPUNIT_TEST_SUITE_END ();

    public:
        StreamTest() {}
        
        void setUp();
        void tearDown();

    protected:
        void testNetwork();
//         void testAddThread();
//         void testRemoveThread();
//         void testThreads();
            
    private:
        Stream* m_stream;
        Network* m_network;
//         std::vector<TestOperator*> m_operators;
//         std::vector<OperatorNode*> m_operatorNodes;
//         DataContainer m_container;
//         Thread* m_thread;
//         InputNode* m_node;
//         OperatorNode* m_operatorNode;
    };
}

#endif // STREAM_STREAMTEST_H 