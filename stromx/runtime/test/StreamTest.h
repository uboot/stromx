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

#ifndef STROMX_RUNTIME_STREAMTEST_H
#define STROMX_RUNTIME_STREAMTEST_H

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>
#include <vector>
#include <boost/concept_check.hpp>
#include "stromx/runtime/ExceptionObserver.h"

namespace stromx
{
    namespace runtime
    {
        class Stream;
        class Network;
        class OperatorKernel;
        
        class StreamTest : public CPPUNIT_NS :: TestFixture
        {
            CPPUNIT_TEST_SUITE (StreamTest);
            CPPUNIT_TEST(testOperators);
            CPPUNIT_TEST(testFactory);
            CPPUNIT_TEST(testSetFactory);
            CPPUNIT_TEST(testSetFactoryWrongState);
            CPPUNIT_TEST(testConnect);
            CPPUNIT_TEST(testDisconnect);
            CPPUNIT_TEST(testAddOperator);
            CPPUNIT_TEST(testAddOperatorWithFactory);
            CPPUNIT_TEST(testHideOperator);
            CPPUNIT_TEST(testHideOperatorWithFactory);
            CPPUNIT_TEST(testShowOperator);
            CPPUNIT_TEST(testShowOperatorWithFactory);
            CPPUNIT_TEST(testRemoveOperator);  
            CPPUNIT_TEST(testInitializeOperator);    
            CPPUNIT_TEST(testDeinitializeOperator);           
            CPPUNIT_TEST(testAddThread);
            CPPUNIT_TEST(testHideThread);
            CPPUNIT_TEST(testShowThread);
            CPPUNIT_TEST(testRemoveThread);
            CPPUNIT_TEST(testStart);
            CPPUNIT_TEST(testStartOperatorError);
            CPPUNIT_TEST(testPause);
            CPPUNIT_TEST(testResume);
            CPPUNIT_TEST(testAddObserver);
            CPPUNIT_TEST(testRemoveObserver);
            CPPUNIT_TEST(testObserver);
            CPPUNIT_TEST(testTwoObserver);
            CPPUNIT_TEST(testDelay);
            CPPUNIT_TEST(testStopDelay);
            CPPUNIT_TEST(testDestructorBlockingOperator);
            CPPUNIT_TEST_SUITE_END ();

        public:
            StreamTest() : m_stream(0), m_network(0), m_op1(0), m_op2(0) {}
            
            void setUp();
            void tearDown();

        protected:
            void testOperators();
            void testFactory();
            void testSetFactory();
            void testSetFactoryWrongState();
            void testConnect();
            void testDisconnect();
            void testAddOperator();
            void testAddOperatorWithFactory();
            void testHideOperator();
            void testHideOperatorWithFactory();
            void testShowOperator();
            void testShowOperatorWithFactory();
            void testRemoveOperator();
            void testInitializeOperator();
            void testDeinitializeOperator();
            void testAddThread();
            void testHideThread();
            void testShowThread();
            void testRemoveThread();
            void testStart();
            void testStartOperatorError();
            void testPause();
            void testResume();
            void testAddObserver();
            void testRemoveObserver();
            void testObserver();
            void testTwoObserver();
            void testDelay();
            void testStopDelay();
            void testDestructorBlockingOperator();
                
        private:
            class TestObserver : public ExceptionObserver
            {
            public:
                TestObserver() : m_phase(ExceptionObserver::ACTIVATION), m_thread(0) {}
                
                void observe(const Phase phase, const OperatorError & ex, const Thread* const thread) const;
                
                ExceptionObserver::Phase phase() const { return m_phase; }
                const std::string message() const { return m_message; }
                const Thread* thread() const { return m_thread; }
                
            private:
                mutable ExceptionObserver::Phase m_phase;
                mutable std::string m_message;
                mutable const Thread* m_thread;
            };
            
            Stream* m_stream;
            Network* m_network;
            OperatorKernel* m_op1;
            OperatorKernel* m_op2;
        };
    }
}

#endif // STROMX_RUNTIME_STREAMTEST_H 