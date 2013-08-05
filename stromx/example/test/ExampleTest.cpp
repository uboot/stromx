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
#include <stromx/runtime/Data.h>
#include <stromx/runtime/Factory.h>
#include <stromx/runtime/Operator.h>
#include "stromx/example/Example.h"
#include "stromx/example/test/ExampleTest.h"

CPPUNIT_TEST_SUITE_REGISTRATION (stromx::example::ExampleTest);

namespace stromx
{
    namespace example
    {
        void ExampleTest::setUp ( void )
        {
            m_factory = new runtime::Factory;
        }
        
        void ExampleTest::testregister()
        {
            CPPUNIT_ASSERT_NO_THROW(stromxExampleRegister(*m_factory));
            
            runtime::Operator* kernel = 0;
            CPPUNIT_ASSERT_NO_THROW(kernel = m_factory->newOperator("Example", "Camera"));
            CPPUNIT_ASSERT(kernel);
            delete kernel;
            
            runtime::Data* data = 0;
            CPPUNIT_ASSERT_NO_THROW(data = m_factory->newData("Example", "Image"));
            CPPUNIT_ASSERT(data);
            delete data;
        }
        
        void ExampleTest::tearDown ( void )
        {
            delete m_factory;
        }
    }
}
