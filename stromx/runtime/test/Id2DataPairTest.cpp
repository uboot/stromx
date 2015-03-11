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
#include "stromx/runtime/DataContainer.h"
#include "stromx/runtime/Exception.h"
#include "stromx/runtime/Id2DataPair.h"
#include "stromx/runtime/None.h"
#include "stromx/runtime/impl/Id2DataMap.h"
#include "stromx/runtime/test/Id2DataPairTest.h"

CPPUNIT_TEST_SUITE_REGISTRATION (stromx::runtime::Id2DataPairTest);

namespace stromx
{
    namespace runtime
    {
        void Id2DataPairTest::setUp ( void )
        {
            std::vector<const Description*> descriptions;
            descriptions.push_back(new Description(0, Variant::NONE));
            descriptions.push_back(new Description(1, Variant::NONE));
            descriptions.push_back(new Description(2, Variant::NONE));
            m_map = new impl::Id2DataMap(descriptions);
            m_dataContainer = DataContainer(new None());
        }

        void Id2DataPairTest::testSet ( void )
        {
            Id2DataPair pair0(0);
            CPPUNIT_ASSERT_THROW(pair0.set(*m_map), WrongState);
            
            Id2DataPair pair1(0, m_dataContainer);
            CPPUNIT_ASSERT_NO_THROW(pair1.set(*m_map));
            CPPUNIT_ASSERT_EQUAL(m_dataContainer, m_map->get(0));
            CPPUNIT_ASSERT(pair1.data().empty());
            CPPUNIT_ASSERT_THROW(pair1.set(*m_map), WrongState);
            
            Id2DataPair pair3(10, m_dataContainer);
            CPPUNIT_ASSERT_THROW(pair3.set(*m_map), WrongId);
        }
        
        void Id2DataPairTest::testTryGet()
        {
            Id2DataPair pair0(0);
            CPPUNIT_ASSERT_NO_THROW(pair0.tryGet(*m_map));
            CPPUNIT_ASSERT_EQUAL(false, pair0.tryGet(*m_map));
            
            m_map->set(0, m_dataContainer);
            CPPUNIT_ASSERT_EQUAL(true, pair0.tryGet(*m_map));
            
            Id2DataPair pair3(10);
            CPPUNIT_ASSERT_THROW(pair3.tryGet(*m_map), WrongId);
        }
        
        void Id2DataPairTest::testTrySet()
        {
            Id2DataPair pair0(0, m_dataContainer);
            CPPUNIT_ASSERT_NO_THROW(pair0.trySet(*m_map));
            CPPUNIT_ASSERT_EQUAL(true, pair0.trySet(*m_map));
            
            m_map->set(0, m_dataContainer);
            CPPUNIT_ASSERT_EQUAL(false, pair0.trySet(*m_map));
            
            Id2DataPair pair3(10);
            CPPUNIT_ASSERT_THROW(pair3.trySet(*m_map), WrongId);
        }

        void Id2DataPairTest::testGet ( void )
        {
            m_map->set(0, m_dataContainer);
            Id2DataPair pair0(0);
            CPPUNIT_ASSERT_NO_THROW(pair0.get(*m_map));
            CPPUNIT_ASSERT_EQUAL(m_dataContainer, pair0.data());
            CPPUNIT_ASSERT(m_map->get(0).empty());
            
            CPPUNIT_ASSERT_THROW(pair0.get(*m_map), WrongState);
            Id2DataPair pair3(10);
            CPPUNIT_ASSERT_THROW(pair3.get(*m_map), WrongId);
        }

        void Id2DataPairTest::tearDown ( void )
        {
            delete m_map;
        }
    }
}