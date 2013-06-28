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
#include <stromx/runtime/Image.h>
#include <stromx/runtime/OperatorTester.h>
#include <stromx/runtime/OperatorException.h>
#include <stromx/runtime/ReadAccess.h>
#include "stromx/cvimgutil/Buffer.h"
#include "stromx/cvimgutil/test/BufferTest.h"

CPPUNIT_TEST_SUITE_REGISTRATION (stromx::cvimgutil::BufferTest);

namespace stromx
{
    using namespace runtime;

    namespace cvimgutil
    {
        void BufferTest::setUp()
        {
            m_operator = new runtime::OperatorTester(new Buffer());
            m_operator->initialize();
            m_operator->setParameter(Buffer::NUM_BUFFERS, UInt32(1));
            m_operator->activate();
        }

        void BufferTest::testExecute()
        {
            DataContainer output;
            
            // get the output data
            output = m_operator->getOutputData(Buffer::OUTPUT);
            
            {
                // assert that the data is image data
                ReadAccess<runtime::Image> access(output);
                CPPUNIT_ASSERT_NO_THROW(access());
            }
            
            // clear all outpout
            m_operator->clearOutputData(Buffer::OUTPUT);
            
            // release the buffer
            output = DataContainer();
            
            // get the output data
            output = m_operator->getOutputData(Buffer::OUTPUT);
            
            {
                // assert that the data is image data
                ReadAccess<runtime::Image> access(output);
                CPPUNIT_ASSERT_NO_THROW(access());
            }
        }
        
        void BufferTest::testSetNoBuffers()
        {
            m_operator->deactivate();
            CPPUNIT_ASSERT_THROW(m_operator->setParameter(Buffer::NUM_BUFFERS, UInt32(0)), WrongParameterValue);
        }

        void BufferTest::tearDown()
        {
            delete m_operator;
        }
    }
}
