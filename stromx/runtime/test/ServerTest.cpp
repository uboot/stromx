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

#include "stromx/runtime/test/ServerTest.h"

#include <cppunit/TestAssert.h>

#include <boost/asio.hpp>
#include <boost/thread.hpp>

#include "stromx/runtime/OperatorTester.h"
#include "stromx/runtime/Server.h"

CPPUNIT_TEST_SUITE_REGISTRATION (stromx::runtime::ServerTest);

namespace 
{
}

namespace stromx
{
    namespace runtime
    {
        namespace 
        {
            void startServer(Operator* op)
            {
                DataContainer data(new UInt32(2));
                op->setInputData(Server::INPUT, data);
            }
        }
    
        void ServerTest::setUp()
        {
            m_operator = new OperatorTester(new Server());
            
            DataContainer data(new UInt32(1));
            m_operator->initialize();
            m_operator->activate();
            m_operator->setInputData(Server::INPUT, data);
        }

        void ServerTest::testTransmit()
        {
            boost::thread t(startServer, m_operator);
            boost::this_thread::sleep(boost::posix_time::microsec(300));
            
            using namespace boost::asio;
            
            io_service io_service;
            ip::tcp::resolver resolver(io_service);
            ip::tcp::resolver::query query("localhost", "49152");
            ip::tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);
        
            ip::tcp::socket socket(io_service);
            connect(socket, endpoint_iterator);
            
            boost::array<char, 128> buf;
            size_t len = socket.read_some(boost::asio::buffer(buf));
            t.join();
            
//             CPPUNIT_ASSERT_EQUAL(size_t(38), len);
        }

        void ServerTest::tearDown()
        {
            delete m_operator;
        }
    }
}
