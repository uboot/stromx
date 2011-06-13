#include "Id2DataCompositeTest.h"

#include <stream/None.h>
#include <stream/Exception.h>
#include <stream/Id2DataPair.h>
#include <stream/DataContainer.h>
#include <stream/Id2DataMap.h>
#include <stream/Id2DataComposite.h>

#include <cppunit/TestAssert.h>

CPPUNIT_TEST_SUITE_REGISTRATION (stream::Id2DataCompositeTest);

namespace stream
{
    void Id2DataCompositeTest::setUp ( void )
    {
        std::vector<Description> descriptions;
        descriptions.push_back(Description(0, DataType()));
        descriptions.push_back(Description(1, DataType()));
        descriptions.push_back(Description(2, DataType()));
        m_map = new Id2DataMap(descriptions);
        m_dataContainer = new DataContainer(new None());
    }

    void Id2DataCompositeTest::testSet ( void )
    {
        {
            (*m_map)[0] = m_dataContainer;
            Id2DataPair pair0(0, m_dataContainer);
            Id2DataPair pair1(1, m_dataContainer);
            Id2DataPair pair2(2, m_dataContainer);
            
            CPPUNIT_ASSERT_THROW(set(pair0 && (pair1 || pair2), *m_map), InvalidStateException);
        }          
        
        {
            (*m_map)[0] = m_dataContainer;
            (*m_map)[1] = 0;
            (*m_map)[2] = 0;
            Id2DataPair pair0(0, m_dataContainer);
            Id2DataPair pair1(1, m_dataContainer);
            Id2DataPair pair2(2, m_dataContainer);
            
            CPPUNIT_ASSERT_NO_THROW(set(pair0 || (pair1 && pair2), *m_map));
            CPPUNIT_ASSERT_EQUAL(m_dataContainer, (*m_map)[0]);
            CPPUNIT_ASSERT_EQUAL(m_dataContainer, pair0.data());
            CPPUNIT_ASSERT_EQUAL(m_dataContainer, (*m_map)[1]);
            CPPUNIT_ASSERT_EQUAL((DataContainer*)(0), pair1.data());
            CPPUNIT_ASSERT_EQUAL(m_dataContainer, (*m_map)[2]);
            CPPUNIT_ASSERT_EQUAL((DataContainer*)(0), pair2.data());
        }         
        
        {
            (*m_map)[0] = 0;
            (*m_map)[1] = m_dataContainer;
            (*m_map)[2] = 0;
            Id2DataPair pair0(0, m_dataContainer);
            Id2DataPair pair1(1, m_dataContainer);
            Id2DataPair pair2(2, m_dataContainer);
            
            CPPUNIT_ASSERT_NO_THROW(set(pair0 || (pair1 && pair2), *m_map));
            CPPUNIT_ASSERT_EQUAL(m_dataContainer, (*m_map)[0]);
            CPPUNIT_ASSERT_EQUAL((DataContainer*)(0), pair0.data());
            CPPUNIT_ASSERT_EQUAL(m_dataContainer, (*m_map)[1]);
            CPPUNIT_ASSERT_EQUAL(m_dataContainer, pair1.data());
            CPPUNIT_ASSERT_EQUAL((DataContainer*)(0), (*m_map)[2]);
            CPPUNIT_ASSERT_EQUAL(m_dataContainer, pair2.data());
        }
    }
    
    void Id2DataCompositeTest::testTryGet()
    {
        Id2DataPair pair0(0);
        Id2DataPair pair1(1);
        Id2DataPair pair2(2);
        (*m_map)[1] = m_dataContainer;
        (*m_map)[2] = m_dataContainer;
        
        CPPUNIT_ASSERT_EQUAL(false, tryGet(pair0 && pair1 && pair2, *m_map));
        CPPUNIT_ASSERT_EQUAL(true, tryGet((pair0 && pair1) || pair2, *m_map));
        CPPUNIT_ASSERT_EQUAL(false, tryGet(pair0 && (pair1 || pair2), *m_map));
    }
    
    void Id2DataCompositeTest::testTrySet()
    {
        Id2DataPair pair0(0, m_dataContainer);
        Id2DataPair pair1(1, m_dataContainer);
        Id2DataPair pair2(2, m_dataContainer);
        (*m_map)[0] = m_dataContainer;
        
        CPPUNIT_ASSERT_EQUAL(false, trySet(pair0 && pair1 && pair2, *m_map));
        CPPUNIT_ASSERT_EQUAL(true, trySet((pair0 && pair1) || pair2, *m_map));
        CPPUNIT_ASSERT_EQUAL(false, trySet(pair0 && (pair1 || pair2), *m_map));
    }

    void Id2DataCompositeTest::testGet ( void )
    {
        {
            (*m_map)[1] = m_dataContainer;
            (*m_map)[2] = m_dataContainer;
            Id2DataPair pair0(0);
            Id2DataPair pair1(1);
            Id2DataPair pair2(2);   
            
            CPPUNIT_ASSERT_THROW(get(pair0 && (pair1 || pair2), *m_map), InvalidStateException);
        }
        
        {
            (*m_map)[0] = 0;
            (*m_map)[1] = m_dataContainer;
            (*m_map)[2] = m_dataContainer;
            Id2DataPair pair0(0);
            Id2DataPair pair1(1);
            Id2DataPair pair2(2);
            
            CPPUNIT_ASSERT_NO_THROW(get((pair0 && pair1) || pair2, *m_map));
            CPPUNIT_ASSERT_EQUAL((DataContainer*)(0), (*m_map)[0]);
            CPPUNIT_ASSERT_EQUAL((DataContainer*)(0), pair0.data());
            CPPUNIT_ASSERT_EQUAL(m_dataContainer, (*m_map)[1]);
            CPPUNIT_ASSERT_EQUAL((DataContainer*)(0), pair1.data());
            CPPUNIT_ASSERT_EQUAL((DataContainer*)(0), (*m_map)[2]);
            CPPUNIT_ASSERT_EQUAL(m_dataContainer, pair2.data());
        }
        
        {
            (*m_map)[0] = m_dataContainer;
            (*m_map)[1] = m_dataContainer;
            (*m_map)[2] = 0;
            Id2DataPair pair0(0);
            Id2DataPair pair1(1);
            Id2DataPair pair2(2);
            
            CPPUNIT_ASSERT_NO_THROW(get((pair0 && pair1) || pair2, *m_map));
            CPPUNIT_ASSERT_EQUAL((DataContainer*)(0), (*m_map)[0]);
            CPPUNIT_ASSERT_EQUAL(m_dataContainer, pair0.data());
            CPPUNIT_ASSERT_EQUAL((DataContainer*)(0), (*m_map)[1]);
            CPPUNIT_ASSERT_EQUAL(m_dataContainer, pair1.data());
            CPPUNIT_ASSERT_EQUAL((DataContainer*)(0), (*m_map)[2]);
            CPPUNIT_ASSERT_EQUAL((DataContainer*)(0), pair2.data());
        }
    }

    void Id2DataCompositeTest::tearDown ( void )
    {
        delete m_map;
        delete m_dataContainer;
    }
}