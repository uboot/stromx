#include "ThreadTest.h"

#include "TestOperator.h"

#include <stream/OperatorWrapper.h>
#include <stream/OperatorNode.h>
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
            OperatorWrapper* wrapper = new OperatorWrapper(m_operators.back());
            m_operatorNodes.push_back(new OperatorNode(wrapper));
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
            m_thread->addOperator(m_operatorNodes[i]);
        
        m_container = DataContainer(new stream::None);
        m_newOperator = new OperatorNode(new OperatorWrapper(new TestOperator()));
    }
    
    void ThreadTest::testAddOperator()
    {
        unsigned int numOperators = m_thread->operatorSequence().size();
        
        CPPUNIT_ASSERT_NO_THROW(m_thread->addOperator(m_newOperator));
        CPPUNIT_ASSERT_EQUAL(numOperators + 1, (unsigned int)(m_thread->operatorSequence().size()));
        CPPUNIT_ASSERT_EQUAL(m_newOperator, m_thread->operatorSequence()[numOperators]);  
    }

    void ThreadTest::testInsertOperator()
    {
        unsigned int numOperators = m_thread->operatorSequence().size();
        
        CPPUNIT_ASSERT_NO_THROW(m_thread->insertOperator(0, m_newOperator));
        CPPUNIT_ASSERT_EQUAL(numOperators + 1, (unsigned int)(m_thread->operatorSequence().size()));
        CPPUNIT_ASSERT_EQUAL(m_newOperator, m_thread->operatorSequence()[0]);  
    }

    void ThreadTest::testRemoveOperator()
    {
        m_thread->insertOperator(0, m_newOperator);
        unsigned int numOperators = m_thread->operatorSequence().size();
        
        CPPUNIT_ASSERT_NO_THROW(m_thread->removeOperator(0));
        CPPUNIT_ASSERT_EQUAL(numOperators - 1, (unsigned int)(m_thread->operatorSequence().size()));
        CPPUNIT_ASSERT(m_newOperator != m_thread->operatorSequence()[0]);  
    }
    
    void ThreadTest::testStart()
    {
        CPPUNIT_ASSERT_NO_THROW(m_thread->start());
        CPPUNIT_ASSERT_THROW(m_thread->start(), InvalidStateException);
        
        m_operatorNodes[0]->op()->setInputData(TestOperator::INPUT_1, m_container);
        m_operatorNodes[0]->op()->setInputData(TestOperator::INPUT_2, m_container);
        
        boost::this_thread::sleep(boost::posix_time::seconds(1));
        
        DataContainer data = m_operatorNodes[2]->op()->getOutputData(TestOperator::OUTPUT_1);
        CPPUNIT_ASSERT_EQUAL(m_container, data);
        
        data = m_operatorNodes[2]->op()->getOutputData(TestOperator::OUTPUT_2);
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
        CPPUNIT_ASSERT_NO_THROW(m_thread->join());
        
        m_thread->start();
        m_thread->stop();
        
        CPPUNIT_ASSERT_NO_THROW(m_thread->join());
        CPPUNIT_ASSERT_NO_THROW(m_thread->join());
    }

    void ThreadTest::tearDown()
    {
        m_thread->stop();
        m_thread->join();
        
        delete m_thread;
        
        for(std::vector<OperatorNode*>::iterator iter = m_operatorNodes.begin();
            iter != m_operatorNodes.end();
            ++iter)
        {
            delete *iter;
        }
        
        delete m_newOperator;
    }
}

