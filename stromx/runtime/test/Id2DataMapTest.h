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

#ifndef STROMX_RUNTIME_ID2DATAMAPTEST_H
#define STROMX_RUNTIME_ID2DATAMAPTEST_H

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>
#include "stromx/runtime/DataContainer.h"
#include "stromx/runtime/Input.h"
#include "stromx/runtime/Output.h"
#include "stromx/runtime/Parameter.h"
#include "stromx/runtime/impl/Id2DataMap.h"
#include "stromx/runtime/impl/ConnectorParameter.h"

namespace stromx
{
    namespace runtime
    {
        class Id2DataMapTest : public CPPUNIT_NS :: TestFixture
        {
            CPPUNIT_TEST_SUITE (Id2DataMapTest);
            CPPUNIT_TEST(testSet);
            CPPUNIT_TEST(testGet);
            CPPUNIT_TEST(testObserver);
            CPPUNIT_TEST(testCanBeSet);
            CPPUNIT_TEST(testMustBeReset);
            CPPUNIT_TEST(testSetInputMap);
            CPPUNIT_TEST(testSetOutputMap);
            CPPUNIT_TEST_SUITE_END ();

        public:
            Id2DataMapTest();
            ~Id2DataMapTest();
            
            void setUp();
            void tearDown();

        protected:
            void testSet();
            void testGet();
            void testObserver();
            void testCanBeSet();
            void testMustBeReset();
            void testSetInputMap();
            void testSetOutputMap();
                
        private:
            class Observer : public impl::Id2DataMapObserver
            {
        public:
                explicit Observer(const Id2DataMapTest* const test);
                virtual ~Observer() {}
                virtual void observe(const unsigned int id, const DataContainer & oldData,
                                     const DataContainer & newData) const;
                
                unsigned int lastId() const { return m_lastId; }
                const DataContainer & lastOldData() const { return m_lastOldData; }
                const DataContainer & lastNewData() const { return m_lastNewData; }
                
            private:
                const Id2DataMapTest* m_test;
                mutable unsigned int m_lastId;
                mutable DataContainer m_lastOldData;
                mutable DataContainer m_lastNewData;
            };
            
            Observer* m_observer;
            impl::Id2DataMap* m_inputMap;
            impl::Id2DataMap* m_outputMap;
            Input m_input0;
            Input m_input1;
            Input m_input2;
            Input m_input3;
            Output m_output0;
            impl::ConnectorParameter m_param0;
            impl::ConnectorParameter m_param1;
            impl::ConnectorParameter m_param2;
            Parameter m_param3;
        };
    }
}

#endif // STROMX_RUNTIME_ID2DATAMAPTEST_H