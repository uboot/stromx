/* 
*  Copyright 2012 Matthias Fuchs
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


#ifndef STROMX_EXAMPLE_MORPHOLOGICALFILTERTEST_H
#define STROMX_EXAMPLE_MORPHOLOGICALFILTERTEST_H

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>

namespace stromx
{
    namespace runtime
    {
        class OperatorKernel;
        class OperatorTester;
    }

    namespace example
    {
        class MorphologicalFilterTest : public CPPUNIT_NS :: TestFixture
        {
            public:
                MorphologicalFilterTest() : m_operator(0) {}
                
                void setUp();
                void tearDown();

            protected:
                void testExecuteMono();
                void testExecuteColor();
                
                virtual runtime::OperatorKernel* allocateOperator() const = 0;
                virtual const std::string getTestName() const = 0;
                
            private:
                runtime::OperatorKernel* m_template;
                runtime::OperatorTester* m_operator;
        };
    }
}

#endif // STROMX_EXAMPLE_MORPHOLOGICALFILTERTEST_H