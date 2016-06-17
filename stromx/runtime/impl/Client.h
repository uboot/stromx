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
#include <boost/array.hpp>
#include <boost/asio.hpp>
#include <boost/thread.hpp>

#include "stromx/runtime/impl/SerializationHeader.h"
#include "stromx/runtime/DataContainer.h"

namespace stromx
{
    namespace runtime
    {
        class AbstractFactory;
        
        namespace impl
        {
            class Client
            {
        public:
                class NoConnection : public std::exception {};
                class Stopped : public std::exception {};
                
                Client(const std::string & url, const std::string & port);
                ~Client();
                
                const DataContainer receive(const AbstractFactory & factory);
                void stop();
                
            private:
                typedef boost::lock_guard<boost::mutex> lock_t;
                
                void run();
                void asyncReceive();
                void handleHeaderRead(const boost::system::error_code& error, size_t bytes_transferred);
                void handleDataRead(const boost::system::error_code& error, size_t bytes_transferred);
                const DataContainer deserializeData(const AbstractFactory& factory);
                
                boost::asio::io_service m_ioService;
                boost::asio::ip::tcp::socket m_socket;
                boost::system::error_code m_error;
                boost::mutex m_mutex;
                bool m_stopped;
                
                boost::array<char, impl::SerializationHeader::NUM_SIZE_DIGITS> m_headerSizeBuffer;
                boost::array<char, impl::SerializationHeader::NUM_SIZE_DIGITS> m_textSizeBuffer;
                boost::array<char, impl::SerializationHeader::NUM_SIZE_DIGITS> m_fileSizeBuffer;
                
                std::vector<char> m_headerData;
                std::vector<char> m_textData;
                std::vector<char> m_fileData;
            };
        }
    }
}

#endif // STROMX_RUNTIME_IMPL_SERVER_H
