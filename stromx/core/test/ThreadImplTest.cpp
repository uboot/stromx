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

#include <boost/thread/thread.hpp>
#include <cppunit/TestAssert.h>
#include "TestOperator.h"
#include "ThreadImplTest.h"
#include "../Exception.h"
#include "../None.h"
#include "../Operator.h"
#include "../OperatorTester.h"
#include "../impl/InputNode.h"
#include "../impl/ThreadImpl.h"

CPPUNIT_TEST_SUITE_REGISTRATION (stromx::core::ThreadImplTest);

namespace stromx
{
    namespace core
    {
        using namespace impl;
        
        void ThreadImplTest::setUp()
        {
            for(unsigned int i = 0; i < 3; ++i)
            {
                TestOperator* kernel = new TestOperator();
                m_kernels.push_back(kernel);
                
                Operator* op = new Operator(kernel);
                op->initialize();
                op->activate();
                
                m_operators.push_back(op);
            }
            
            for(unsigned int i = 0; i < 2; ++i)
            {
                m_operators[i + 1]->getInputNode(TestOperator::INPUT_1)
                    ->connect(m_operators[i]->getOutputNode(TestOperator::OUTPUT_1));
                    
                m_operators[i + 1]->getInputNode(TestOperator::INPUT_2)
                    ->connect(m_operators[i]->getOutputNode(TestOperator::OUTPUT_2));
            }
            
            m_thread = new ThreadImpl();
            
            for(unsigned int i = 1; i < 3; ++i)
            {
                for(std::vector<const Description*>::const_iterator input = m_operators[i]->info().inputs().begin();
                    input != m_operators[i]->info().inputs().end();
                    ++input)
                {
                    InputNode* inputNode = m_operators[i]->getInputNode((*input)->id());
                    m_thread->addNode(inputNode);
                }
            }
            
            m_container = DataContainer(new core::None);
            
            m_operator = new Operator(new TestOperator());
            m_operator->initialize();
            m_node = m_operator->getInputNode(TestOperator::INPUT_1);
        }
        
        void ThreadImplTest::testAddOperator()
        {
            unsigned int numNodes = m_thread->inputSequence().size();
            
            CPPUNIT_ASSERT_NO_THROW(m_thread->addNode(m_node));
            CPPUNIT_ASSERT_EQUAL(numNodes + 1, (unsigned int)(m_thread->inputSequence().size()));
            CPPUNIT_ASSERT_EQUAL(m_node, m_thread->inputSequence()[numNodes]);  
        }

        void ThreadImplTest::testInsertOperator()
        {
            unsigned int numNodes = m_thread->inputSequence().size();
            
            CPPUNIT_ASSERT_NO_THROW(m_thread->insertNode(0, m_node));
            CPPUNIT_ASSERT_EQUAL(numNodes + 1, (unsigned int)(m_thread->inputSequence().size()));
            CPPUNIT_ASSERT_EQUAL(m_node, m_thread->inputSequence()[0]);  
        }

        void ThreadImplTest::testRemoveNode()
        {
            m_thread->insertNode(0, m_node);
            unsigned int numNodes = m_thread->inputSequence().size();
            
            CPPUNIT_ASSERT_NO_THROW(m_thread->removeNode(0));
            CPPUNIT_ASSERT_EQUAL(numNodes - 1, (unsigned int)(m_thread->inputSequence().size()));
            CPPUNIT_ASSERT(m_node != m_thread->inputSequence()[0]);  
        }
        
        void ThreadImplTest::testRemoveOperator()
        {
            Operator* op = m_operators[1];
            
            CPPUNIT_ASSERT_NO_THROW(m_thread->removeOperator(op));
            CPPUNIT_ASSERT_EQUAL((unsigned int)(2), (unsigned int)(m_thread->inputSequence().size()));
            
            CPPUNIT_ASSERT_NO_THROW(m_thread->removeOperator(m_operator));
            CPPUNIT_ASSERT_EQUAL((unsigned int)(2), (unsigned int)(m_thread->inputSequence().size()));
        }
        
        void ThreadImplTest::testStart()
        {
            CPPUNIT_ASSERT_NO_THROW(m_thread->start());
            CPPUNIT_ASSERT_THROW(m_thread->start(), WrongState);
            
            m_operators[0]->setInputData(TestOperator::INPUT_1, m_container);
            m_operators[0]->setInputData(TestOperator::INPUT_2, m_container);
            
            boost::this_thread::sleep(boost::posix_time::seconds(1));
            
            DataContainer data = m_operators[2]->getOutputData(TestOperator::OUTPUT_1);
            CPPUNIT_ASSERT_EQUAL(m_container, data);
            
            data = m_operators[2]->getOutputData(TestOperator::OUTPUT_2);
            CPPUNIT_ASSERT_EQUAL(m_container, data);
            
            for(std::vector<TestOperator*>::const_iterator iter = m_kernels.begin();
                iter != m_kernels.end();
                ++iter)
            {
                CPPUNIT_ASSERT_EQUAL((unsigned int)(1), (*iter)->numExecutes());
            }     
        }

        void ThreadImplTest::testStop()
        {
            CPPUNIT_ASSERT_NO_THROW(m_thread->stop());
            
            m_thread->start();
            CPPUNIT_ASSERT_NO_THROW(m_thread->stop());
            CPPUNIT_ASSERT_NO_THROW(m_thread->stop());
            CPPUNIT_ASSERT_EQUAL(ThreadImpl::DEACTIVATING, m_thread->status());
        }

        void ThreadImplTest::testJoin()
        {
            CPPUNIT_ASSERT_NO_THROW(m_thread->join());
            
            m_thread->start();
            m_thread->stop();
            
            CPPUNIT_ASSERT_NO_THROW(m_thread->join());
            CPPUNIT_ASSERT_EQUAL(ThreadImpl::INACTIVE, m_thread->status());
        }
        
        void ThreadImplTest::testPause()
        {
            CPPUNIT_ASSERT_THROW(m_thread->pause(), WrongState);
            
            m_thread->start();
            CPPUNIT_ASSERT_NO_THROW(m_thread->pause());
            
            m_operators[0]->setInputData(TestOperator::INPUT_1, m_container);
            m_operators[0]->setInputData(TestOperator::INPUT_2, m_container);
            boost::this_thread::sleep(boost::posix_time::seconds(1));
            
            CPPUNIT_ASSERT_EQUAL(ThreadImpl::PAUSED, m_thread->status());
        }
        
        void ThreadImplTest::testResume()
        {
            CPPUNIT_ASSERT_THROW(m_thread->resume(), WrongState);
            
            m_thread->start();
            
            CPPUNIT_ASSERT_THROW(m_thread->resume(), WrongState);
            
            m_thread->pause();
            m_operators[0]->setInputData(TestOperator::INPUT_1, m_container);
            m_operators[0]->setInputData(TestOperator::INPUT_2, m_container);
            boost::this_thread::sleep(boost::posix_time::seconds(1));
            
            CPPUNIT_ASSERT_NO_THROW(m_thread->resume());
            CPPUNIT_ASSERT_EQUAL(ThreadImpl::ACTIVE, m_thread->status());
        }

        void ThreadImplTest::tearDown()
        {
            if(m_thread)
            {
                m_thread->stop();
                m_thread->join();
            }
            
            delete m_thread;
            
            for(std::vector<Operator*>::iterator iter = m_operators.begin();
                iter != m_operators.end();
                ++iter)
            {
                delete *iter;
            }
            
            delete m_operator;
        }
    }
}

