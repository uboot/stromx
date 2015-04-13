/* 
 *  Copyright 2015 Matthias Fuchs
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

#include "AssignThreadsAlgorithmTest.h"

#include <cppunit/TestAssert.h>
#include "stromx/runtime/AssignThreadsAlgorithm.h"
#include "stromx/runtime/Counter.h"
#include "stromx/runtime/Dump.h"
#include "stromx/runtime/Fork.h"
#include "stromx/runtime/Iterate.h"
#include "stromx/runtime/Join.h"
#include "stromx/runtime/Merge.h"
#include "stromx/runtime/PeriodicDelay.h"
#include "stromx/runtime/Stream.h"
#include "stromx/runtime/Thread.h"

CPPUNIT_TEST_SUITE_REGISTRATION (stromx::runtime::AssignThreadsAlgorithmTest);

namespace stromx
{
    namespace runtime
    {
        void AssignThreadsAlgorithmTest::setUp()
        {
            m_stream = 0;
        }

        void AssignThreadsAlgorithmTest::tearDown()
        {
            delete m_stream;
        }
        
        void AssignThreadsAlgorithmTest::testApplyForkJoin()
        {
            m_stream = new Stream;
            
            Operator* counter = m_stream->addOperator(new Counter);
            Operator* fork = m_stream->addOperator(new Fork);
            Operator* join = m_stream->addOperator(new Join);
            Operator* dump = m_stream->addOperator(new Dump);
            
            m_stream->initializeOperator(counter);
            m_stream->initializeOperator(fork);
            m_stream->initializeOperator(join);
            m_stream->initializeOperator(dump);
            
            m_stream->connect(counter, Counter::OUTPUT, fork, Fork::INPUT);
            m_stream->connect(fork, 0, join, 0);
            m_stream->connect(fork, 1, join, 1);
            m_stream->connect(join, Join::OUTPUT, dump, Dump::INPUT);
            
            AssignThreadsAlgorithm algorithm;
            algorithm.apply(*m_stream);
            
            CPPUNIT_ASSERT_EQUAL(std::size_t(4), m_stream->threads().size());
            
            Thread* thread0 = m_stream->threads()[0];
            Thread* thread1 = m_stream->threads()[1];
            Thread* thread2 = m_stream->threads()[2];
            Thread* thread3 = m_stream->threads()[3];
            
            CPPUNIT_ASSERT_EQUAL(std::size_t(1), thread0->inputSequence().size());
            CPPUNIT_ASSERT_EQUAL(std::size_t(1), thread1->inputSequence().size());
            CPPUNIT_ASSERT_EQUAL(std::size_t(1), thread2->inputSequence().size());
            CPPUNIT_ASSERT_EQUAL(std::size_t(1), thread3->inputSequence().size());
        }
        
        void AssignThreadsAlgorithmTest::testApplyIterateMerge()
        {
            m_stream = new Stream;
            
            Operator* counter = m_stream->addOperator(new Counter);
            Operator* iterate = m_stream->addOperator(new Iterate);
            Operator* merge = m_stream->addOperator(new Merge);
            Operator* dump = m_stream->addOperator(new Dump);
            
            m_stream->initializeOperator(counter);
            m_stream->initializeOperator(iterate);
            m_stream->initializeOperator(merge);
            m_stream->initializeOperator(dump);
            
            m_stream->connect(counter, Counter::OUTPUT,
                              iterate, Iterate::INPUT);
            m_stream->connect(iterate, Iterate::OUTPUT_DATA,
                              merge, Merge::INPUT_DATA);
            m_stream->connect(iterate, Iterate::OUTPUT_NUM_ITEMS,
                              merge, Merge::INPUT_NUM_ITEMS);
            m_stream->connect(merge, Merge::OUTPUT, dump, Dump::INPUT);
            
            AssignThreadsAlgorithm algorithm;
            algorithm.apply(*m_stream);
            
            CPPUNIT_ASSERT_EQUAL(std::size_t(2), m_stream->threads().size());
            
            Thread* thread0 = m_stream->threads()[0];
            Thread* thread1 = m_stream->threads()[1];
            
            CPPUNIT_ASSERT_EQUAL(std::size_t(3), thread0->inputSequence().size());
            CPPUNIT_ASSERT_EQUAL(std::size_t(1), thread1->inputSequence().size());
            
            CPPUNIT_ASSERT_EQUAL(static_cast<const Operator*>(iterate),
                                 thread0->inputSequence()[0].op());
            CPPUNIT_ASSERT_EQUAL((unsigned int)(Iterate::INPUT),
                                 thread0->inputSequence()[0].id());
            
            CPPUNIT_ASSERT_EQUAL(static_cast<const Operator*>(merge),
                                 thread0->inputSequence()[1].op());
            CPPUNIT_ASSERT_EQUAL((unsigned int)(Merge::INPUT_NUM_ITEMS),
                                 thread0->inputSequence()[1].id());
            
            CPPUNIT_ASSERT_EQUAL(static_cast<const Operator*>(dump),
                                 thread0->inputSequence()[2].op());
            CPPUNIT_ASSERT_EQUAL((unsigned int)(Dump::INPUT),
                                 thread0->inputSequence()[2].id());
            
            CPPUNIT_ASSERT_EQUAL(static_cast<const Operator*>(merge),
                                 thread1->inputSequence()[0].op());
            CPPUNIT_ASSERT_EQUAL((unsigned int)(Merge::INPUT_DATA),
                                 thread1->inputSequence()[0].id());
        }
        
        void AssignThreadsAlgorithmTest::testApplyCounterDump()
        {
            m_stream = new Stream;
            
            Operator* counter = m_stream->addOperator(new Counter);
            Operator* dump = m_stream->addOperator(new Dump);
            
            m_stream->initializeOperator(counter);
            m_stream->initializeOperator(dump);
            
            m_stream->connect(counter, Counter::OUTPUT,
                              dump, Dump::INPUT);
            
            AssignThreadsAlgorithm algorithm;
            algorithm.apply(*m_stream);
            
            CPPUNIT_ASSERT_EQUAL(std::size_t(1), m_stream->threads().size());
            
            Thread* thread = m_stream->threads()[0];
            
            CPPUNIT_ASSERT_EQUAL(std::size_t(1), thread->inputSequence().size());
            
            CPPUNIT_ASSERT_EQUAL(static_cast<const Operator*>(dump),
                                 thread->inputSequence()[0].op());
            CPPUNIT_ASSERT_EQUAL((unsigned int)(Dump::INPUT),
                                 thread->inputSequence()[0].id());
        }
        
        void AssignThreadsAlgorithmTest::testApplyInverseOrder()
        {
            m_stream = new Stream;
            
            Operator* counter = m_stream->addOperator(new Counter);
            Operator* delay = m_stream->addOperator(new PeriodicDelay);
            Operator* dump = m_stream->addOperator(new Dump);
            
            m_stream->initializeOperator(dump);
            m_stream->initializeOperator(delay);
            m_stream->initializeOperator(counter);
            
            m_stream->connect(counter, Counter::OUTPUT, delay, PeriodicDelay::INPUT);
            m_stream->connect(delay, PeriodicDelay::OUTPUT, dump, Dump::INPUT);
            
            AssignThreadsAlgorithm algorithm;
            algorithm.apply(*m_stream);
            
            CPPUNIT_ASSERT_EQUAL(std::size_t(1), m_stream->threads().size());
            
            Thread* thread = m_stream->threads()[0];
            
            CPPUNIT_ASSERT_EQUAL(std::size_t(2), thread->inputSequence().size());
            
            CPPUNIT_ASSERT_EQUAL(static_cast<const Operator*>(delay),
                                 thread->inputSequence()[0].op());
            CPPUNIT_ASSERT_EQUAL((unsigned int)(PeriodicDelay::INPUT),
                                 thread->inputSequence()[0].id());
            CPPUNIT_ASSERT_EQUAL(static_cast<const Operator*>(dump),
                                 thread->inputSequence()[1].op());
            CPPUNIT_ASSERT_EQUAL((unsigned int)(Dump::INPUT),
                                 thread->inputSequence()[1].id());
        }
        
        void AssignThreadsAlgorithmTest::testApplyTwoComponents()
        {
            m_stream = new Stream;
            
            Operator* counter0 = m_stream->addOperator(new Counter);
            Operator* dump0 = m_stream->addOperator(new Dump);
            
            m_stream->initializeOperator(counter0);
            m_stream->initializeOperator(dump0);
            
            m_stream->connect(counter0, Counter::OUTPUT,
                              dump0, Dump::INPUT);
            
            Operator* counter1 = m_stream->addOperator(new Counter);
            Operator* dump1 = m_stream->addOperator(new Dump);
            
            m_stream->initializeOperator(counter1);
            m_stream->initializeOperator(dump1);
            
            m_stream->connect(counter1, Counter::OUTPUT,
                              dump1, Dump::INPUT);
            
            AssignThreadsAlgorithm algorithm;
            algorithm.apply(*m_stream);
            
            CPPUNIT_ASSERT_EQUAL(std::size_t(2), m_stream->threads().size());
            
            Thread* thread0 = m_stream->threads()[0];
            Thread* thread1 = m_stream->threads()[1];
            
            CPPUNIT_ASSERT_EQUAL(std::size_t(1), thread0->inputSequence().size());
            CPPUNIT_ASSERT_EQUAL(std::size_t(1), thread1->inputSequence().size());
            
            CPPUNIT_ASSERT_EQUAL(static_cast<const Operator*>(dump0),
                                 thread0->inputSequence()[0].op());
            CPPUNIT_ASSERT_EQUAL((unsigned int)(Dump::INPUT),
                                 thread0->inputSequence()[0].id());
            
            CPPUNIT_ASSERT_EQUAL(static_cast<const Operator*>(dump1),
                                 thread1->inputSequence()[0].op());
            CPPUNIT_ASSERT_EQUAL((unsigned int)(Dump::INPUT),
                                 thread1->inputSequence()[0].id());
        }
    }
}
