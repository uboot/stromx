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

#ifndef STROMX_CORE_SORTINPUTSALGORITHMTEST_H
#define STROMX_CORE_SORTINPUTSALGORITHMTEST_H

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>

namespace stromx
{
    namespace core
    {
        class Operator;
        class Stream;
        
        class SortInputsAlgorithmTest : public CPPUNIT_NS :: TestFixture
        {
            CPPUNIT_TEST_SUITE (SortInputsAlgorithmTest);
            CPPUNIT_TEST (testApply);
            CPPUNIT_TEST_SUITE_END ();

            public:
                SortInputsAlgorithmTest() : m_stream(0), m_op1(0), m_op2(0) {}
                
                void setUp();
                void tearDown();

            protected:
                void testApply();
                
            private:
                Stream* m_stream;
                Operator* m_op1;
                Operator* m_op2;
            
        };
    }
}

#endif // STROMX_CORE_SORTINPUTSALGORITHMTEST_H