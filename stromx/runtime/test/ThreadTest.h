/* 
*  Copyright 2011 Matthias Fuchs
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

#ifndef STROMX_RUNTIME_THREADTEST_H
#define STROMX_RUNTIME_THREADTEST_H

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>

namespace stromx
{
    namespace runtime
    {
        namespace impl
        {
            class Network;
        }
        class Thread;
        class OperatorTester;
        
        class ThreadTest : public CPPUNIT_NS :: TestFixture
        {
            CPPUNIT_TEST_SUITE (ThreadTest);
            CPPUNIT_TEST(testAddInput);
            CPPUNIT_TEST(testInsertInput);
            CPPUNIT_TEST(testRemoveInputPosition);
            CPPUNIT_TEST(testRemoveInputOpId);
            CPPUNIT_TEST(testRemoveOperator);
            CPPUNIT_TEST_SUITE_END ();

        public:
            ThreadTest() : m_network(0), m_thread(0), m_op1(0), m_op2(0) {}
            
            void setUp();
            void tearDown();
            

        protected:
            void testAddInput();
            void testInsertInput();
            void testRemoveInputPosition();
            void testRemoveInputOpId();
            void testRemoveOperator();
                
        private:
            impl::Network* m_network;
            Thread* m_thread;
            OperatorTester* m_op1;
            OperatorTester* m_op2;
        };
    }
}

#endif // STROMX_RUNTIME_THREADTEST_H