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

#ifndef STROMX_CORE_THREADIMPLTEST_H
#define STROMX_CORE_THREADIMPLTEST_H

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>
#include "../DataContainer.h"
#include "../impl/ThreadImplObserver.h"

namespace stromx
{
    namespace core
    {
        class SynchronizedOperatorKernel;
        class TestOperator;
        class Operator;
        class DataContainer;
        
        namespace impl
        {
            class ThreadImpl;
            class InputNode;
        }
        
        class ThreadImplTest : public CPPUNIT_NS :: TestFixture
        {
            CPPUNIT_TEST_SUITE (ThreadImplTest);
            CPPUNIT_TEST(testStart);
            CPPUNIT_TEST(testStop);
            CPPUNIT_TEST(testJoin);
            CPPUNIT_TEST(testPause);
            CPPUNIT_TEST(testResume);
            CPPUNIT_TEST(testAddInput);
            CPPUNIT_TEST(testInsertInput);
            CPPUNIT_TEST(testRemoveInput);
            CPPUNIT_TEST(testRemoveOperator);
            CPPUNIT_TEST(testObserver);
            CPPUNIT_TEST_SUITE_END ();

        public:
            ThreadImplTest() : m_thread(0), m_node(0), m_operator(0) {}
            
            void setUp();
            void tearDown();

        protected:
            void testAddInput();
            void testInsertInput();
            void testRemoveInput();
            void testRemoveOperator();
            
            void testStart();
            void testStop();
            void testJoin();
            void testPause();
            void testResume();
            
            void testObserver();
                
        private: 
            class TestObserver : public impl::ThreadImplObserver
            {
            public:
                void observe(const OperatorError & ex) const;
                
                const std::string message() const { return m_message; }
                
            private:
                mutable std::string m_message;
            };
            
            std::vector<TestOperator*> m_kernels;
            std::vector<Operator*> m_operators;
            DataContainer m_container;
            impl::ThreadImpl* m_thread;
            impl::InputNode* m_node;
            Operator* m_operator;
        };
    }
}

#endif // STROMX_CORE_THREADIMPLTEST_H