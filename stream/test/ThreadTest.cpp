#include "ThreadTest.h"

#include "TestOperator.h"

#include <stream/SynchronizedOperatorKernel.h>
#include <stream/Operator.h>
#include <stream/None.h>
#include <stream/InputNode.h>
#include <stream/Thread.h>
#include <stream/Exception.h>

#include <cppunit/TestAssert.h>
#include <boost/thread/thread.hpp>

CPPUNIT_TEST_SUITE_REGISTRATION (stream::ThreadTest);

namespace stream
{
    void ThreadTest::setUp()
    {
        for(unsigned int i = 0; i < 3; ++i)
        {
            m_operators.push_back(new TestOperator());
            SynchronizedOperatorKernel* wrapper = new SynchronizedOperatorKernel(m_operators.back());
            wrapper->initialize();
            
            m_operatorNodes.push_back(new Operator(wrapper));
            wrapper->activate();
        }
        
        for(unsigned int i = 0; i < 2; ++i)
        {
            m_operatorNodes[i + 1]->getInputNode(TestOperator::INPUT_1)
                ->connect(m_operatorNodes[i]->getOutputNode(TestOperator::OUTPUT_1));
                
            m_operatorNodes[i + 1]->getInputNode(TestOperator::INPUT_2)
                ->connect(m_operatorNodes[i]->getOutputNode(TestOperator::OUTPUT_2));
        }
        
        m_thread = new Thread();
        
        for(unsigned int i = 1; i < 3; ++i)
        {
            for(std::vector<const Description*>::const_iterator input = m_operatorNodes[i]->kernel()->info()->inputs().begin();
                input != m_operatorNodes[i]->kernel()->info()->inputs().end();
                ++input)
            {
                InputNode* inputNode = m_operatorNodes[i]->getInputNode((*input)->id());
                m_thread->addNode(inputNode);
            }
        }
        
        m_container = DataContainer(new stream::None);
        SynchronizedOperatorKernel* wrapper = new SynchronizedOperatorKernel(new TestOperator());
        wrapper->initialize();
        m_operatorNode = new Operator(wrapper);
        m_node = m_operatorNode->getInputNode(TestOperator::INPUT_1);
    }
    
    void ThreadTest::testAddOperator()
    {
        unsigned int numNodes = m_thread->nodeSequence().size();
        
        CPPUNIT_ASSERT_NO_THROW(m_thread->addNode(m_node));
        CPPUNIT_ASSERT_EQUAL(numNodes + 1, (unsigned int)(m_thread->nodeSequence().size()));
        CPPUNIT_ASSERT_EQUAL(m_node, m_thread->nodeSequence()[numNodes]);  
    }

    void ThreadTest::testInsertOperator()
    {
        unsigned int numNodes = m_thread->nodeSequence().size();
        
        CPPUNIT_ASSERT_NO_THROW(m_thread->insertNode(0, m_node));
        CPPUNIT_ASSERT_EQUAL(numNodes + 1, (unsigned int)(m_thread->nodeSequence().size()));
        CPPUNIT_ASSERT_EQUAL(m_node, m_thread->nodeSequence()[0]);  
    }

    void ThreadTest::testRemoveOperator()
    {
        m_thread->insertNode(0, m_node);
        unsigned int numNodes = m_thread->nodeSequence().size();
        
        CPPUNIT_ASSERT_NO_THROW(m_thread->removeNode(0));
        CPPUNIT_ASSERT_EQUAL(numNodes - 1, (unsigned int)(m_thread->nodeSequence().size()));
        CPPUNIT_ASSERT(m_node != m_thread->nodeSequence()[0]);  
    }
    
    void ThreadTest::testStart()
    {
        CPPUNIT_ASSERT_NO_THROW(m_thread->start());
        CPPUNIT_ASSERT_THROW(m_thread->start(), WrongState);
        
        m_operatorNodes[0]->kernel()->setInputData(TestOperator::INPUT_1, m_container);
        m_operatorNodes[0]->kernel()->setInputData(TestOperator::INPUT_2, m_container);
        
        boost::this_thread::sleep(boost::posix_time::seconds(1));
        
        DataContainer data = m_operatorNodes[2]->kernel()->getOutputData(TestOperator::OUTPUT_1);
        CPPUNIT_ASSERT_EQUAL(m_container, data);
        
        data = m_operatorNodes[2]->kernel()->getOutputData(TestOperator::OUTPUT_2);
        CPPUNIT_ASSERT_EQUAL(m_container, data);
        
        for(std::vector<TestOperator*>::const_iterator iter = m_operators.begin();
            iter != m_operators.end();
            ++iter)
        {
            CPPUNIT_ASSERT_EQUAL((unsigned int)(1), (*iter)->numExecutes());
        }     
    }

    void ThreadTest::testStop()
    {
        CPPUNIT_ASSERT_NO_THROW(m_thread->stop());
        
        m_thread->start();
        CPPUNIT_ASSERT_NO_THROW(m_thread->stop());
        CPPUNIT_ASSERT_NO_THROW(m_thread->stop());
    }

    void ThreadTest::testJoin()
    {
        CPPUNIT_ASSERT_THROW(m_thread->join(), WrongState);
        
        m_thread->start();
        m_thread->stop();
        
        CPPUNIT_ASSERT_NO_THROW(m_thread->join());
        CPPUNIT_ASSERT_THROW(m_thread->join(), WrongState);
    }

    void ThreadTest::tearDown()
    {
        if(m_thread)
        {
            m_thread->stop();
            
            if(m_thread->status() == Thread::DEACTIVATING)
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

