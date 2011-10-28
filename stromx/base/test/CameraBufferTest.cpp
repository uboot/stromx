#include "CameraBufferTest.h"

#include <cppunit/TestAssert.h>


#include <stromx/base/impl/CameraBuffer.h>

#include <stromx/core/ReadAccess.h>
#include <stromx/core/OperatorTester.h>

CPPUNIT_TEST_SUITE_REGISTRATION (stromx::base::CameraBufferTest);

namespace stromx
{
    using namespace core;

    namespace base
    {
        using namespace impl;
        
        void CameraBufferTest::setUp()
        {
            m_operator = new core::OperatorTester(new CameraBuffer());
            m_operator->initialize();
            m_operator->setParameter(CameraBuffer::NUM_BUFFERS, UInt32(1));
            m_operator->activate();
        }

        void CameraBufferTest::testExecute()
        {
            DataContainer input1(new UInt32(0));
            DataContainer input2(new UInt32(1));
            
            DataContainer output1;
            DataContainer output2;
            
            DataContainer outputBuffer1;
            DataContainer outputBuffer2;
            
            DataContainer outputIndex1;
            DataContainer outputIndex2;
            
            // set the input data
            m_operator->setInputData(CameraBuffer::INPUT, input1);
            
            // get the output data
            output1 = m_operator->getOutputData(CameraBuffer::OUTPUT);
            outputBuffer1 = m_operator->getOutputData(CameraBuffer::BUFFER);
            outputIndex1 = m_operator->getOutputData(CameraBuffer::INDEX);
            
            CPPUNIT_ASSERT_EQUAL(UInt32(0), ReadAccess<UInt32>(output1)());
            CPPUNIT_ASSERT_EQUAL(UInt32(0), ReadAccess<UInt32>(outputIndex1)());
            
            // clear all outpout
            m_operator->clearOutputData(CameraBuffer::OUTPUT);
            m_operator->clearOutputData(CameraBuffer::BUFFER);
            m_operator->clearOutputData(CameraBuffer::INDEX);
            
            // set the input (no buffer available)
            m_operator->setInputData(CameraBuffer::INPUT, input2);
            
            // set the input
            m_operator->setInputData(CameraBuffer::INPUT, input2);
            
            // release the buffer
            outputBuffer1 = DataContainer();
            
            // get the output
            output2 = m_operator->getOutputData(CameraBuffer::OUTPUT);
            outputBuffer2 = m_operator->getOutputData(CameraBuffer::BUFFER);
            outputIndex2 = m_operator->getOutputData(CameraBuffer::INDEX);
            
            CPPUNIT_ASSERT_EQUAL(UInt32(1), ReadAccess<UInt32>(output2)());
            CPPUNIT_ASSERT_EQUAL(UInt32(2), ReadAccess<UInt32>(outputIndex2)());
        }

        void CameraBufferTest::tearDown()
        {
            delete m_operator;
        }
    }
}
