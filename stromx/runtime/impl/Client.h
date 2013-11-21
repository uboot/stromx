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

#ifndef STROMX_RUNTIME_IMPL_SERVER_H
#define STROMX_RUNTIME_IMPL_SERVER_H

#include <deque>
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include "stromx/runtime/DataContainer.h"

namespace stromx
{
    namespace runtime
    {
        class Factory;
        
        namespace impl
        {
            class Client
            {
            public:
                class NoConnection : public std::exception {};
                
                Client(const std::string & url, const std::string & port);
                
                const DataContainer receive(const Factory & factory);
                void stop();
                void join();
                
            private:
                const static std::string LINE_DELIMITER;
                
                void run();
                
                boost::asio::io_service m_ioService;
                boost::asio::ip::tcp::socket m_socket;
                boost::thread m_thread;
                bool m_isConnected;
                std::deque<DataContainer> m_deque;
            };
        }
    }
}

#endif // STROMX_RUNTIME_IMPL_SERVER_H
