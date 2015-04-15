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

#ifndef STROMX_RUNTIME_ASSIGNTHREADSALGORITHMTEST_H
#define STROMX_RUNTIME_ASSIGNTHREADSALGORITHMTEST_H

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>

namespace stromx
{
    namespace runtime
    {
        class Operator;
        class Stream;
        
        class AssignThreadsAlgorithmTest : public CPPUNIT_NS :: TestFixture
        {
            CPPUNIT_TEST_SUITE (AssignThreadsAlgorithmTest);
            CPPUNIT_TEST (testApplyCounterDump);
            CPPUNIT_TEST (testApplyIterateMerge);
            CPPUNIT_TEST (testApplyForkJoin);
            CPPUNIT_TEST (testApplyInverseOrder);
            CPPUNIT_TEST (testApplyWrongOrder);
            CPPUNIT_TEST (testApplyTwoComponents);
            CPPUNIT_TEST (testApplyParallel);
            CPPUNIT_TEST (testTwoOutputsAtOneInput);
            CPPUNIT_TEST_SUITE_END ();

            public:
                AssignThreadsAlgorithmTest() : m_stream(0) {}
                
                void setUp();
                void tearDown();

            protected:
                void testApplyIterateMerge();
                void testApplyCounterDump();
                void testApplyForkJoin();
                void testApplyInverseOrder();
                void testApplyWrongOrder();
                void testApplyTwoComponents();
                void testApplyParallel();
                void testTwoOutputsAtOneInput();
                
            private:
                Stream* m_stream;
            
        };
    }
}

#endif // STROMX_RUNTIME_ASSIGNTHREADSALGORITHMTEST_H