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

#include <strom/Strom.h>
#include <strom/Factory.h>
#include <strom/Operator.h>
#include <strom/Data.h>

CPPUNIT_TEST_SUITE_REGISTRATION (strom::StromTest);

namespace strom
{
    void StromTest::setUp ( void )
    {
        m_factory = new strom::Factory;
    }
    
    void StromTest::testRegisterStrom()
    {
        CPPUNIT_ASSERT_NO_THROW(registerStrom(m_factory));
        
        strom::Data* data = 0;
        CPPUNIT_ASSERT_NO_THROW(data = m_factory->newData("Strom", "UInt32"));
        CPPUNIT_ASSERT(data);
        delete data;
    }
    
    void StromTest::tearDown ( void )
    {
        delete m_factory;
    }
}
