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

#include <boost/thread/thread.hpp>
#include <cppunit/TestAssert.h>
#include "stromx/runtime/Dump.h"
#include "stromx/runtime/Exception.h"
#include "stromx/runtime/Operator.h"
#include "stromx/runtime/OperatorTester.h"
#include "stromx/runtime/impl/Network.h"
#include "stromx/runtime/test/NetworkTest.h"
#include "stromx/runtime/test/TestOperator.h"
#include "stromx/runtime/test/ExceptionOperator.h"

CPPUNIT_TEST_SUITE_REGISTRATION (stromx::runtime::NetworkTest);

namespace stromx
{
    namespace runtime
    {
        using namespace impl;
        
        class Operator;
        
        void NetworkTest::setUp()
        {
            m_network = new Network();
        }

        void NetworkTest::testAddOperator()
        {
            //add nullpointer should throw an exception
            CPPUNIT_ASSERT_THROW(m_network->addOperator(0), WrongArgument);
            
            OperatorKernel* kernel = new TestOperator();
            Operator* op1 = new Operator(kernel);
            // add an unitialized operator should throw an exception
            CPPUNIT_ASSERT_THROW(m_network->addOperator(op1), WrongArgument);
            
            op1->initialize();
            // add initialized operator should not throw an exception;
            CPPUNIT_ASSERT_NO_THROW(m_network->addOperator(op1));
            // size of vector containing all operators belonging to the network should be +1 
            CPPUNIT_ASSERT_EQUAL((unsigned int)(1), (unsigned int)(m_network->operators().size()));
            
            CPPUNIT_ASSERT_EQUAL(op1, m_network->operators()[0]);
            
            // can not add the same operator again
            Operator* op2 = new Operator(kernel);
            CPPUNIT_ASSERT_THROW(m_network->addOperator(op2), WrongArgument);
            
            // The pointer op2 should be deleted her, however this results in an error because
            // op has already been deleted. In other words this is deliberate memory leak.
        }
        
        void NetworkTest::testRemoveOperator()
        {
            CPPUNIT_ASSERT_THROW(m_network->removeOperator(0), WrongArgument);
            
            Operator* op = new Operator(new TestOperator());
            op->initialize();
            m_network->addOperator(op);
            
            Operator* testOp = new Operator(new TestOperator());
            CPPUNIT_ASSERT_THROW(m_network->removeOperator(testOp), WrongArgument);
            delete testOp;
            
            CPPUNIT_ASSERT_NO_THROW(m_network->removeOperator(op));
            CPPUNIT_ASSERT_EQUAL((unsigned int)(0), (unsigned int)(m_network->operators().size()));
        }
        
        void NetworkTest::testRemoveConnectedOperator()
        {
            Operator* op0 = new Operator(new TestOperator);
            op0->initialize();
            m_network->addOperator(op0);
            
            Operator* op1 = new Operator(new TestOperator);
            op1->initialize();
            m_network->addOperator(op1);
            
            Operator* op2 = new Operator(new TestOperator);
            op2->initialize();
            m_network->addOperator(op2);
            
            m_network->connect(op0, TestOperator::OUTPUT_1, op1, TestOperator::INPUT_1);
            m_network->connect(op0, TestOperator::OUTPUT_2, op1, TestOperator::INPUT_2);
            
            m_network->connect(op1, TestOperator::OUTPUT_1, op2, TestOperator::INPUT_1);
            m_network->connect(op1, TestOperator::OUTPUT_2, op2, TestOperator::INPUT_2);
            
            CPPUNIT_ASSERT_NO_THROW(m_network->removeOperator(op1));
            CPPUNIT_ASSERT(! m_network->connectionSource(op2, TestOperator::INPUT_1).valid());
            CPPUNIT_ASSERT(! m_network->connectionSource(op2, TestOperator::INPUT_2).valid());
        }
    
        void NetworkTest::testConnectionSource()
        {
            Operator* op0 = new Operator(new TestOperator);
            op0->initialize();
            m_network->addOperator(op0);
            
            Operator* op1 = new Operator(new TestOperator);
            op1->initialize();
            m_network->addOperator(op1);
            
            Output source;
            CPPUNIT_ASSERT_NO_THROW(source = m_network->connectionSource(op1, TestOperator::INPUT_1));
            CPPUNIT_ASSERT(! source.valid());
            
            m_network->connect(op0, TestOperator::OUTPUT_1, op1, TestOperator::INPUT_1);
            m_network->connect(op0, TestOperator::OUTPUT_2, op1, TestOperator::INPUT_2);
            
            CPPUNIT_ASSERT_NO_THROW(source = m_network->connectionSource(op1, TestOperator::INPUT_1));
            CPPUNIT_ASSERT_EQUAL(static_cast<const Operator*>(op0), source.op());
            CPPUNIT_ASSERT_EQUAL((unsigned int)(TestOperator::OUTPUT_1), source.id());
        }

        void NetworkTest::testActivate()
        {
            Operator* op1 = new Operator(new TestOperator());
            Operator* op2 = new Operator(new TestOperator());
            op1->initialize();
            op2->initialize();
            m_network->addOperator(op1);
            m_network->addOperator(op2);
            CPPUNIT_ASSERT_NO_THROW(m_network->activate());
            CPPUNIT_ASSERT_EQUAL(Operator::ACTIVE,op1->status());
            CPPUNIT_ASSERT_EQUAL(Operator::ACTIVE,op2->status());
        }

        void NetworkTest::testDeactivate()
        {
            CPPUNIT_ASSERT_NO_THROW(m_network->deactivate());
            Operator* op1 = new Operator(new TestOperator());
            Operator* op2 = new Operator(new TestOperator());
            op1->initialize();
            op2->initialize();
            m_network->addOperator(op1);
            m_network->addOperator(op2);
            m_network->activate();
            CPPUNIT_ASSERT_NO_THROW(m_network->deactivate());
        }
        
        void NetworkTest::testDeactivateFails()
        {
            Operator* op1 = new Operator(new ExceptionOperator);
            Operator* op2 = new Operator(new Dump);
            
            op1->initialize();
            op2->initialize();
            op1->setParameter(ExceptionOperator::THROW_DEACTIVATE, Bool(true));
            m_network->addOperator(op1);
            m_network->addOperator(op2);
            m_network->activate();
            
            CPPUNIT_ASSERT_NO_THROW(m_network->deactivate());
        }

        void NetworkTest::tearDown()
        {
            delete m_network;
            
        }
    }
}
