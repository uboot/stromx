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
#include <boost/lexical_cast.hpp>
#include <boost/thread.hpp>

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
            CPPUNIT_ASSERT_EQUAL((unsigned int)(0), m_server->numConnections());
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
            CPPUNIT_ASSERT_EQUAL((unsigned int)(0), m_server->numConnections());
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
            
            m_server->waitForNumConnections(1);
            CPPUNIT_ASSERT_EQUAL((unsigned int)(1), m_server->numConnections());
        }
        
        void ServerTest::testConstructorFails()
        {
            CPPUNIT_ASSERT_THROW(impl::Server(49152), boost::system::system_error);
        }
        
        void ServerTest::testReceive()
        {
            using namespace boost::asio;
            
            io_service ioService;
            ip::tcp::socket socket(ioService);
            connectToServer(ioService, socket);
            
            DataContainer data(new UInt32(2));
            m_server->send(data);
            
            std::string received = receiveString(socket, resultString(2).length());
            CPPUNIT_ASSERT_EQUAL(resultString(2), received);
        }
        
        void ServerTest::testClientDisconnects()
        {
            using namespace boost::asio;
            
            io_service ioService;
            ip::tcp::socket socket(ioService);
            connectToServer(ioService, socket);
            
            socket.close();
            DataContainer data(new UInt32(2));
            m_server->send(data);
            
            m_server->waitForNumConnections(0);
            CPPUNIT_ASSERT_EQUAL((unsigned int)(0), m_server->numConnections());
        }
        
        void ServerTest::testReceiveMultipleData()
        {
            using namespace boost::asio;
            
            io_service ioService;
            ip::tcp::socket socket(ioService);
            connectToServer(ioService, socket);
            
            DataContainer data2(new UInt32(2));
            DataContainer data10(new UInt32(10));
            
            m_server->send(data2);
            m_server->send(data10);
            std::string received2 = receiveString(socket, resultString(2).length());
            std::string received10 = receiveString(socket, resultString(10).length());
            
            CPPUNIT_ASSERT_EQUAL(resultString(2), received2);
            CPPUNIT_ASSERT_EQUAL(resultString(10), received10);
        }
        
        const std::string ServerTest::receiveString(ip::tcp::socket& socket, const unsigned int length)
        {
            boost::array<char, 128> buf;
            read(socket, buffer(buf), transfer_exactly(length));
            return std::string(buf.data());
        }
        
        const std::string ServerTest::resultString(const unsigned int value)
        {
            std::string valueStr = boost::lexical_cast<std::string>(value);
            
            std::ostringstream out;
            out << "0.1.0\r\n";
            out << "Runtime\r\n";
            out << "UInt32\r\n";
            out << "0.1.0\r\n";
            out << valueStr.length() << "\r\n";
            out << "0\r\n";
            out << valueStr;
            
            return out.str();
        }
    }
}
