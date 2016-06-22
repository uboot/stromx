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

#include <boost/archive/text_iarchive.hpp>
#include <boost/array.hpp>
#include <boost/asio.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/thread.hpp>

#include "stromx/runtime/DataContainer.h"
#include "stromx/runtime/Primitive.h"
#include <stromx/runtime/impl/SerializationHeader.h>
#include "stromx/runtime/impl/Server.h"

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
            
            unsigned int sizeFromBuffer(const char* data)
            {
                std::istringstream is(std::string(data, impl::SerializationHeader::NUM_SIZE_DIGITS));
                unsigned int size;
                is >> std::hex >> size;
                return size;
            }
            
            const impl::SerializationHeader receive(ip::tcp::socket& socket, std::string & text, std::string & file)
            {
                boost::array<char, impl::SerializationHeader::NUM_SIZE_DIGITS> headerSizeBuffer, textSizeBuffer, fileSizeBuffer;
                
                std::vector<mutable_buffer> sizeBuffers;
                sizeBuffers.push_back(buffer(headerSizeBuffer));
                sizeBuffers.push_back(buffer(textSizeBuffer));
                sizeBuffers.push_back(buffer(fileSizeBuffer));
                
                read(socket, sizeBuffers);
                
                unsigned int headerSize = sizeFromBuffer(headerSizeBuffer.data());
                unsigned int textSize = sizeFromBuffer(textSizeBuffer.data());
                unsigned int fileSize = sizeFromBuffer(fileSizeBuffer.data());
                
                std::vector<char> headerData(headerSize);
                std::vector<char> textData(textSize);
                std::vector<char> fileData(fileSize);
                
                std::vector<mutable_buffer> dataBuffers;
                dataBuffers.push_back(buffer(headerData));
                dataBuffers.push_back(buffer(textData));
                dataBuffers.push_back(buffer(fileData));
                
                read(socket, dataBuffers);
                
                text = std::string(textData.data(), textData.size());
                file = std::string(fileData.data(), fileData.size());
                
                impl::SerializationHeader header;
                std::istringstream headerStream(std::string(headerData.data(), headerData.size()));
                boost::archive::text_iarchive headerArchive(headerStream);
                headerArchive >> header;
                
                return header;
            }
            
            const impl::SerializationHeader uintHeader()
            {
                impl::SerializationHeader header;
                
                header.serverVersion = Version(STROMX_RUNTIME_VERSION_MAJOR, STROMX_RUNTIME_VERSION_MINOR, STROMX_RUNTIME_VERSION_PATCH);
                header.package = "runtime";
                header.type = "UInt32";
                header.version = Version(STROMX_RUNTIME_VERSION_MAJOR, STROMX_RUNTIME_VERSION_MINOR, STROMX_RUNTIME_VERSION_PATCH);
                
                return header;
            }
        }
    
        void ServerTest::setUp()
        {
            m_server = new impl::Server(49152);
            
            // wait a bit to increase the chance that the requested socket is available
            boost::this_thread::sleep_for(boost::chrono::seconds(1));
        }

        void ServerTest::tearDown()
        {
            if (m_server)
            {
                m_server->stop();
                m_server->join();
            }
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
            
            std::string text;
            std::string file;
            impl::SerializationHeader header = receive(socket, text, file);
            
            CPPUNIT_ASSERT_EQUAL(uintHeader(), header);
            CPPUNIT_ASSERT_EQUAL(std::string("2"), text);
            CPPUNIT_ASSERT_EQUAL(std::string(), file);
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
            
            std::string text;
            std::string file;
            impl::SerializationHeader header;
            
            header = receive(socket, text, file);
            CPPUNIT_ASSERT_EQUAL(uintHeader(), header);
            CPPUNIT_ASSERT_EQUAL(std::string("2"), text);
            CPPUNIT_ASSERT_EQUAL(std::string(), file);
            
            header = receive(socket, text, file);
            CPPUNIT_ASSERT_EQUAL(uintHeader(), header);
            CPPUNIT_ASSERT_EQUAL(std::string("10"), text);
            CPPUNIT_ASSERT_EQUAL(std::string(), file);
        }
    }
}
