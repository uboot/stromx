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
#include <stromx/core/DataContainer.h>

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
            CPPUNIT_TEST(testAddOperator);
            CPPUNIT_TEST(testInsertOperator);
            CPPUNIT_TEST(testRemoveOperator);
            CPPUNIT_TEST_SUITE_END ();

        public:
            ThreadImplTest() : m_thread(0), m_node(0), m_operatorNode(0) {}
            
            void setUp();
            void tearDown();

        protected:
            void testAddOperator();
            void testInsertOperator();
            void testRemoveOperator();
            
            void testStart();
            void testStop();
            void testJoin();
                
        private: 
            std::vector<TestOperator*> m_kernels;
            std::vector<Operator*> m_operatorNodes;
            DataContainer m_container;
            impl::ThreadImpl* m_thread;
            impl::InputNode* m_node;
            Operator* m_operatorNode;
        };
    }
}

#endif // STROMX_CORE_THREADIMPLTEST_H