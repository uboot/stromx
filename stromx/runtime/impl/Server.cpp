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

#include "Server.h"

#include <boost/bind.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/graph/graph_concepts.hpp>

using namespace boost::asio;

namespace stromx
{
    namespace runtime
    {
        namespace impl
        {
            class Server;
            
            class Connection
            {
            public:
                Connection(Server* server, io_service& ioService)
                  : m_server(server),
                    m_socket(ioService)
                {
                }

                ip::tcp::socket& socket()
                {
                    return m_socket;
                }

                void start()
                {
                    std::string message = "test";

                    boost::asio::async_write(m_socket, boost::asio::buffer(message),
                        boost::bind(&Connection::handleWrite, this,
                        placeholders::error,
                        placeholders::bytes_transferred));
                }

            private:
                void handleWrite(const boost::system::error_code& /*error*/,
                    size_t /*bytes_transferred*/)
                {
                    delete this;
                }

                Server* m_server;
                ip::tcp::socket m_socket;
            };

            Server::Server(unsigned int port)
              : m_acceptor(m_ioService, ip::tcp::endpoint(ip::tcp::v4(), port)),
                m_thread(boost::bind(&Server::startAccept, this))
            {}
            
            void Server::send(const DataContainer& data)
            {

            }
            
            void Server::stop()
            {
                m_ioService.stop();
            }
            
            void Server::join()
            {
                m_thread.join();
            }
            
            void Server::startAccept()
            {
                Connection* connection = new Connection(this, m_ioService);
                m_acceptor.async_accept(connection->socket(),
                                        boost::bind(&Server::handleAccept, this, 
                                                    connection, placeholders::error));
                
                m_ioService.run();
            }
            
            void Server::handleAccept(Connection* connection, 
                                      const boost::system::error_code& error)
            {
                if (! error)
                    connection->start();
                
                startAccept();
            }
        }
    }
}

