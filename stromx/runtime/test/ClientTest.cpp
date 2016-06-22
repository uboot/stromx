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

#include "stromx/runtime/test/ClientTest.h"

#include <cppunit/TestAssert.h>
#include <boost/archive/text_oarchive.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/thread.hpp>

#include "stromx/runtime/Factory.h"
#include "stromx/runtime/Primitive.h"
#include "stromx/runtime/ReadAccess.h"
#include "stromx/runtime/impl/Client.h"

CPPUNIT_TEST_SUITE_REGISTRATION (stromx::runtime::ClientTest);

namespace stromx
{
    namespace runtime
    {
        namespace
        {
            using namespace boost::asio;

            void startConnection()
            {
                io_service ioService;
                ip::tcp::acceptor acceptor(ioService, ip::tcp::endpoint(ip::tcp::v4(), 49152));
                ip::tcp::socket socket(ioService);
                acceptor.accept(socket);
            }

            void acceptConnections(ip::tcp::acceptor* acceptor, ip::tcp::socket* socket)
            {
                acceptor->accept(*socket);
            }

            void receiveData(stromx::runtime::impl::Client* client )
            {
                stromx::runtime::Factory factory;
                CPPUNIT_ASSERT_THROW(client->receive(factory), impl::Client::Stopped);
            } 
            
            void sendValue(ip::tcp::socket & socket, unsigned int value)
            {
                // serialize the header
                impl::SerializationHeader header;
                header.serverVersion = Version(STROMX_RUNTIME_VERSION_MAJOR, STROMX_RUNTIME_VERSION_MINOR, STROMX_RUNTIME_VERSION_PATCH);
                header.package = "runtime";
                header.type = "UInt32";
                header.version = Version(STROMX_RUNTIME_VERSION_MAJOR, STROMX_RUNTIME_VERSION_MINOR, STROMX_RUNTIME_VERSION_PATCH);
                std::ostringstream headerStream;
                boost::archive::text_oarchive headerArchive(headerStream);
                headerArchive << header;
                
                // serialize the data
                std::string headerData = headerStream.str();
                std::string textData = boost::lexical_cast<std::string>(value);;
                std::string fileData = "";
                
                // output the sizes of the buffers
                std::ostringstream sizesStream;
                sizesStream << std::setw(impl::SerializationHeader::NUM_SIZE_DIGITS) << std::hex << headerData.size();
                sizesStream << std::setw(impl::SerializationHeader::NUM_SIZE_DIGITS) << std::hex << textData.size();
                sizesStream << std::setw(impl::SerializationHeader::NUM_SIZE_DIGITS) << std::hex << fileData.size();
                
                // construct a buffer sequence
                std::vector<const_buffer> buffers;
                buffers.push_back(buffer(sizesStream.str()));
                buffers.push_back(buffer(headerData));
                buffers.push_back(buffer(textData));
                buffers.push_back(buffer(fileData));
                
                boost::asio::write(socket, buffers);
            }

            void sendData(const unsigned int value)
            {
                io_service ioService;
                ip::tcp::acceptor acceptor(ioService, ip::tcp::endpoint(ip::tcp::v4(), 49152));
                ip::tcp::socket socket(ioService);
                acceptor.accept(socket);
                
                sendValue(socket, value);
            }

            void sendMultipleData()
            {
                io_service ioService;
                ip::tcp::acceptor acceptor(ioService, ip::tcp::endpoint(ip::tcp::v4(), 49152));
                ip::tcp::socket socket(ioService);
                acceptor.accept(socket);
                
                sendValue(socket, 2);
                sendValue(socket, 3);
            } 
        }

        void ClientTest::setUp()
        {
            m_client = 0;
            
            // wait a bit to increase the chance that the requested socket is available
            boost::this_thread::sleep_for(boost::chrono::seconds(1));
        }

        void ClientTest::testNoConnection()
        {
            CPPUNIT_ASSERT_THROW(impl::Client("localhost", "49152"), 
                                 impl::Client::NoConnection);
        }

        void ClientTest::testConnection()
        {
            boost::thread t(startConnection);
            
            CPPUNIT_ASSERT_NO_THROW(m_client = new impl::Client("localhost", "49152"));
            t.join();
        }

        void ClientTest::testReceive()
        {
            boost::thread t(sendData, 2);
            
            m_client = new impl::Client("localhost", "49152");
            Factory factory;
            factory.registerData(new UInt32);
            DataContainer data = m_client->receive(factory);
            ReadAccess<UInt32> access(data);
            
            CPPUNIT_ASSERT_EQUAL(UInt32(2), access());
            t.join();
        }

        void ClientTest::testReceiveMultipleData()
        {
            boost::thread t(sendMultipleData);
            m_client = new impl::Client("localhost", "49152");
            Factory factory;
            factory.registerData(new UInt32);
            DataContainer data;
            
            data = m_client->receive(factory);
            ReadAccess<UInt32> access1(data);
            CPPUNIT_ASSERT_EQUAL(UInt32(2), access1());
            
            data = m_client->receive(factory);
            ReadAccess<UInt32> access2(data);
            CPPUNIT_ASSERT_EQUAL(UInt32(3), access2());
            t.join();
        }

        void ClientTest::testReceiveClosedConnection()
        {
            boost::thread t(startConnection);
            
            m_client = new impl::Client("localhost", "49152");
            Factory factory;
            CPPUNIT_ASSERT_THROW(m_client->receive(factory), impl::Client::NoConnection);
            
            t.join();
        }

        void ClientTest::testStop()
        {
            io_service ioService;
            ip::tcp::acceptor acceptor(ioService, ip::tcp::endpoint(ip::tcp::v4(), 49152));
            ip::tcp::socket socket(ioService);
            
            // wait for incoming connections
            boost::thread server(boost::bind(&acceptConnections, &acceptor, &socket));
            
            // connect to server
            m_client = new impl::Client("localhost", "49152");
            
            // request data from server a separate thread
            boost::thread client(boost::bind(&receiveData, m_client));
            
            // stop the client
            m_client->stop();
            
            // wait for its thread to finish
            client.join();
            
            // wait for the server
            server.join();
        }

        void ClientTest::testStopWithWait()
        {
            io_service ioService;
            ip::tcp::acceptor acceptor(ioService, ip::tcp::endpoint(ip::tcp::v4(), 49152));
            ip::tcp::socket socket(ioService);
            
            // wait for incoming connections
            boost::thread server(boost::bind(&acceptConnections, &acceptor, &socket));
            
            // connect to server
            m_client = new impl::Client("localhost", "49152");
            
            // request data from server a separate thread
            boost::thread client(boost::bind(&receiveData, m_client));
            boost::this_thread::sleep_for(boost::chrono::seconds(1));
            
            // stop the client
            m_client->stop();
            
            // wait for its thread to finish
            client.join();
            
            // wait for the server
            server.join();
        }

        void ClientTest::tearDown()
        {            
            delete m_client;
        }
    }
}
