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
#include <boost/lexical_cast.hpp>

#include <stromx/runtime/Factory.h>
#include <stromx/runtime/Primitive.h>
#include <stromx/runtime/ReadAccess.h>
#include "stromx/runtime/impl/Client.h"

CPPUNIT_TEST_SUITE_REGISTRATION (stromx::runtime::ClientTest);

namespace stromx
{
namespace runtime
{
namespace
{

using namespace boost::asio;

void sendData(const unsigned int value)
{
    io_service ioService;
    ip::tcp::acceptor acceptor(ioService, ip::tcp::endpoint(ip::tcp::v4(), 49152));
    ip::tcp::socket socket(ioService);
    acceptor.accept(socket);
    
    std::string valueStr = boost::lexical_cast<std::string>(value);
    streambuf buf;
    std::ostream out(&buf);
    
    out << "0.1.0\r\n";
    out << "Runtime\r\n";
    out << "UInt32\r\n";
    out << "0.1.0\r\n";
    out << valueStr.length() << "\r\n";
    out << "0\r\n";
    out << valueStr;
    out << "\r\n";
    
    write(socket, buf);
}

}

void ClientTest::setUp()
{}

void ClientTest::testNoConnection()
{
    CPPUNIT_ASSERT_THROW(impl::Client("localhost", "49152"), 
                         impl::Client::NoConnection);
}

void ClientTest::testConnection()
{
    boost::thread t(sendData, 2);
    
    CPPUNIT_ASSERT_NO_THROW(impl::Client("localhost", "49152"));
    t.join();
}

void ClientTest::testReceive()
{
    boost::thread t(sendData, 2);
    
    impl::Client client("localhost", "49152");
    Factory factory;
    factory.registerData(new UInt32);
    DataContainer data = client.receive(factory);
    ReadAccess<UInt32> access(data);
    
    CPPUNIT_ASSERT_EQUAL(UInt32(2), access());
    t.join();
}

void ClientTest::tearDown()
{
    if (m_client)
    {
        m_client->stop();
        m_client->join();
    }
    
    delete m_client;
}

}
}
