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
#include "stromx/runtime/Exception.h"
#include "stromx/runtime/Factory.h"
#include "stromx/runtime/Operator.h"
#include "stromx/runtime/Stream.h"
#include "stromx/runtime/Thread.h"
#include "stromx/runtime/impl/Network.h"
#include "stromx/runtime/test/StreamTest.h"
#include "stromx/runtime/test/TestOperator.h"
#include "stromx/runtime/test/TestUtilities.h"

CPPUNIT_TEST_SUITE_REGISTRATION (stromx::runtime::StreamTest);

namespace stromx
{
    namespace runtime
    {
        void StreamTest::TestObserver::observe(const Phase phase, const OperatorError & ex, const Thread* const thread) const
        {
            m_phase = phase;
            m_message = std::string(ex.what());
            m_thread = thread;
        }

        void StreamTest::setUp()
        {
            // Build a complete stream (with connected operators)
            m_stream = TestUtilities::buildTestStream();
            
            // Build two new operators (not connected, not known by the stream)
            m_op1 = new TestOperator;
            m_op1->setParameter(TestOperator::BUFFER_SIZE, UInt32(7000));
            
            m_op2 = new TestOperator;
            m_op2->setParameter(TestOperator::BUFFER_SIZE, UInt32(7000));
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
        
        void StreamTest::testOperators()
        {
            CPPUNIT_ASSERT_EQUAL((unsigned int)(4),(unsigned int)(m_stream->operators().size()));
        }
        
        void StreamTest::testFactory()
        {
            CPPUNIT_ASSERT_EQUAL((const AbstractFactory*)(0),
                                 m_stream->factory());
            
            CPPUNIT_ASSERT_EQUAL((const AbstractFactory*)(0),
                                 m_stream->operators()[0]->factory());
            CPPUNIT_ASSERT_EQUAL((const AbstractFactory*)(0),
                                 m_stream->operators()[1]->factory());
            CPPUNIT_ASSERT_EQUAL((const AbstractFactory*)(0),
                                 m_stream->operators()[2]->factory());
            CPPUNIT_ASSERT_EQUAL((const AbstractFactory*)(0),
                                 m_stream->operators()[3]->factory());
        }
        
        void StreamTest::testSetFactory()
        {
            Factory factory;
            
            m_stream->setFactory(&factory);
            
            CPPUNIT_ASSERT_EQUAL((const AbstractFactory*)(&factory),
                                 m_stream->factory());
            
            CPPUNIT_ASSERT_EQUAL((const AbstractFactory*)(&factory),
                                 m_stream->operators()[0]->factory());
            CPPUNIT_ASSERT_EQUAL((const AbstractFactory*)(&factory),
                                 m_stream->operators()[1]->factory());
            CPPUNIT_ASSERT_EQUAL((const AbstractFactory*)(&factory),
                                 m_stream->operators()[2]->factory());
            CPPUNIT_ASSERT_EQUAL((const AbstractFactory*)(&factory),
                                 m_stream->operators()[3]->factory());
        }
        
        void StreamTest::testConnect()
        {
            // Register first operator to the stream
            Operator* op1 = m_stream->addOperator(m_op1);
            m_stream->initializeOperator(op1);
            
            Operator* op3 = new Operator(new TestOperator);
                      
            //Invalid source operator (operator op3 not known by the stream)
            CPPUNIT_ASSERT_THROW(m_stream->connect(op3, TestOperator::OUTPUT_1, op1, TestOperator::INPUT_1), WrongArgument);
            //Invalid target operator (operator localOp3 not known by the stream)
            CPPUNIT_ASSERT_THROW(m_stream->connect(op1, TestOperator::OUTPUT_1, op3, TestOperator::INPUT_1), WrongArgument);
            
            // Register first operator to the stream
            Operator* op2 = m_stream->addOperator(m_op2);
            m_stream->initializeOperator(op2);
            
            //Invalid output of source operator
            CPPUNIT_ASSERT_THROW(m_stream->connect(op1, 100, op2, TestOperator::INPUT_1), WrongArgument);
            //Invalid input of target operator
            CPPUNIT_ASSERT_THROW(m_stream->connect(op1, TestOperator::OUTPUT_1, op2, 100), WrongArgument);
            
            //Invalid state of the stream (not INACTIVE)
            m_stream->start();
            CPPUNIT_ASSERT_THROW(m_stream->connect(op1, TestOperator::OUTPUT_1, op2, TestOperator::INPUT_1), WrongState);
            m_stream->stop();
            m_stream->join();
            
            //Valid input parameters and stream INACTIVE
            CPPUNIT_ASSERT_NO_THROW(m_stream->connect(op1, TestOperator::OUTPUT_1, op2, TestOperator::INPUT_1));
            
            // uninitialized operators
            m_stream->deinitializeOperator(op1);
            CPPUNIT_ASSERT_THROW(m_stream->connect(op1, TestOperator::OUTPUT_1, op2, TestOperator::INPUT_1), WrongState);
            m_stream->initializeOperator(op1);
            m_stream->deinitializeOperator(op2);
            CPPUNIT_ASSERT_THROW(m_stream->connect(op1, TestOperator::OUTPUT_1, op2, TestOperator::INPUT_1), WrongState);
        }

        void StreamTest::testDisconnect()
        {
            std::vector<Operator*> ops = m_stream->operators();   
            Operator* op3 = new Operator(new TestOperator);        
            
            //Invalid target operator (operator op3 not known by the stream)
            CPPUNIT_ASSERT_THROW(m_stream->disconnect(op3, TestOperator::INPUT_1), WrongArgument);
            //Invalid input of target operator
            CPPUNIT_ASSERT_THROW(m_stream->disconnect(ops[1], 100), WrongArgument);
            
            //Invalid state of the stream (not INACTIVE)
            m_stream->start();
            CPPUNIT_ASSERT_THROW(m_stream->disconnect(ops[1], TestOperator::INPUT_1), WrongState);
            m_stream->stop();
            m_stream->join();
            
            //Valid input parameters and stream INACTIVE
            CPPUNIT_ASSERT_NO_THROW(m_stream->disconnect(ops[1], TestOperator::INPUT_1));
            
            // uninitialized operator
            m_stream->deinitializeOperator(ops[1]);
            CPPUNIT_ASSERT_THROW(m_stream->disconnect(ops[1], TestOperator::INPUT_1), WrongState);
        }

        void StreamTest::testAddOperator()
        {       
            //Invalid input parameter (Null pointer)
            CPPUNIT_ASSERT_THROW(m_stream->addOperator(0), WrongArgument);
            
            //Invalid input parameter (operator already added to the stream)
            m_stream->addOperator(m_op1);
            CPPUNIT_ASSERT_THROW(m_stream->addOperator(m_op1), WrongArgument);
            
            //Invalid state of the stream (not INACTIVE)
            m_stream->start();
            CPPUNIT_ASSERT_THROW(m_stream->addOperator(m_op2), WrongState);
            m_stream->stop();
            m_stream->join();
                        
            //Valid input parameter and stream INACTIVE
            CPPUNIT_ASSERT_NO_THROW(m_stream->addOperator(m_op2));
            CPPUNIT_ASSERT_EQUAL((unsigned int)(6), (unsigned int)(m_stream->operators().size()));
        }
        
        void StreamTest::testRemoveOperator()
        { 
            Operator* op = m_stream->operators()[1];
            
            //Invalid input parameter (Null pointer)
            CPPUNIT_ASSERT_THROW(m_stream->removeOperator(0), WrongArgument);
            
            //Invalid input parameter (operator op not known by the stream)
            Operator* op1 = new Operator(m_op1);
            CPPUNIT_ASSERT_THROW(m_stream->removeOperator(op1), WrongArgument);
             
            //Invalid state of the stream (not INACTIVE)
            m_stream->start();
            CPPUNIT_ASSERT_THROW(m_stream->removeOperator(op), WrongState);
            m_stream->stop();
            m_stream->join();
            
            //Valid input parameter and stream INACTIVE           
            CPPUNIT_ASSERT_NO_THROW(m_stream->removeOperator(op));
            CPPUNIT_ASSERT_EQUAL((unsigned int)(3), (unsigned int)(m_stream->operators().size()));
            CPPUNIT_ASSERT_EQUAL((unsigned int)(2), (unsigned int)(m_stream->threads()[0]->inputSequence().size()));
            
            // remove uninitialized operator
            Operator* op2 = m_stream->addOperator(m_op2);
            CPPUNIT_ASSERT_NO_THROW(m_stream->removeOperator(op2));
            
            // delete removed operator
            delete op2;
        }
        
        void StreamTest::testInitializeOperator()
        {
            Operator* op = m_stream->operators()[1];
            
            // can not initialize operator which is not part of the stream 
            Operator* op3 = new Operator(new TestOperator);
            CPPUNIT_ASSERT_THROW(m_stream->initializeOperator(op3), WrongArgument);
            
            // can not initialize initialized operator
            CPPUNIT_ASSERT_THROW(m_stream->initializeOperator(op), WrongOperatorState);
             
            // can not initialize while stream is active
            Operator* op1 = m_stream->addOperator(m_op1);
            m_stream->start();
            CPPUNIT_ASSERT_THROW(m_stream->initializeOperator(op1), WrongState);
            m_stream->stop();
            m_stream->join();
            
            CPPUNIT_ASSERT_NO_THROW(m_stream->initializeOperator(op1));
            CPPUNIT_ASSERT_EQUAL(Operator::INITIALIZED, op1->status());
        }
        
        void StreamTest::testDeinitializeOperator()
        {
            Operator* op = m_stream->operators()[1];
            
            // deinitialize operator which is not part of the stream
            Operator* op3 = new Operator(new TestOperator);
            CPPUNIT_ASSERT_THROW(m_stream->deinitializeOperator(op3), WrongArgument);
            
            // deinitialize uninitialized operator
            m_stream->deinitializeOperator(op);
            CPPUNIT_ASSERT_NO_THROW(m_stream->deinitializeOperator(op));
             
            // can not deinitialize while stream is active
            Operator* op1 = m_stream->addOperator(m_op1);
            m_stream->initializeOperator(op1);
            m_stream->start();
            CPPUNIT_ASSERT_THROW(m_stream->deinitializeOperator(op1), WrongState);
            m_stream->stop();
            m_stream->join();
            
            CPPUNIT_ASSERT_NO_THROW(m_stream->deinitializeOperator(op1));
            CPPUNIT_ASSERT_EQUAL(Operator::NONE, op1->status());
        }
       
        void StreamTest::testAddThread()
        {
            Thread* thr = 0;
            CPPUNIT_ASSERT_NO_THROW(thr = m_stream->addThread());
            CPPUNIT_ASSERT_EQUAL((unsigned int)(3), (unsigned int)(m_stream->threads().size()));
            CPPUNIT_ASSERT_EQUAL(thr, m_stream->threads()[2]);
        }
        
        void StreamTest::testRemoveThread()
        {
            CPPUNIT_ASSERT_THROW(m_stream->removeThread(0), WrongArgument);
            
            Thread* thr = m_stream->threads()[0];
           
            CPPUNIT_ASSERT_NO_THROW(m_stream->removeThread(thr));
            CPPUNIT_ASSERT_EQUAL((unsigned int)(1), (unsigned int)(m_stream->threads().size()));
            
            CPPUNIT_ASSERT_THROW(m_stream->removeThread(thr), WrongArgument);
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
        
        void StreamTest::testDelay()
        {
            CPPUNIT_ASSERT_EQUAL((unsigned int)(0), m_stream->delay());
            m_stream->setDelay(100);
            CPPUNIT_ASSERT_EQUAL((unsigned int)(100), m_stream->delay());
        }
        
        void StreamTest::testStopDelay()
        {
            
            // set delay to 10 seconds
            m_stream->setDelay(10000);
            
            // start the threads
            m_stream->start();
            
            // set the input data
            Operator* op = m_stream->operators()[0];
            op->setInputData(TestOperator::INPUT_1, DataContainer(new None));
            op->setInputData(TestOperator::INPUT_2, DataContainer(new None));
            
            // wait a bit (the thread should be delayed)
            boost::this_thread::sleep(boost::posix_time::millisec(500));
            
            // this should happen immediately
            m_stream->stop();
            m_stream->join();
        }
    }
}