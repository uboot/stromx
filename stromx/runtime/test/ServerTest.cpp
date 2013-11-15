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

#include "stromx/runtime/DataContainer.h"
#include "stromx/runtime/Primitive.h"
#include "stromx/runtime/impl/Server.h"
#include <boost/array.hpp>
#include <boost/asio.hpp>

CPPUNIT_TEST_SUITE_REGISTRATION (stromx::runtime::ServerTest);

namespace stromx
{
    namespace runtime
    {
        namespace 
        {
            using namespace boost::asio;
            
            void connectToServer(io_service & ioService, ip::tcp::socket & socket)
            {
                ip::tcp::resolver resolver(ioService);
                ip::tcp::resolver::query query("localhost", "49152");
                ip::tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);

                boost::asio::connect(socket, endpoint_iterator);
            }
            
        }
    
        void ServerTest::setUp()
        {
            m_server = new impl::Server(49152);
        }

        void ServerTest::tearDown()
        {
            m_server->stop();
            m_server->join();
            delete m_server;
        }
        
        void ServerTest::testConstructor()
        {
        }
        
        void ServerTest::testConnectFails()
        {
            using namespace boost::asio;
            
            io_service ioService;
            ip::tcp::socket socket(ioService);

            ip::tcp::resolver resolver(ioService);
            ip::tcp::resolver::query query("localhost", "50000");
            ip::tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);

            CPPUNIT_ASSERT_THROW(boost::asio::connect(socket, endpoint_iterator), boost::system::system_error);
        }
                
        void ServerTest::testConnect()
        {
            using namespace boost::asio;
            
            io_service ioService;
            ip::tcp::socket socket(ioService);

            ip::tcp::resolver resolver(ioService);
            ip::tcp::resolver::query query("localhost", "49152");
            ip::tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);

            CPPUNIT_ASSERT_NO_THROW(boost::asio::connect(socket, endpoint_iterator));
        }
        
        void ServerTest::testReceive()
        {
            using namespace boost::asio;
            
            io_service ioService;
            ip::tcp::socket socket(ioService);
            connectToServer(ioService, socket);
            
            DataContainer data(new UInt32(2));
            m_server->send(data);

            boost::array<char, 128> buf;
            std::fill(buf.begin(), buf.end(), 0);
            socket.read_some(buffer(buf));
            
            CPPUNIT_ASSERT_EQUAL(resultString(2), std::string(buf.data()));
        }
        
        const std::string ServerTest::resultString(const unsigned int value)
        {
            std::ostringstream out;
            out << "0.1.0\r\n";
            out << "Runtime\r\n";
            out << "UInt32\r\n";
            out << "0.1.0\r\n";
            out << "1\r\n";
            out << "0\r\n";
            out << value;
            
            return out.str();
        }
    }
}
