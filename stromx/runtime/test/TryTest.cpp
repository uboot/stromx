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
#include "stromx/runtime/Try.h"
#include "stromx/runtime/impl/Id2DataMap.h"
#include "stromx/runtime/test/TryTest.h"

CPPUNIT_TEST_SUITE_REGISTRATION (stromx::runtime::TryTest);

namespace stromx
{
    namespace runtime
    {
        void TryTest::setUp ( void )
        {
            std::vector<const Description*> descriptions;
            descriptions.push_back(new Description(0, Variant::NONE));
            descriptions.push_back(new Description(1, Variant::NONE));
            descriptions.push_back(new Description(2, Variant::NONE));
            m_map = new impl::Id2DataMap(descriptions);
            m_dataContainer = DataContainer(new None());
        }

        void TryTest::testSet ( void )
        {
            Id2DataPair pair0(0, m_dataContainer);
            CPPUNIT_ASSERT_NO_THROW((Try(pair0)).set(*m_map));
            CPPUNIT_ASSERT_EQUAL(m_dataContainer, m_map->get(0));
            CPPUNIT_ASSERT(pair0.data().empty());
            
            Id2DataPair pair1(0, m_dataContainer);
            CPPUNIT_ASSERT_NO_THROW((Try(pair1)).set(*m_map));
            CPPUNIT_ASSERT_EQUAL(m_dataContainer, pair1.data());  
        }
        
        void TryTest::testTryGet()
        {
            Id2DataPair pair0(0);
            CPPUNIT_ASSERT_EQUAL(true, (Try(pair0)).tryGet(*m_map));
            
            m_map->set(0, m_dataContainer);
            CPPUNIT_ASSERT_EQUAL(true, (Try(pair0)).tryGet(*m_map));
        }
        
        void TryTest::testTrySet()
        {
            Id2DataPair pair0(0, m_dataContainer);
            CPPUNIT_ASSERT_EQUAL(true, (Try(pair0)).trySet(*m_map));
            
            m_map->set(0, m_dataContainer);
            CPPUNIT_ASSERT_EQUAL(true, (Try(pair0)).trySet(*m_map));
        }

        void TryTest::testGet ( void )
        {
            m_map->set(0, m_dataContainer);
            Id2DataPair pair0(0);
            CPPUNIT_ASSERT_NO_THROW((Try(pair0)).get(*m_map));
            CPPUNIT_ASSERT_EQUAL(m_dataContainer, pair0.data());
            CPPUNIT_ASSERT(m_map->get(0).empty());
            
            Id2DataPair pair1(0);
            CPPUNIT_ASSERT_NO_THROW((Try(pair1)).get(*m_map));
            CPPUNIT_ASSERT(pair1.data().empty());
        }

        void TryTest::tearDown ( void )
        {
            delete m_map;
        }
    }
}