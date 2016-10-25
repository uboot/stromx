/* 
*  Copyright 2016 Matthias Fuchs
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

#include "stromx/runtime/IsNotEmpty.h"

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestAssert.h>
#include <cppunit/TestFixture.h>

#include <stromx/runtime/DataContainer.h>
#include <stromx/runtime/OperatorTester.h>
#include <stromx/runtime/ReadAccess.h>

#include "stromx/runtime/test/MatrixImpl.h"

namespace stromx
{
namespace runtime
{

class IsNotEmptyTest : public CPPUNIT_NS :: TestFixture
{
    CPPUNIT_TEST_SUITE (IsNotEmptyTest);
    CPPUNIT_TEST (testExecute0x0);
    CPPUNIT_TEST (testExecute1x0);
    CPPUNIT_TEST (testExecute1x1);
    CPPUNIT_TEST_SUITE_END ();

    public:
        IsNotEmptyTest() : m_operator(0) {}
        
        void setUp();
        void tearDown();

    protected:
        void testExecute0x0();
        void testExecute1x0();
        void testExecute1x1();
        
    private:
        runtime::OperatorTester* m_operator;
};

CPPUNIT_TEST_SUITE_REGISTRATION (IsNotEmptyTest);

void IsNotEmptyTest::setUp ( void )
{
    m_operator = new runtime::OperatorTester(new IsNotEmpty());
    m_operator->initialize();
    m_operator->activate();
}

void IsNotEmptyTest::testExecute0x0()
{
    runtime::DataContainer input(new MatrixImpl(0, 0, Matrix::INT_32)); 
    m_operator->setInputData(IsNotEmpty::INPUT, input);
    
    runtime::DataContainer result = m_operator->getOutputData(IsNotEmpty::PROPERTY);
    
    runtime::ReadAccess access(result);
    const Bool & value = access.get<runtime::Bool>();
    CPPUNIT_ASSERT_EQUAL(false, bool(value));
}

void IsNotEmptyTest::testExecute1x0()
{
    runtime::DataContainer input(new MatrixImpl(1, 0, Matrix::INT_32)); 
    m_operator->setInputData(IsNotEmpty::INPUT, input);
    
    runtime::DataContainer result = m_operator->getOutputData(IsNotEmpty::PROPERTY);
    
    runtime::ReadAccess access(result);
    const Bool & value = access.get<runtime::Bool>();
    CPPUNIT_ASSERT_EQUAL(false, bool(value));
}

void IsNotEmptyTest::testExecute1x1()
{
    runtime::DataContainer input(new MatrixImpl(1, 1, Matrix::INT_32)); 
    m_operator->setInputData(IsNotEmpty::INPUT, input);
    
    runtime::DataContainer result = m_operator->getOutputData(IsNotEmpty::PROPERTY);
    
    runtime::ReadAccess access(result);
    const Bool & value = access.get<runtime::Bool>();
    CPPUNIT_ASSERT_EQUAL(true, bool(value));
}

void IsNotEmptyTest::tearDown ( void )
{
    delete m_operator;
}

}
}

