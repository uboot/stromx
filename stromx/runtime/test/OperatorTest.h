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

#ifndef STROMX_RUNTIME_OPERATORTEST_H
#define STROMX_RUNTIME_OPERATORTEST_H

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>
#include "stromx/runtime/Connector.h"
#include "stromx/runtime/ConnectorObserver.h"
#include "stromx/runtime/DataContainer.h"

namespace stromx
{
    namespace runtime
    {
        class OperatorTester;
        class TestOperator;

        class OperatorTest : public CPPUNIT_NS :: TestFixture
        {
            CPPUNIT_TEST_SUITE (OperatorTest);
            CPPUNIT_TEST (testSetInputData);
            CPPUNIT_TEST (testGetOutputData);
            CPPUNIT_TEST (testClearOutputData);
            CPPUNIT_TEST (testInitialize);
            CPPUNIT_TEST (testDeinitialize);
            CPPUNIT_TEST (testActivate);
            CPPUNIT_TEST (testDeactivate);
            CPPUNIT_TEST (testGetParameter);
            CPPUNIT_TEST (testGetParameterNoTimeout);
            CPPUNIT_TEST (testGetParameterTimeout);
            CPPUNIT_TEST (testGetParameterStatusNone);
            CPPUNIT_TEST (testSetParameter);
            CPPUNIT_TEST (testSetParameterNoTimeout);
            CPPUNIT_TEST (testSetParameterTimeout);
            CPPUNIT_TEST (testSetParameterStatusNone);
            CPPUNIT_TEST (testAddObserver);
            CPPUNIT_TEST (testRemoveObserver);
            CPPUNIT_TEST (testObserver);
            CPPUNIT_TEST (testTwoObservers);
            CPPUNIT_TEST (testSetFactory);
            CPPUNIT_TEST (testSetFactoryWrongState);
            CPPUNIT_TEST_SUITE_END ();

        public:
            OperatorTest() : m_operator(0), m_testOperator(0) {}
            
            void setUp();
            void tearDown();

        protected:
            void testSetInputData();
            void testGetOutputData();
            void testClearOutputData();
            void testInitialize();
            void testDeinitialize();
            void testActivate();
            void testDeactivate();
            void testGetParameter();
            void testGetParameterNoTimeout();
            void testGetParameterTimeout();
            void testGetParameterStatusNone();
            void testSetParameter();
            void testSetParameterNoTimeout();
            void testSetParameterTimeout();
            void testSetParameterStatusNone();
            void testAddObserver();
            void testRemoveObserver();
            void testObserver();
            void testTwoObservers();
            void testSetFactory();
            void testSetFactoryWrongState();
                
        private:
            class TestObserver : public ConnectorObserver
            {
            public:
                void observe(const Connector & connector, const DataContainer & oldData,
                             const DataContainer & newData, const Thread* const thread) const;
                
                const Connector& lastConnector() const { return m_lastConnector; }
                const DataContainer & lastOldData() const { return m_lastOldData; }
                const DataContainer & lastNewData() const { return m_lastNewData; }
                const Thread* thread() const { return m_thread; }
                
            private:
                mutable Connector m_lastConnector;
                mutable DataContainer m_lastOldData;
                mutable DataContainer m_lastNewData;
                mutable const Thread* m_thread;
            };
            
            void setInputDataDelayed(const unsigned int id); 
            void clearOutputDataDelayed(const unsigned int id); 
            void getOutputDataWithInterrupt(const unsigned id);
            void setInputDataWithInterrupt(const unsigned int id);
            void setWaitingTime(const unsigned int timeInMs);
            
            OperatorTester* m_operator;
            TestOperator* m_testOperator;
            DataContainer m_container;
            TestObserver m_observer1;
            TestObserver m_observer2;
        };
    }
}

#endif // STROMX_RUNTIME_OPERATORTEST_H