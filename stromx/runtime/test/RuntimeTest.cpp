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
#include "stromx/runtime/Data.h"
#include "stromx/runtime/Factory.h"
#include "stromx/runtime/Operator.h"
#include "stromx/runtime/Runtime.h"
#include "stromx/runtime/test/RuntimeTest.h"

CPPUNIT_TEST_SUITE_REGISTRATION (stromx::runtime::RuntimeTest);

namespace stromx
{
    namespace runtime
    {
        void RuntimeTest::setUp ( void )
        {
            m_factory = new runtime::Factory;
        }
        
        void RuntimeTest::testRegisterRuntime()
        {
            CPPUNIT_ASSERT_NO_THROW(stromxRegisterRuntime(*m_factory));
            
            runtime::Data* data = 0;
            CPPUNIT_ASSERT_NO_THROW(data = m_factory->newData("Runtime", "UInt32"));
            CPPUNIT_ASSERT(data);
            delete data;
        }
        
        void RuntimeTest::tearDown ( void )
        {
            delete m_factory;
        }
    }
}
