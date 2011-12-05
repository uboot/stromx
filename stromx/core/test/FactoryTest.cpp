/* 
 *  Copyright 2011 Thomas Fidler
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
#include "FactoryTest.h"
#include "TestData.h"
#include "TestOperator.h"
#include "../Exception.h"
#include "../Factory.h"
#include "../Operator.h"

CPPUNIT_TEST_SUITE_REGISTRATION (stromx::core::FactoryTest);

namespace stromx
{
    namespace core
    {
        void FactoryTest::setUp()
        {
            // allocate a new empty factory
            m_factory = new Factory;
        }

        void FactoryTest::tearDown()
        {
            // the factory automatically deletes all its operators
            // (cf. aggregation, i.e. the connection with the black diamond, in
            // the UML diagram)
            delete m_factory;
        }
        
        void FactoryTest::testNewOperator()
        {
            // add a test operator to the factory
            OperatorKernel* op = new TestOperator;
            m_factory->registerOperator(op);
            
            // try to get an operator which does not exist in the factory
            CPPUNIT_ASSERT_THROW(m_factory->newOperator("FunnyPackage", "RareOperator"), WrongArgument);
            
            // get the existing operator
            Operator* newOp;
            CPPUNIT_ASSERT_NO_THROW(newOp = m_factory->newOperator("TestPackage", "TestOperator"));
            CPPUNIT_ASSERT(newOp);
            
            delete newOp;
        }
        
        void FactoryTest::testNewData()
        {
            // add a test data to the factory
            Data* data = new TestData;
            m_factory->registerData(data);
            
            // try to get data which does not exist in the factory
            CPPUNIT_ASSERT_THROW(m_factory->newOperator("FunnyPackage", "RareData"), WrongArgument);
            
            // get the existing data
            Data* newData;
            CPPUNIT_ASSERT_NO_THROW(newData = m_factory->newData("TestPackage", "TestData"));
            CPPUNIT_ASSERT(newData);
            
            delete newData;
        }
        
        void FactoryTest::testRegisterData()
        {
            // only non-zero input arguments are allowed
            CPPUNIT_ASSERT_THROW(m_factory->registerData(0), WrongArgument);
            
            // add a new data to the factory
            Data* data = new TestData;
            CPPUNIT_ASSERT_NO_THROW(m_factory->registerData(data));
            
            // now we try to add another data of the same type
            // this should result in an exception, because a data with the same name()
            // and package() has already been added to the factory
            Data* duplicateData = new TestData;
            CPPUNIT_ASSERT_THROW(m_factory->registerData(duplicateData), WrongArgument);
            
            // delete duplicateData to avoid a memory leak
            // this is not necessary for data because it belongs to the factory
            delete duplicateData;
        }
        
        void FactoryTest::testRegisterOperator()
        {
            // only non-zero input arguments are allowed
            CPPUNIT_ASSERT_THROW(m_factory->registerOperator(0), WrongArgument);
            
            // add a new operator to the factory
            OperatorKernel* op = new TestOperator;
            CPPUNIT_ASSERT_NO_THROW(m_factory->registerOperator(op));
            
            // test if it has been successfully added
            CPPUNIT_ASSERT_EQUAL(1, (int)m_factory->availableOperators().size());
            CPPUNIT_ASSERT_EQUAL((const OperatorKernel*)(op), m_factory->availableOperators()[0]);
            
            // now we try to add another operator of the same type
            // this should result in an exception, because an operator with the same name()
            // and package() has already been added to the factory
            OperatorKernel* duplicateOp = new TestOperator;
            CPPUNIT_ASSERT_THROW(m_factory->registerOperator(duplicateOp), WrongArgument);
            
            // delete duplicateOperator to avoid a memory leak
            // this is not necessary for op because it belongs to the factory
            delete duplicateOp;
        }
    }
}
