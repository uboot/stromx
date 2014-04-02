/* 
*  Copyright 2013 Matthias Fuchs
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
#include <boost/thread.hpp>

#include "stromx/runtime/Factory.h"
#include "stromx/runtime/OperatorTester.h"
#include "stromx/runtime/ReadAccess.h"
#include "stromx/runtime/Receive.h"
#include "stromx/runtime/Send.h"

#include "stromx/runtime/test/SendReceiveTest.h"

CPPUNIT_TEST_SUITE_REGISTRATION (stromx::runtime::SendReceiveTest);

namespace stromx
{
    using namespace runtime;

    namespace runtime
    {
        namespace
        {
            void getOutputData(Operator* op)
            {
                CPPUNIT_ASSERT_THROW(op->getOutputData(Receive::OUTPUT), Interrupt);
            }
        }
        
        void SendReceiveTest::setUp ( void )
        {
            m_factory = new Factory();
            m_send = new runtime::OperatorTester(new Send());
            m_receive = new runtime::OperatorTester(new Receive());
            m_receive->setFactory(m_factory);
            
            m_send->initialize();
            m_receive->initialize();
            
            m_send->setParameter(Send::PORT, UInt16(49152));
            m_receive->setParameter(Receive::URL, String("localhost"));
            m_receive->setParameter(Receive::PORT, UInt16(49152));
            
            m_send->activate();
            m_receive->activate();
        }
        
        void SendReceiveTest::testSendUInt32()
        {
            DataContainer in(new UInt32(2));
            m_send->setInputData(Send::INPUT, in);
            
            m_factory->registerData(new UInt32);
            DataContainer out = m_receive->getOutputData(Receive::OUTPUT);
            ReadAccess<UInt32> access(out);
            
            CPPUNIT_ASSERT_EQUAL(UInt32(2), access());
        }   
        
        void SendReceiveTest::testInterrupt()
        {
            boost::thread receive(boost::bind(&getOutputData, m_receive));
            boost::this_thread::sleep(boost::posix_time::seconds(1));
            
            receive.interrupt();
            m_receive->interrupt();
            receive.join();
        }
        
        void SendReceiveTest::tearDown ( void )
        {
            delete m_factory;
            delete m_send;
            delete m_receive;
        }
    }
}
