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

#include "Id2DataPairTest.h"

#include <stream/None.h>
#include <stream/Exception.h>
#include <stream/Id2DataPair.h>
#include <stream/DataContainer.h>
#include <stream/impl/Id2DataMap.h>

#include <cppunit/TestAssert.h>

CPPUNIT_TEST_SUITE_REGISTRATION (stream::Id2DataPairTest);

namespace stream
{
    void Id2DataPairTest::setUp ( void )
    {
        std::vector<const Description*> descriptions;
        descriptions.push_back(new Description(0, DataVariant::NONE));
        descriptions.push_back(new Description(1, DataVariant::NONE));
        descriptions.push_back(new Description(2, DataVariant::NONE));
        m_map = new impl::Id2DataMap(descriptions);
        m_dataContainer = DataContainer(new None());
    }

    void Id2DataPairTest::testSet ( void )
    {
        Id2DataPair pair0(0);
        CPPUNIT_ASSERT_THROW(set(pair0, *m_map), InvalidState);
        
        Id2DataPair pair1(0, m_dataContainer);
        CPPUNIT_ASSERT_NO_THROW(set(pair1,*m_map));
        CPPUNIT_ASSERT_EQUAL(m_dataContainer, (*m_map)[0]);
        CPPUNIT_ASSERT(pair1.data().empty());
        CPPUNIT_ASSERT_THROW(set(pair1,*m_map), InvalidState);
        
        Id2DataPair pair3(10, m_dataContainer);
        CPPUNIT_ASSERT_THROW(set(pair3,*m_map), WrongId);
    }
    
    void Id2DataPairTest::testTryGet()
    {
        Id2DataPair pair0(0);
        CPPUNIT_ASSERT_NO_THROW(tryGet(pair0,*m_map));
        CPPUNIT_ASSERT_EQUAL(false, tryGet(pair0,*m_map));
        
        (*m_map)[0] = m_dataContainer;
        CPPUNIT_ASSERT_EQUAL(true, tryGet(pair0,*m_map));
        
        Id2DataPair pair3(10);
        CPPUNIT_ASSERT_THROW(tryGet(pair3,*m_map), WrongId);
    }
    
    void Id2DataPairTest::testTrySet()
    {
        Id2DataPair pair0(0, m_dataContainer);
        CPPUNIT_ASSERT_NO_THROW(trySet(pair0, *m_map));
        CPPUNIT_ASSERT_EQUAL(true, trySet(pair0, *m_map));
        
        (*m_map)[0] = m_dataContainer;
        CPPUNIT_ASSERT_EQUAL(false, trySet(pair0, *m_map));
        
        Id2DataPair pair3(10);
        CPPUNIT_ASSERT_THROW(trySet(pair3, *m_map), WrongId);
    }

    void Id2DataPairTest::testGet ( void )
    {
        (*m_map)[0] = m_dataContainer;
        Id2DataPair pair0(0);
        CPPUNIT_ASSERT_NO_THROW(get(pair0, *m_map));
        CPPUNIT_ASSERT_EQUAL(m_dataContainer, pair0.data());
        CPPUNIT_ASSERT((*m_map)[0].empty());
        
        CPPUNIT_ASSERT_THROW(get(pair0, *m_map), InvalidState);
        Id2DataPair pair3(10);
        CPPUNIT_ASSERT_THROW(get(pair3, *m_map), WrongId);
    }

    void Id2DataPairTest::tearDown ( void )
    {
        delete m_map;
    }
}