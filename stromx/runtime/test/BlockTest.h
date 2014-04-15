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

#ifndef STROMX_RUNTIME_BLOCKTEST_H
#define STROMX_RUNTIME_BLOCKTEST_H

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>
#include "stromx/runtime/DataContainer.h"

namespace stromx
{
    namespace runtime
    {
        class OperatorTester;
    }

    namespace runtime
    {
        class BlockTest : public CPPUNIT_NS :: TestFixture
        {
            CPPUNIT_TEST_SUITE (BlockTest);
            CPPUNIT_TEST (testExecuteActive);
            CPPUNIT_TEST (testExecuteSwitchToInactive);
            CPPUNIT_TEST (testExecuteInactive);
            CPPUNIT_TEST_SUITE_END ();

            public:
                BlockTest() : m_operator(0) {}
                
                void setUp();
                void tearDown();

            protected:
                void testExecuteActive();
                void testExecuteSwitchToInactive();
                void testExecuteInactive();
                
            private:
                void triggerDelayed ();
                void getOutputData();
                void getOutputDataInterrupted();
        
                runtime::OperatorTester* m_operator;
                runtime::DataContainer m_data;
        };
    }
}

#endif // STROMX_RUNTIME_BLOCKTEST_H