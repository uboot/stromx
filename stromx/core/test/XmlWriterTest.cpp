/* 
 *  Copyright 2011 Thomas Fidler
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
#include "TestOperator.h"
#include "XmlWriterTest.h"
#include <stromx/core/Operator.h>
#include <stromx/core/XmlWriter.h>
#include <stromx/core/Operator.h>
#include <stromx/core/Input.h>
#include <stromx/core/Thread.h>
#include <stromx/core/Stream.h>
#include <stromx/core/XmlWriter.h>

CPPUNIT_TEST_SUITE_REGISTRATION (stromx::core::XmlWriterTest);

namespace stromx
{
    namespace core
    {
        void XmlWriterTest::setUp()
        {
            m_stream = new Stream;
            m_stream->setName("TestStream");
            
            Operator* op0 = new Operator(new TestOperator);
            op0->setParameter(TestOperator::BUFFER_SIZE, UInt32(5000));
            op0->initialize();
            op0->setParameter(TestOperator::SLEEP_TIME, UInt32(200));
            op0->setName("Number 1");
            m_stream->addOperator(op0);
            
            Operator* op1 = new Operator(new TestOperator);
            op1->setParameter(TestOperator::BUFFER_SIZE, UInt32(6000));
            op1->initialize();
            op1->setParameter(TestOperator::SLEEP_TIME, UInt32(250));
            op1->setName("Number 2");
            m_stream->addOperator(op1);
            
            Operator* op2 = new Operator(new TestOperator);
            op2->setParameter(TestOperator::BUFFER_SIZE, UInt32(7000));
            op2->initialize();
            op2->setParameter(TestOperator::SLEEP_TIME, UInt32(300));
            op2->setName("Number 3");
            m_stream->addOperator(op2);
            
            m_stream->connect(Output(op0, TestOperator::OUTPUT_1), Input(op1, TestOperator::INPUT_1));
            m_stream->connect(Output(op0, TestOperator::OUTPUT_2), Input(op1, TestOperator::INPUT_2));
            
            m_stream->connect(Output(op1, TestOperator::OUTPUT_1), Input(op2, TestOperator::INPUT_1));
            m_stream->connect(Output(op1, TestOperator::OUTPUT_2), Input(op2, TestOperator::INPUT_2));
            
            Thread* thread = m_stream->addThread();
            thread->setName("Processing thread");
            
            thread->addNode(Input(op1, TestOperator::INPUT_1));
            thread->addNode(Input(op1, TestOperator::INPUT_2));
            thread->addNode(Input(op2, TestOperator::INPUT_1));
            thread->addNode(Input(op2, TestOperator::INPUT_2));
        }
        
        void XmlWriterTest::testWrite()
        {
            //Attempt to write to filesystem without neccessary permission
            CPPUNIT_ASSERT_THROW(XmlWriter().write("/root/test/XmlWriterTest_testWrite.xml", *m_stream), FileAccessFailed);
            //Attempt to write to filesystem where access is guaranteed (hopefully: it is written to the subdirectory where
            //the test was initiated
            XmlWriter().write("XmlWriterTest_testWrite.xml", *m_stream);
        }

        void XmlWriterTest::tearDown()
        {
            delete m_stream;
        }
    }
}
