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
#include <boost/asio.hpp>
#include <boost/thread.hpp>

#include "stromx/runtime/DataContainer.h"
#include "stromx/runtime/Version.h"

namespace stromx
{
    namespace runtime
    {
        namespace impl
        {
            class Connection;

            class Server
            {
                friend class Connection;
                
            public:
                static const Version VERSION;
                
                Server(const unsigned int port);
                
                void send(const DataContainer & data);
                void stop();
                void join();
                
            private:
                void startAccept();
                void handleAccept(Connection* connection, 
                                  const boost::system::error_code & error);
                const DataContainer waitForData();
                
                boost::asio::io_service m_ioService;
                boost::asio::ip::tcp::acceptor m_acceptor;
                boost::thread m_thread;
                boost::mutex m_mutex;
                boost::condition_variable m_cond;
                std::deque<DataContainer> m_queue;
            };
        }
    }
}

#endif // STROMX_RUNTIME_SERVER_H
