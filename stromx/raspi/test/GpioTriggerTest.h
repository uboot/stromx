/* 
*  Copyright 2014 Matthias Fuchs
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

#ifndef STROMX_RASPI_GPIOTRIGGERTEST_H
#define STROMX_RASPI_GPIOTRIGGERTEST_H

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>
#include <boost/graph/graph_concepts.hpp>

#include "stromx/runtime/OperatorTester.h"

namespace stromx
{
    namespace raspi
    {
        class GpioTriggerTest : public CPPUNIT_NS :: TestFixture
        {
            CPPUNIT_TEST_SUITE (GpioTriggerTest);
            CPPUNIT_TEST(testInterrupt);
            CPPUNIT_TEST_SUITE_END ();

        public:
            GpioTriggerTest() : m_operator(0) {}
            
            void setUp();
            void tearDown();

        protected:
            void testInterrupt();
                
        private:
            void interruptExecution();
            
            runtime::OperatorTester* m_operator;
        };
    }
}

#endif // STROMX_RASPI_GPIOTRIGGERTEST_H