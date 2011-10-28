#include "CounterTest.h"

#include <cppunit/TestAssert.h>

#include <stromx/core/OperatorTester.h>
#include <stromx/core/DataContainer.h>
#include <stromx/core/ReadAccess.h>


#include <stromx/base/Counter.h>

CPPUNIT_TEST_SUITE_REGISTRATION (stromx::base::CounterTest);

namespace stromx
{
    using namespace core;

    namespace base
    {
        void CounterTest::setUp()
        {
            m_operator = new OperatorTester(new Counter());
            m_operator->initialize();
            m_operator->activate();
        }

        void CounterTest::testExecute()
        {
            DataContainer data = m_operator->getOutputData(base::Counter::OUTPUT);
            CPPUNIT_ASSERT_EQUAL(UInt32(0), ReadAccess<UInt32>(data)());
            
            m_operator->clearOutputData(base::Counter::OUTPUT);
            
            data = m_operator->getOutputData(base::Counter::OUTPUT);
            CPPUNIT_ASSERT_EQUAL(UInt32(1), ReadAccess<UInt32>(data)());
            
            m_operator->deactivate();
            m_operator->activate();
            
            data = m_operator->getOutputData(base::Counter::OUTPUT);
            CPPUNIT_ASSERT_EQUAL(UInt32(0), ReadAccess<UInt32>(data)());
        }

        void CounterTest::tearDown()
        {
            delete m_operator;
        }
    }
}
