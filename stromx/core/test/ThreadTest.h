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

#ifndef STROM_THREADTEST_H
#define STROM_THREADTEST_H

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

namespace stromx
{
    namespace core
    {
        namespace impl
        {
            class Network;
        }
        class Thread;
        class Operator;
        
        class ThreadTest : public CPPUNIT_NS :: TestFixture
        {
            CPPUNIT_TEST_SUITE (ThreadTest);
            CPPUNIT_TEST(testAddOperator);
            CPPUNIT_TEST(testInsertOperator);
            CPPUNIT_TEST(testRemoveOperator);
            CPPUNIT_TEST_SUITE_END ();

        public:
            ThreadTest() {}
            
            void setUp();
            void tearDown();
            

        protected:
            void testAddOperator();
            void testInsertOperator();
            void testRemoveOperator();
                
        private:
            impl::Network* m_network;
            Thread* m_thread;
            Operator* m_op1;
            Operator* m_op2;
        };
    }
}

#endif // STROM_THREADTEST_H