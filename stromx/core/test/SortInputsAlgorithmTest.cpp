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

#include "SortInputsAlgorithmTest.h"

#include <cppunit/TestAssert.h>
#include "../SortInputsAlgorithm.h"
#include "../Stream.h"
#include "../Thread.h"
#include "TestOperator.h"
#include "TestUtilities.h"

CPPUNIT_TEST_SUITE_REGISTRATION (stromx::core::SortInputsAlgorithmTest);

namespace stromx
{
    namespace core
    {
        void SortInputsAlgorithmTest::setUp()
        {
            // create the test stream
            m_stream = TestUtilities::buildTestStream();
            
            // remove the thread
            m_stream->removeThread(*m_stream->threads().begin());
            
            m_op1 = m_stream->operators()[1];
            m_op2 = m_stream->operators()[2];
        }

        void SortInputsAlgorithmTest::tearDown()
        {
            delete m_stream;
        }
        
        void SortInputsAlgorithmTest::testApplyTwoThreads()
        {
            // add two new threads
            Thread* thread1 = m_stream->addThread();
            Thread* thread2 = m_stream->addThread();
            
            // add inputs in the wrong order
            thread1->addInput(m_op2, TestOperator::INPUT_1);
            thread1->addInput(m_op1, TestOperator::INPUT_1);
            thread2->addInput(m_op2, TestOperator::INPUT_2);
            thread2->addInput(m_op1, TestOperator::INPUT_2);
            
            SortInputsAlgorithm sort;
            CPPUNIT_ASSERT_NO_THROW(sort.apply(*m_stream));
            
            CPPUNIT_ASSERT_EQUAL(static_cast<const Operator*>(m_op1), thread1->inputSequence()[0].op());
            CPPUNIT_ASSERT_EQUAL(static_cast<const Operator*>(m_op2), thread1->inputSequence()[1].op());
            CPPUNIT_ASSERT_EQUAL(static_cast<const Operator*>(m_op1), thread2->inputSequence()[0].op());
            CPPUNIT_ASSERT_EQUAL(static_cast<const Operator*>(m_op2), thread2->inputSequence()[1].op());
        }
        
        void SortInputsAlgorithmTest::testApplyOneThread()
        {
            // add two new threads
            Thread* thread = m_stream->addThread();
            
            // add inputs in the wrong order
            thread->addInput(m_op2, TestOperator::INPUT_1);
            thread->addInput(m_op1, TestOperator::INPUT_1);
            thread->addInput(m_op2, TestOperator::INPUT_2);
            thread->addInput(m_op1, TestOperator::INPUT_2);
            
            SortInputsAlgorithm sort;
            CPPUNIT_ASSERT_NO_THROW(sort.apply(*m_stream));
            
            CPPUNIT_ASSERT_EQUAL(static_cast<const Operator*>(m_op1), thread->inputSequence()[0].op());
            CPPUNIT_ASSERT_EQUAL(static_cast<const Operator*>(m_op1), thread->inputSequence()[1].op());
            CPPUNIT_ASSERT_EQUAL(static_cast<const Operator*>(m_op2), thread->inputSequence()[2].op());
            CPPUNIT_ASSERT_EQUAL(static_cast<const Operator*>(m_op2), thread->inputSequence()[3].op());
        }
    }
}
