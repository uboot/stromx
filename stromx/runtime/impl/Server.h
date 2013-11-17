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

#ifndef STROMX_RUNTIME_SERVER_H
#define STROMX_RUNTIME_SERVER_H

#include <deque>
#include <set>
#include <boost/asio.hpp>
#include <boost/thread.hpp>

#include "stromx/runtime/DataContainer.h"
#include "stromx/runtime/Version.h"

namespace stromx
{
    namespace runtime
    {
        class ServerTest; 
        
        namespace impl
        {
            class Server;

            class Connection
            {
            public:
                
                Connection(Server* server, boost::asio::io_service& ioService);

                bool active() const { return m_active; }
                void setActive(const bool isActive) { m_active = isActive; }
                boost::asio::ip::tcp::socket& socket() { return m_socket; }
                void send(const DataContainer & data);

            private:
                const static std::string LINE_DELIMITER;
                
                static void handleWrite(const boost::system::error_code& /*error*/,
                    size_t /*bytes_transferred*/) 
                {}

                Server* m_server;
                boost::asio::ip::tcp::socket m_socket;
                bool m_active;
            };
            
            class Server
            {
                friend class Connection;
                friend class stromx::runtime::ServerTest;
                
            public:
                static const Version VERSION;
                
                Server(const unsigned int port);
                ~Server();
                
                unsigned int numConnections();
                
                void send(const DataContainer & data);
                void stop();
                void join();
                
            private:
                void run();
                void startAccept();
                void handleAccept(Connection* connection, const boost::system::error_code & error);
                void setConnectionActive(Connection* connection, const bool isActive);
                void removeConnection(Connection* connection);
                void waitForNumConnections(const unsigned int numConnections);
                
                std::set<Connection*> m_connections;
                boost::asio::io_service m_ioService;
                boost::asio::ip::tcp::acceptor m_acceptor;
                boost::thread m_thread;
                boost::mutex m_mutex;
                boost::condition_variable m_cond;
            };
        }
    }
}

#endif // STROMX_RUNTIME_SERVER_H
