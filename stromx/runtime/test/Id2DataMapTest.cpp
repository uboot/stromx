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

#include <cppunit/TestAssert.h>
#include "stromx/runtime/Description.h"
#include "stromx/runtime/Exception.h"
#include "stromx/runtime/Input.h"
#include "stromx/runtime/Primitive.h"
#include "stromx/runtime/Variant.h"
#include "stromx/runtime/impl/ConnectorParameter.h"
#include "stromx/runtime/test/Id2DataMapTest.h"

CPPUNIT_TEST_SUITE_REGISTRATION (stromx::runtime::Id2DataMapTest);

namespace stromx
{
    namespace runtime
    {
            Id2DataMapTest::Id2DataMapTest()
          : m_observer(0),
            m_id2DataMap(0),
            m_input0(0, Variant::NONE),
            m_input1(1, Variant::NONE),
            m_input2(2, Variant::NONE),
            m_input3(3, Variant::NONE),
            m_output0(4, Variant::NONE),
            m_param0(&m_input2, DescriptionBase::PERSISTENT),
            m_param1(&m_input3, DescriptionBase::PUSH),
            m_param2(&m_output0, DescriptionBase::PULL),
            m_param3(10, Variant::NONE)
        {
            m_observer = new Observer(this);
        }
        
        void Id2DataMapTest::setUp()
        {
            std::vector<const Input*> descriptions;
            descriptions.push_back(&m_input0);
            descriptions.push_back(&m_input1);
            
            std::vector<const Parameter*> parameters;
            parameters.push_back(&m_param0);
            parameters.push_back(&m_param1);
            parameters.push_back(&m_param2);
            parameters.push_back(&m_param3);
            
            m_id2DataMap = new impl::Id2DataMap;
            m_id2DataMap->initialize(descriptions, parameters);
            m_id2DataMap->setObserver(m_observer);
            
            descriptions.clear();
        }
        
        void Id2DataMapTest::tearDown()
        {
            delete m_id2DataMap;
        }
        
        void Id2DataMapTest::testGet()
        {
            CPPUNIT_ASSERT_NO_THROW(m_id2DataMap->get(0));
            CPPUNIT_ASSERT_NO_THROW(m_id2DataMap->get(1));
            CPPUNIT_ASSERT_THROW(m_id2DataMap->get(10), WrongId);
        }
        
        void Id2DataMapTest::testSet()
        {
            DataContainer in(new UInt8());
            m_id2DataMap->set(0, in);
            
            DataContainer out;
            CPPUNIT_ASSERT_NO_THROW(out = m_id2DataMap->get(0));
            CPPUNIT_ASSERT_EQUAL(in, out);
            
            CPPUNIT_ASSERT_THROW(m_id2DataMap->set(10, in), WrongId);
        }

        void Id2DataMapTest::testObserver()
        {
            DataContainer data1(new UInt8());
            m_id2DataMap->set(1, data1);
            
            CPPUNIT_ASSERT_EQUAL((unsigned int)(1), m_observer->lastId());
            CPPUNIT_ASSERT_EQUAL(DataContainer(), m_observer->lastOldData());
            CPPUNIT_ASSERT_EQUAL(data1, m_observer->lastNewData());
            
            DataContainer data2(new UInt8());
            m_id2DataMap->set(0, data2);
            
            CPPUNIT_ASSERT_EQUAL((unsigned int)(0), m_observer->lastId());
            CPPUNIT_ASSERT_EQUAL(DataContainer(), m_observer->lastOldData());
            CPPUNIT_ASSERT_EQUAL(data2, m_observer->lastNewData());
            
            DataContainer data3(new UInt8());
            m_id2DataMap->set(0, data3);
            
            CPPUNIT_ASSERT_EQUAL((unsigned int)(0), m_observer->lastId());
            CPPUNIT_ASSERT_EQUAL(data2, m_observer->lastOldData());
            CPPUNIT_ASSERT_EQUAL(data3, m_observer->lastNewData());
        }
        
        void Id2DataMapTest::testCanBeSet()
        {
            CPPUNIT_ASSERT(m_id2DataMap->canBeSet(0));
            CPPUNIT_ASSERT(m_id2DataMap->canBeSet(2));
            CPPUNIT_ASSERT(m_id2DataMap->canBeSet(3));
            
            DataContainer data(new UInt8());
            m_id2DataMap->set(0, data);
            m_id2DataMap->set(2, data);
            m_id2DataMap->set(3, data);
            
            CPPUNIT_ASSERT(! m_id2DataMap->canBeSet(0));
            CPPUNIT_ASSERT(m_id2DataMap->canBeSet(2));
            CPPUNIT_ASSERT(! m_id2DataMap->canBeSet(3));
        }
        
        void Id2DataMapTest::testMustBeReset()
        {
            CPPUNIT_ASSERT(m_id2DataMap->mustBeReset(0));
            CPPUNIT_ASSERT(! m_id2DataMap->mustBeReset(2));
            CPPUNIT_ASSERT(m_id2DataMap->mustBeReset(3));
        }
        
        Id2DataMapTest::~Id2DataMapTest()
        {
            delete m_observer;
        }
        
        Id2DataMapTest::Observer::Observer(const stromx::runtime::Id2DataMapTest*const test)
          : m_test(test),
            m_lastId(0)
        {}
    
        void Id2DataMapTest::Observer::observe(const unsigned int id,const DataContainer & oldData,
                                               const DataContainer & newData) const
        {
            m_lastId = id;
            m_lastNewData = newData;
            m_lastOldData = oldData;
        }
    }
}
