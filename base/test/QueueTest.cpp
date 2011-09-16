#include "QueueTest.h"

#include <base/Queue.h>
#include <strom/DataContainer.h>
#include <strom/Primitive.h>
#include <strom/ReadAccess.h>

#include <strom/OperatorTester.h>

#include <cppunit/TestAssert.h>

CPPUNIT_TEST_SUITE_REGISTRATION (base::QueueTest);

using namespace strom;

namespace base
{
    void QueueTest::setUp ( void )
    {
        m_operator = new strom::OperatorTester(new Queue());
        m_operator->initialize();
        m_operator->setParameter(Queue::SIZE, UInt32(3));
        m_operator->activate();
    }
    
    void QueueTest::testExecute()
    {
        DataContainer input1(new UInt8);
        DataContainer input2(new UInt8);
        DataContainer input3(new UInt8);
        
        m_operator->setInputData(Queue::INPUT, input1);
        m_operator->setInputData(Queue::INPUT, input2);
        m_operator->setInputData(Queue::INPUT, input3);
        
        DataContainer output1 = m_operator->getOutputData(Queue::OUTPUT);
        DataContainer output2 = m_operator->getOutputData(Queue::OUTPUT);
        DataContainer output3 = m_operator->getOutputData(Queue::OUTPUT);
        
        CPPUNIT_ASSERT_EQUAL(&ReadAccess<>(input1)(), &ReadAccess<>(output1)());
    }    

    void QueueTest::tearDown ( void )
    {
        delete m_operator;
    }
}