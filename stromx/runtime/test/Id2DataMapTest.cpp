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
#include "stromx/runtime/Primitive.h"
#include "stromx/runtime/test/Id2DataMapTest.h"

CPPUNIT_TEST_SUITE_REGISTRATION (stromx::runtime::Id2DataMapTest);

namespace stromx
{
    namespace runtime
    {
        void Id2DataMapTest::setUp()
        {
            std::vector<const Description*> descriptions;
            descriptions.push_back(new Description(0, Variant::NONE));
            descriptions.push_back(new Description(1, Variant::NONE));
            
            m_id2DataMap = new impl::Id2DataMap(descriptions);
            m_id2DataMap->setObserver(m_observer);
            
            for(std::vector<const Description*>::iterator iter = descriptions.begin();
                iter != descriptions.end();
                ++iter)
            {
                delete *iter;
            }
            
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
            CPPUNIT_ASSERT_THROW(m_id2DataMap->get(2), WrongId);
        }
        
        void Id2DataMapTest::testSet()
        {
            DataContainer in(new UInt8());
            m_id2DataMap->set(0, in);
            
            DataContainer out;
            CPPUNIT_ASSERT_NO_THROW(out = m_id2DataMap->get(0));
            CPPUNIT_ASSERT_EQUAL(in, out);
            
            CPPUNIT_ASSERT_THROW(m_id2DataMap->set(2, in), WrongId);
        }

        void Id2DataMapTest::testObserver()
        {
            DataContainer data1(new UInt8());
            m_id2DataMap->set(1, data1);
            
            CPPUNIT_ASSERT_EQUAL((unsigned int)(1), m_observer->lastId());
            CPPUNIT_ASSERT_EQUAL(data1, m_observer->lastData());
            
            DataContainer data2(new UInt8());
            m_id2DataMap->set(0, data2);
            
            CPPUNIT_ASSERT_EQUAL((unsigned int)(0), m_observer->lastId());
            CPPUNIT_ASSERT_EQUAL(data2, m_observer->lastData());
        }

        Id2DataMapTest::Id2DataMapTest()
          : m_observer(0),
            m_id2DataMap(0)
        {
            m_observer = new Observer(this);
        }
        
        Id2DataMapTest::~Id2DataMapTest()
        {
            delete m_observer;
        }
        
        Id2DataMapTest::Observer::Observer(const stromx::runtime::Id2DataMapTest*const test)
          : m_test(test),
            m_lastId(0)
        {}
    
        void Id2DataMapTest::Observer::observe(const unsigned int id, const stromx::runtime::DataContainer& data) const
        {
            m_lastId = id;
            m_lastData = data;
        }
    }
}
