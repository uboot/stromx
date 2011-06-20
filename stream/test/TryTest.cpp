#include "TryTest.h"

#include <stream/None.h>
#include <stream/Exception.h>
#include <stream/Id2DataPair.h>
#include <stream/Try.h>
#include <stream/DataContainer.h>
#include <stream/Id2DataMap.h>

#include <cppunit/TestAssert.h>

CPPUNIT_TEST_SUITE_REGISTRATION (stream::TryTest);

namespace stream
{
    void TryTest::setUp ( void )
    {
        std::vector<Description*> descriptions;
        descriptions.push_back(new Description(0, DataType::NONE));
        descriptions.push_back(new Description(1, DataType::NONE));
        descriptions.push_back(new Description(2, DataType::NONE));
        m_map = new Id2DataMap(descriptions);
        m_dataContainer = new DataContainer(new None());
    }

    void TryTest::testSet ( void )
    {
        Id2DataPair pair0(0, m_dataContainer);
        CPPUNIT_ASSERT_NO_THROW(set(Try(pair0), *m_map));
        CPPUNIT_ASSERT_EQUAL(m_dataContainer, (*m_map)[0]);
        CPPUNIT_ASSERT_EQUAL((DataContainer*)(0), pair0.data());
        
        Id2DataPair pair1(0, m_dataContainer);
        CPPUNIT_ASSERT_NO_THROW(set(Try(pair1), *m_map));
        CPPUNIT_ASSERT_EQUAL(m_dataContainer, pair1.data());  
    }
    
    void TryTest::testTryGet()
    {
        Id2DataPair pair0(0);
        CPPUNIT_ASSERT_EQUAL(true, tryGet(Try(pair0),*m_map));
        
        (*m_map)[0] = m_dataContainer;
        CPPUNIT_ASSERT_EQUAL(true, tryGet(Try(pair0),*m_map));
    }
    
    void TryTest::testTrySet()
    {
        Id2DataPair pair0(0, m_dataContainer);
        CPPUNIT_ASSERT_EQUAL(true, trySet(Try(pair0), *m_map));
        
        (*m_map)[0] = m_dataContainer;
        CPPUNIT_ASSERT_EQUAL(true, trySet(Try(pair0), *m_map));
    }

    void TryTest::testGet ( void )
    {
        (*m_map)[0] = m_dataContainer;
        Id2DataPair pair0(0);
        CPPUNIT_ASSERT_NO_THROW(get(Try(pair0), *m_map));
        CPPUNIT_ASSERT_EQUAL(m_dataContainer, pair0.data());
        CPPUNIT_ASSERT_EQUAL((DataContainer*)(0), (*m_map)[0]);
        
        Id2DataPair pair1(0);
        CPPUNIT_ASSERT_NO_THROW(get(Try(pair1), *m_map));
        CPPUNIT_ASSERT_EQUAL((DataContainer*)(0), pair1.data());
    }

    void TryTest::tearDown ( void )
    {
        delete m_map;
        delete m_dataContainer;
    }
}