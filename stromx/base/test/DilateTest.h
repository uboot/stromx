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

#ifndef STROMX_BASE_DILATETEST_H
#define STROMX_BASE_DILATETEST_H

#include "MorphologicalFilterTest.h"

namespace stromx
{
    namespace core
    {
        class OperatorTester;
    }

    namespace base
    {
        class DilateTest : public MorphologicalFilterTest
        {
            CPPUNIT_TEST_SUITE (DilateTest);
            CPPUNIT_TEST (testExecuteMono);
            CPPUNIT_TEST (testExecuteColor);
            CPPUNIT_TEST_SUITE_END ();

            protected:
                core::OperatorKernel*const allocateOperator() const;
                const std::string getTestName() const { return "DilateTest"; }
        };
    }
}

#endif // STROMX_BASE_DILATETEST_H