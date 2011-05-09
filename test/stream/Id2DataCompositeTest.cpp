#include "Id2DataCompositeTest.h"

#include <stream/None.h>
#include <stream/Exception.h>
#include <stream/Id2DataComposite.h>
#include <stream/Id2DataPair.h>
#include <stream/DataContainer.h>

#include <cppunit/TestAssert.h>

CPPUNIT_TEST_SUITE_REGISTRATION (stream::Id2DataCompositeTest);

namespace stream
{
    void Id2DataCompositeTest::setUp ( void )
    {
        std::vector<Descriptor> descriptors;
        descriptors.push_back(Descriptor("Data 0", 0, DataType()));
        descriptors.push_back(Descriptor("Data 1", 1, DataType()));
        descriptors.push_back(Descriptor("Data 2", 2, DataType()));
        m_map = new Id2DataMap(descriptors);
        m_dataContainer = new DataContainer(new None());
    }

    void Id2DataCompositeTest::setTest ( void )
    {
        Id2DataPair pair0 = Id2DataPair(0, m_dataContainer);
        Id2DataPair pair1 = Id2DataPair(1, m_dataContainer);
        Id2DataPair pair2 = Id2DataPair(2, m_dataContainer);
        Id2DataComposite mapper = (pair0 || pair1);
        mapper && pair2;
    }
    
    void Id2DataCompositeTest::tryGetTest()
    {
    }
    
    void Id2DataCompositeTest::trySetTest()
    {
    }

    void Id2DataCompositeTest::getTest ( void )
    {
    }

    void Id2DataCompositeTest::tearDown ( void )
    {
        delete m_map;
        m_dataContainer->dereference();
    }
}