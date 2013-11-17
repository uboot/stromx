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
            const std::string Connection::LINE_DELIMITER("\r\n");
            const Version Server::VERSION(0, 1, 0);

            Connection::Connection(Server* server, boost::asio::io_service& ioService)
                  : m_server(server),
                    m_socket(ioService),
                    m_active(false)
            {}
            
            void Connection::send(const DataContainer & data)
            {
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

                try
                {
                // TODO: write data asynchronously to make sure the
                // the server thread is not blocked but executes the asio
                // event loop most of the time
                boost::asio::write(m_socket, buffer);
                boost::asio::write(m_socket, output.textBuffer());
                boost::asio::write(m_socket, output.fileBuffer());

//                         boost::asio::async_write(m_socket, buffer, &handleWrite);
//                         boost::asio::async_write(m_socket, output.textBuffer(), &handleWrite);
//                         boost::asio::async_write(m_socket, output.fileBuffer(), &handleWrite);
                }
                catch(boost::system::system_error& exc)
                {
                    // in case the client disconnected (safely) remove the connection 
                    // from the server and close it
                    m_server->removeConnection(this);
                    delete this;
                    return;
                }
                
                m_server->setConnectionActive(this, false);
            }     

            Server::Server(unsigned int port)
              : m_acceptor(m_ioService, ip::tcp::endpoint(ip::tcp::v4(), port)),
                m_thread(boost::bind(&Server::run, this))
            {}
            
            Server::~Server()
            {
                for (std::set<Connection*>::const_iterator iter = m_connections.begin();
                    iter != m_connections.end(); ++iter)
                {
                    delete *iter;
                }
            }
            
            void Server::send(const DataContainer& data)
            {
                boost::unique_lock<boost::mutex> l(m_mutex);
                
                while (true)
                {
                    for (std::set<Connection*>::const_iterator iter = m_connections.begin();
                        iter != m_connections.end(); ++iter)
                    {
                        if ((*iter)->active() == false)
                        {
                            // in case an inactive connection was found return
                            (*iter)->setActive(true);
                            m_ioService.post(boost::bind(&Connection::send, *iter, data));
                            return;
                        }
                    }
                    
                    m_cond.wait(l);
                }
            }
            
            void Server::stop()
            {
                m_ioService.stop();
            }
            
            void Server::join()
            {
                m_thread.join();
            }
            
            void Server::run()
            {
                startAccept();
                m_ioService.run();
            }
            
            void Server::startAccept()
            {
                Connection* connection = new Connection(this, m_ioService);
                
                m_acceptor.async_accept(connection->socket(),
                                        boost::bind(&Server::handleAccept, this,
                                                    connection, boost::asio::placeholders::error));
            }
            
            void Server::handleAccept(Connection* connection, const boost::system::error_code& /*error*/)
            {
                {
                    boost::lock_guard<boost::mutex> l(m_mutex);
                    m_connections.insert(connection);
                    m_cond.notify_all();
                }
                
                startAccept();
            }
            
            void Server::removeConnection(Connection* connection)
            {
                boost::lock_guard<boost::mutex> l(m_mutex);
                m_connections.erase(connection);
                m_cond.notify_all();
            }
            
            unsigned int Server::numConnections()
            {
                boost::lock_guard<boost::mutex> l(m_mutex);
                return m_connections.size();
            }
            
            void Server::setConnectionActive(Connection* connection, const bool isActive)
            {
                boost::lock_guard<boost::mutex> l(m_mutex);
                connection->setActive(isActive);
                m_cond.notify_all();
            }
            
            void Server::waitForNumConnections(const unsigned int numConnections)
            {
                boost::unique_lock<boost::mutex> l(m_mutex);
                
                while (m_connections.size() != numConnections)
                    m_cond.wait(l);
            }
        }
    }
}

