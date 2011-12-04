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
#include <stromx/core/Exception.h>
#include <stromx/core/None.h>
#include <stromx/core/Operator.h>
#include <stromx/core/OperatorTester.h>
#include <stromx/core/impl/InputNode.h>
#include <stromx/core/impl/ThreadImpl.h>

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
                
                m_operatorNodes.push_back(op);
            }
            
            for(unsigned int i = 0; i < 2; ++i)
            {
                m_operatorNodes[i + 1]->getInputNode(TestOperator::INPUT_1)
                    ->connect(m_operatorNodes[i]->getOutputNode(TestOperator::OUTPUT_1));
                    
                m_operatorNodes[i + 1]->getInputNode(TestOperator::INPUT_2)
                    ->connect(m_operatorNodes[i]->getOutputNode(TestOperator::OUTPUT_2));
            }
            
            m_thread = new ThreadImpl();
            
            for(unsigned int i = 1; i < 3; ++i)
            {
                for(std::vector<const Description*>::const_iterator input = m_operatorNodes[i]->info().inputs().begin();
                    input != m_operatorNodes[i]->info().inputs().end();
                    ++input)
                {
                    InputNode* inputNode = m_operatorNodes[i]->getInputNode((*input)->id());
                    m_thread->addNode(inputNode);
                }
            }
            
            m_container = DataContainer(new core::None);
            
            m_operatorNode = new Operator(new TestOperator());
            m_operatorNode->initialize();
            m_node = m_operatorNode->getInputNode(TestOperator::INPUT_1);
        }
        
        void ThreadImplTest::testAddOperator()
        {
            unsigned int numNodes = m_thread->nodeSequence().size();
            
            CPPUNIT_ASSERT_NO_THROW(m_thread->addNode(m_node));
            CPPUNIT_ASSERT_EQUAL(numNodes + 1, (unsigned int)(m_thread->nodeSequence().size()));
            CPPUNIT_ASSERT_EQUAL(m_node, m_thread->nodeSequence()[numNodes]);  
        }

        void ThreadImplTest::testInsertOperator()
        {
            unsigned int numNodes = m_thread->nodeSequence().size();
            
            CPPUNIT_ASSERT_NO_THROW(m_thread->insertNode(0, m_node));
            CPPUNIT_ASSERT_EQUAL(numNodes + 1, (unsigned int)(m_thread->nodeSequence().size()));
            CPPUNIT_ASSERT_EQUAL(m_node, m_thread->nodeSequence()[0]);  
        }

        void ThreadImplTest::testRemoveOperator()
        {
            m_thread->insertNode(0, m_node);
            unsigned int numNodes = m_thread->nodeSequence().size();
            
            CPPUNIT_ASSERT_NO_THROW(m_thread->removeNode(0));
            CPPUNIT_ASSERT_EQUAL(numNodes - 1, (unsigned int)(m_thread->nodeSequence().size()));
            CPPUNIT_ASSERT(m_node != m_thread->nodeSequence()[0]);  
        }
        
        void ThreadImplTest::testStart()
        {
            CPPUNIT_ASSERT_NO_THROW(m_thread->start());
            CPPUNIT_ASSERT_THROW(m_thread->start(), WrongState);
            
            m_operatorNodes[0]->setInputData(TestOperator::INPUT_1, m_container);
            m_operatorNodes[0]->setInputData(TestOperator::INPUT_2, m_container);
            
            boost::this_thread::sleep(boost::posix_time::seconds(1));
            
            DataContainer data = m_operatorNodes[2]->getOutputData(TestOperator::OUTPUT_1);
            CPPUNIT_ASSERT_EQUAL(m_container, data);
            
            data = m_operatorNodes[2]->getOutputData(TestOperator::OUTPUT_2);
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
        }

        void ThreadImplTest::testJoin()
        {
            CPPUNIT_ASSERT_THROW(m_thread->join(), WrongState);
            
            m_thread->start();
            m_thread->stop();
            
            CPPUNIT_ASSERT_NO_THROW(m_thread->join());
            CPPUNIT_ASSERT_THROW(m_thread->join(), WrongState);
        }

        void ThreadImplTest::tearDown()
        {
            if(m_thread)
            {
                m_thread->stop();
                
                if(m_thread->status() == ThreadImpl::DEACTIVATING)
                    m_thread->join();
            }
            
            delete m_thread;
            
            for(std::vector<Operator*>::iterator iter = m_operatorNodes.begin();
                iter != m_operatorNodes.end();
                ++iter)
            {
                delete *iter;
            }
            
            delete m_operatorNode;
        }
    }
}

