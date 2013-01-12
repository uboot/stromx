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
#include "CameraBufferTest.h"
#include "../impl/CameraBuffer.h"
#include <stromx/runtime/OperatorTester.h>
#include <stromx/runtime/ReadAccess.h>

CPPUNIT_TEST_SUITE_REGISTRATION (stromx::base::CameraBufferTest);

namespace stromx
{
    using namespace runtime;

    namespace base
    {
        using namespace impl;
        
        void CameraBufferTest::setUp()
        {
            m_operator = new runtime::OperatorTester(new CameraBuffer());
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
