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
#include "CoreTest.h"
#include "../Core.h"
#include "../Data.h"
#include "../Factory.h"
#include "../Operator.h"

CPPUNIT_TEST_SUITE_REGISTRATION (stromx::core::CoreTest);

namespace stromx
{
    namespace core
    {
        void CoreTest::setUp ( void )
        {
            m_factory = new core::Factory;
        }
        
        void CoreTest::testRegisterCore()
        {
            CPPUNIT_ASSERT_NO_THROW(stromxRegisterCore(*m_factory));
            
            core::Data* data = 0;
            CPPUNIT_ASSERT_NO_THROW(data = m_factory->newData("Core", "UInt32"));
            CPPUNIT_ASSERT(data);
            delete data;
        }
        
        void CoreTest::tearDown ( void )
        {
            delete m_factory;
        }
    }
}
