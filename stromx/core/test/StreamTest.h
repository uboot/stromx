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

#ifndef STROMX_CORE_STREAMTEST_H
#define STROMX_CORE_STREAMTEST_H

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>
#include <vector>
#include "../ExceptionObserver.h"

namespace stromx
{
    namespace core
    {
        class Stream;
        class Network;
        
        class StreamTest : public CPPUNIT_NS :: TestFixture
        {
            CPPUNIT_TEST_SUITE (StreamTest);
            CPPUNIT_TEST(testAddThread);
            CPPUNIT_TEST(testRemoveThread);
            CPPUNIT_TEST(testPause);
            CPPUNIT_TEST(testResume);
            CPPUNIT_TEST(testRemoveOperator);
            CPPUNIT_TEST(testAddObserver);
            CPPUNIT_TEST(testRemoveObserver);
            CPPUNIT_TEST(testObserver);
            CPPUNIT_TEST(testTwoObserver);
            CPPUNIT_TEST_SUITE_END ();

        public:
            StreamTest() {}
            
            void setUp();
            void tearDown();

        protected:
            void testAddThread();
            void testRemoveThread();
            void testPause();
            void testResume();
            void testRemoveOperator();
            void testAddObserver();
            void testRemoveObserver();
            void testObserver();
            void testTwoObserver();
                
        private:
            class TestObserver : public ExceptionObserver
            {
            public:
                void observe(const Phase phase, const OperatorError & ex, const Thread* const thread) const;
                
                const ExceptionObserver::Phase phase() const { return m_phase; }
                const std::string message() const { return m_message; }
                const Thread* const thread() const { return m_thread; }
                
            private:
                mutable ExceptionObserver::Phase m_phase;
                mutable std::string m_message;
                mutable const Thread* m_thread;
            };
            
            Stream* m_stream;
            Network* m_network;
        };
    }
}

#endif // STROMX_CORE_STREAMTEST_H 