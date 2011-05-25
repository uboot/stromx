#include "Id2DataPairTest.h"

#include "None.h"
#include "Exception.h"
#include "Id2DataPair.h"
#include "DataContainer.h"
#include "Id2DataMap.h"

#include <cppunit/TestAssert.h>

CPPUNIT_TEST_SUITE_REGISTRATION (stream::Id2DataPairTest);

namespace stream
{
    void Id2DataPairTest::setUp ( void )
    {
        std::vector<Description> descriptions;
        descriptions.push_back(Description("Data 0", 0, DataType()));
        descriptions.push_back(Description("Data 1", 1, DataType()));
        descriptions.push_back(Description("Data 2", 2, DataType()));
        m_map = new Id2DataMap(descriptions);
        m_dataContainer = new DataContainer(new None());
    }

    void Id2DataPairTest::setTest ( void )
    {
        Id2DataPair pair0(0, 0);
        CPPUNIT_ASSERT_THROW(set(pair0, *m_map), InvalidStateException);
        
        Id2DataPair pair1(0, m_dataContainer);
        CPPUNIT_ASSERT_NO_THROW(set(pair1,*m_map));
        CPPUNIT_ASSERT_EQUAL(m_dataContainer, (*m_map)[0]);
        CPPUNIT_ASSERT_EQUAL((DataContainer*)(0), pair1.data());
        CPPUNIT_ASSERT_THROW(set(pair1,*m_map), InvalidStateException);
        
        Id2DataPair pair3(10, m_dataContainer);
        CPPUNIT_ASSERT_THROW(set(pair3,*m_map), WrongIdException);
    }
    
    void Id2DataPairTest::tryGetTest()
    {
        Id2DataPair pair0(0);
        CPPUNIT_ASSERT_NO_THROW(tryGet(pair0,*m_map));
        CPPUNIT_ASSERT_EQUAL(false, tryGet(pair0,*m_map));
        
        (*m_map)[0] = m_dataContainer;
        CPPUNIT_ASSERT_EQUAL(true, tryGet(pair0,*m_map));
        
        Id2DataPair pair3(10);
        CPPUNIT_ASSERT_THROW(tryGet(pair3,*m_map), WrongIdException);
    }
    
    void Id2DataPairTest::trySetTest()
    {
        Id2DataPair pair0(0, m_dataContainer);
        CPPUNIT_ASSERT_NO_THROW(trySet(pair0, *m_map));
        CPPUNIT_ASSERT_EQUAL(true, trySet(pair0, *m_map));
        
        (*m_map)[0] = m_dataContainer;
        CPPUNIT_ASSERT_EQUAL(false, trySet(pair0, *m_map));
        
        Id2DataPair pair3(10, 0);
        CPPUNIT_ASSERT_THROW(trySet(pair3, *m_map), WrongIdException);
    }

    void Id2DataPairTest::getTest ( void )
    {
        (*m_map)[0] = m_dataContainer;
        Id2DataPair pair0(0);
        CPPUNIT_ASSERT_NO_THROW(get(pair0, *m_map));
        CPPUNIT_ASSERT_EQUAL(m_dataContainer, pair0.data());
        CPPUNIT_ASSERT_EQUAL((DataContainer*)(0), (*m_map)[0]);
        
        CPPUNIT_ASSERT_THROW(get(pair0, *m_map), InvalidStateException);
        Id2DataPair pair3(10);
        CPPUNIT_ASSERT_THROW(get(pair3, *m_map), WrongIdException);
    }

    void Id2DataPairTest::tearDown ( void )
    {
        delete m_map;
        delete m_dataContainer;
    }
}