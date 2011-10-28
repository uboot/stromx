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

#include "StromTest.h"

#include <cppunit/TestAssert.h>

#include <stromx/core/Strom.h>
#include <stromx/core/Factory.h>
#include <stromx/core/Operator.h>
#include <stromx/core/Data.h>

CPPUNIT_TEST_SUITE_REGISTRATION (core::StromTest);

namespace core
{
    void StromTest::setUp ( void )
    {
        m_factory = new core::Factory;
    }
    
    void StromTest::testRegisterStrom()
    {
        CPPUNIT_ASSERT_NO_THROW(registerStrom(*m_factory));
        
        core::Data* data = 0;
        CPPUNIT_ASSERT_NO_THROW(data = m_factory->newData("Strom", "UInt32"));
        CPPUNIT_ASSERT(data);
        delete data;
    }
    
    void StromTest::tearDown ( void )
    {
        delete m_factory;
    }
}
