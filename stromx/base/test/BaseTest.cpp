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
#include "BaseTest.h"
#include "../Base.h"
#include <stromx/core/Data.h>
#include <stromx/core/Factory.h>
#include <stromx/core/Operator.h>

CPPUNIT_TEST_SUITE_REGISTRATION (stromx::base::BaseTest);

namespace stromx
{
    namespace base
    {
        void BaseTest::setUp ( void )
        {
            m_factory = new core::Factory;
        }
        
        void BaseTest::testRegisterBase()
        {
            CPPUNIT_ASSERT_NO_THROW(stromxRegisterBase(*m_factory));
            
            core::Operator* kernel = 0;
            CPPUNIT_ASSERT_NO_THROW(kernel = m_factory->newOperator("Base", "Camera"));
            CPPUNIT_ASSERT(kernel);
            delete kernel;
            
            core::Data* data = 0;
            CPPUNIT_ASSERT_NO_THROW(data = m_factory->newData("Base", "Image"));
            CPPUNIT_ASSERT(data);
            delete data;
        }
        
        void BaseTest::tearDown ( void )
        {
            delete m_factory;
        }
    }
}
