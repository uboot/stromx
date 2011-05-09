#include "Id2DataPairTest.h"

#include <stream/None.h>
#include <stream/Exception.h>
#include <stream/Id2DataPair.h>
#include <stream/DataContainer.h>

#include <cppunit/TestAssert.h>

CPPUNIT_TEST_SUITE_REGISTRATION (stream::Id2DataPairTest);

namespace stream
{
    void Id2DataPairTest::setUp ( void )
    {
        std::vector<Descriptor> descriptors;
        descriptors.push_back(Descriptor("Data", 0, DataType()));
        m_map = new Id2DataMap(descriptors);
        m_dataContainer = new DataContainer(new None());
    }

    void Id2DataPairTest::setTest ( void )
    {
        Id2DataPair pair1(0, 0);
        CPPUNIT_ASSERT_THROW(pair1.set(*m_map), InvalidStateException);
        
        Id2DataPair pair2(0, m_dataContainer);
        CPPUNIT_ASSERT_NO_THROW(pair2.set(*m_map));
        CPPUNIT_ASSERT_EQUAL(m_dataContainer, (*m_map)[0]);
        CPPUNIT_ASSERT_THROW(pair2.set(*m_map), InvalidStateException);
        
        Id2DataPair pair3(1, m_dataContainer);
        CPPUNIT_ASSERT_THROW(pair3.set(*m_map), WrongIdException);
    }
    
    void Id2DataPairTest::tryGetTest()
    {
        Id2DataPair pair1(0);
        CPPUNIT_ASSERT_NO_THROW(pair1.tryGet(*m_map));
        CPPUNIT_ASSERT_EQUAL(false, pair1.tryGet(*m_map));
        
        (*m_map)[0] = m_dataContainer;
        CPPUNIT_ASSERT_EQUAL(true, pair1.tryGet(*m_map));
        
        Id2DataPair pair2(1);
        CPPUNIT_ASSERT_THROW(pair2.tryGet(*m_map), WrongIdException);
    }
    
    void Id2DataPairTest::trySetTest()
    {
        Id2DataPair pair1(0, 0);
        CPPUNIT_ASSERT_NO_THROW(pair1.trySet(*m_map));
        CPPUNIT_ASSERT_EQUAL(true, pair1.trySet(*m_map));
        
        (*m_map)[0] = m_dataContainer;
        CPPUNIT_ASSERT_EQUAL(false, pair1.trySet(*m_map));
        
        Id2DataPair pair2(1, 0);
        CPPUNIT_ASSERT_THROW(pair2.trySet(*m_map), WrongIdException);
    }

    void Id2DataPairTest::getTest ( void )
    {
        (*m_map)[0] = m_dataContainer;
        
        Id2DataPair pair1(0);
        CPPUNIT_ASSERT_NO_THROW(pair1.get(*m_map));
        CPPUNIT_ASSERT_EQUAL(m_dataContainer, pair1.data());
        
        CPPUNIT_ASSERT_THROW(pair1.get(*m_map), InvalidStateException);
        
        Id2DataPair pair2(1);
        CPPUNIT_ASSERT_THROW(pair2.get(*m_map), WrongIdException);
    }

    void Id2DataPairTest::tearDown ( void )
    {
        delete m_map;
        m_dataContainer->dereference();
    }
}