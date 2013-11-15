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
#include "stromx/runtime/OutputProvider.h"
#include "stromx/runtime/ReadAccess.h"

using namespace boost::asio;

namespace
{
    class StreamOutput : public stromx::runtime::OutputProvider
    {
    public:
        StreamOutput()
          : m_textStream(&m_textBuffer),
            m_fileStream(&m_fileBuffer)
        {}
        
        std::ostream & text ()
        {
            return m_textStream;
        }
        
        std::ostream & openFile (const std::string &/*ext*/, const OpenMode /*mode*/)
        {
            return m_fileStream;
        }
        
        std::ostream & file ()
        {
            return m_fileStream;
        }
        
        boost::asio::streambuf & textBuffer() { return m_textBuffer; }
        boost::asio::streambuf & fileBuffer() { return m_fileBuffer; }
        
    private:
        boost::asio::streambuf m_textBuffer;
        boost::asio::streambuf m_fileBuffer;
        
        std::ostream m_textStream;
        std::ostream m_fileStream;
    };
}

namespace stromx
{
    namespace runtime
    {
        namespace impl
        {            
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
                    DataContainer data;
                    
                    // wait for new data
                    try
                    {
                        data = m_server->waitForData();
                    }
                    catch(boost::thread_interrupted&)
                    {
                        // return if the server thread was signalled to stop
                        return;
                    }
                    
                    ReadAccess<> access(data);
                    
                    StreamOutput output;
                    access().serialize(output);
                    
                    boost::asio::streambuf buffer;
                    std::ostream dataStream(&buffer);
                    dataStream << Server::VERSION << LINE_DELIMITER;
                    dataStream << access().package() << LINE_DELIMITER;
                    dataStream << access().type() << LINE_DELIMITER;
                    dataStream << access().version() << LINE_DELIMITER;
                    dataStream << output.textBuffer().size() << LINE_DELIMITER;
                    dataStream << output.fileBuffer().size() << LINE_DELIMITER;

                    boost::asio::write(m_socket, buffer);
                    boost::asio::write(m_socket, output.textBuffer());
                    boost::asio::write(m_socket, output.fileBuffer());

//                     boost::asio::async_write(m_socket, boost::asio::buffer(message),
//                         boost::bind(&Connection::handleWrite, this,
//                         placeholders::error,
//                         placeholders::bytes_transferred));
                    
                    delete this;
                }

            private:
                const static std::string LINE_DELIMITER;
                
                void handleWrite(const boost::system::error_code& /*error*/,
                    size_t /*bytes_transferred*/)
                {
//                     delete this;
                }

                Server* m_server;
                ip::tcp::socket m_socket;
            };
            
            const std::string Connection::LINE_DELIMITER("\r\n");
            const Version Server::VERSION(0, 1, 0);

            Server::Server(unsigned int port)
              : m_acceptor(m_ioService, ip::tcp::endpoint(ip::tcp::v4(), port)),
                m_thread(boost::bind(&Server::startAccept, this))
            {}
            
            void Server::send(const DataContainer& data)
            {
                boost::lock_guard<boost::mutex> l(m_mutex);
                
                m_queue.push_front(data);
                m_cond.notify_all();
            }
            
            void Server::stop()
            {
                m_ioService.stop();
                m_thread.interrupt();
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
            
            const DataContainer Server::waitForData()
            {
                boost::unique_lock<boost::mutex> l(m_mutex);
                
                while (m_queue.empty())
                    m_cond.wait(l);
                
                DataContainer data = m_queue.back();
                m_queue.pop_back();
                
                return data;
            }
        }
    }
}

