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
#include "StreamTest.h"
#include "TestOperator.h"
#include "TestUtilities.h"
#include "../Exception.h"
#include "../Operator.h"
#include "../Stream.h"
#include "../Thread.h"
#include "../impl/Network.h"

CPPUNIT_TEST_SUITE_REGISTRATION (stromx::core::StreamTest);

namespace stromx
{
    namespace core
    {
        void StreamTest::TestObserver::observe(const Phase phase, const OperatorError & ex, const Thread* const thread) const
        {
            m_phase = phase;
            m_message = std::string(ex.what());
            m_thread = thread;
        }

        void StreamTest::setUp()
        {
            m_stream = TestUtilities::buildTestStream();
        }

        void StreamTest::tearDown()
        {            
            if(m_stream)
            {
                m_stream->stop();
                m_stream->join();
            }
            
            delete m_stream;
        }
        
        void StreamTest::testAddThread()
        {
            Thread* thr = 0;
            CPPUNIT_ASSERT_NO_THROW(thr = m_stream->addThread());
            CPPUNIT_ASSERT_EQUAL((unsigned int)(2), (unsigned int)(m_stream->threads().size()));
            CPPUNIT_ASSERT_EQUAL(thr, m_stream->threads()[1]);
        }
        
        void StreamTest::testRemoveThread()
        {
            CPPUNIT_ASSERT_THROW(m_stream->removeThread(0), WrongArgument);
            
            Thread* thr = m_stream->threads()[0];
           
            CPPUNIT_ASSERT_NO_THROW(m_stream->removeThread(thr));
            CPPUNIT_ASSERT_EQUAL((unsigned int)(0), (unsigned int)(m_stream->threads().size()));
            
            CPPUNIT_ASSERT_THROW(m_stream->removeThread(thr), WrongArgument);
        }
        
        void StreamTest::testRemoveOperator()
        {
            Operator* op = m_stream->operators()[1];
            
            CPPUNIT_ASSERT_NO_THROW(m_stream->removeOperator(op));
            CPPUNIT_ASSERT_EQUAL((unsigned int)(2), (unsigned int)(m_stream->operators().size()));
            CPPUNIT_ASSERT_EQUAL((unsigned int)(2), (unsigned int)(m_stream->threads()[0]->inputSequence().size()));
        }

        void StreamTest::testPause()
        {
            CPPUNIT_ASSERT_THROW(m_stream->pause(), WrongState);
            
            m_stream->start();
            
            CPPUNIT_ASSERT_NO_THROW(m_stream->pause());
            CPPUNIT_ASSERT_EQUAL(Stream::PAUSED, m_stream->status());
            CPPUNIT_ASSERT_THROW(m_stream->pause(), WrongState);
        }

        void StreamTest::testResume()
        {
            CPPUNIT_ASSERT_THROW(m_stream->resume(), WrongState);
            
            m_stream->start();
            m_stream->pause();
            
            CPPUNIT_ASSERT_NO_THROW(m_stream->resume());
            CPPUNIT_ASSERT_EQUAL(Stream::ACTIVE, m_stream->status());
            CPPUNIT_ASSERT_THROW(m_stream->resume(), WrongState);
        }
        
        void StreamTest::testAddObserver()
        {
            TestObserver* observer1 = new TestObserver;
            TestObserver* observer2 = new TestObserver;
            
            CPPUNIT_ASSERT_NO_THROW(m_stream->addObserver(observer1));
            CPPUNIT_ASSERT_NO_THROW(m_stream->addObserver(observer2));
            CPPUNIT_ASSERT_THROW(m_stream->addObserver(0), WrongArgument);
        }

        void StreamTest::testRemoveObserver()
        {
            TestObserver* observer1 = new TestObserver;
            TestObserver* observer2 = new TestObserver;
            
            m_stream->addObserver(observer1);
            m_stream->addObserver(observer2);
            
            CPPUNIT_ASSERT_NO_THROW(m_stream->removeObserver(observer1));
            CPPUNIT_ASSERT_THROW(m_stream->removeObserver(observer1), WrongArgument);
            CPPUNIT_ASSERT_NO_THROW(m_stream->removeObserver(observer2));
        }
                
        void StreamTest::testObserver()
        {
            TestObserver* observer = new TestObserver;
            m_stream->addObserver(observer);
            m_stream->operators()[0]->setParameter(TestOperator::THROW_EXCEPTION, Bool(true));
            
            m_stream->start();
            boost::this_thread::sleep(boost::posix_time::seconds(1));
            m_stream->stop();
            m_stream->join();
            
            CPPUNIT_ASSERT_EQUAL(std::string("Funny exception."), observer->message());
            CPPUNIT_ASSERT(observer->thread());
        }
        
        void StreamTest::testTwoObserver()
        {
            TestObserver* observer1 = new TestObserver;
            TestObserver* observer2 = new TestObserver;
            m_stream->addObserver(observer1);
            m_stream->addObserver(observer2);
            
            m_stream->operators()[0]->setParameter(TestOperator::THROW_EXCEPTION, Bool(true));
            
            m_stream->start();
            boost::this_thread::sleep(boost::posix_time::seconds(1));
            m_stream->stop();
            m_stream->join();
            
            CPPUNIT_ASSERT_EQUAL(std::string("Funny exception."), observer1->message());
            CPPUNIT_ASSERT(observer1->thread());
            CPPUNIT_ASSERT_EQUAL(std::string("Funny exception."), observer2->message());
            CPPUNIT_ASSERT(observer2->thread());
        }        
    }
}