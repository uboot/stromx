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

#include <boost/archive/text_oarchive.hpp>
#include <boost/bind.hpp>
#include "stromx/runtime/impl/SerializationHeader.h"
#include "stromx/runtime/OutputProvider.h"
#include "stromx/runtime/ReadAccess.h"

using namespace boost::asio;

namespace
{
    class StreamOutput : public stromx::runtime::OutputProvider
    {
    public:
        StreamOutput()
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
        
        std::string textData() const { return m_textStream.str(); }
        std::string fileData() const { return m_fileStream.str(); }
        
    private:
        std::ostringstream m_textStream;
        std::ostringstream m_fileStream;
    };
}

namespace stromx
{
    namespace runtime
    {
        namespace impl
        {            
            const Version Server::VERSION(0, 1, 0);

            Connection::Connection(Server* server, boost::asio::io_service& ioService)
              : m_server(server),
                m_socket(ioService),
                m_active(false)
            {}
            
            void Connection::send(const DataContainer & data)
            {
                ReadAccess<> access(data);
                
                // serialize the header
                SerializationHeader header;
                header.serverVersion = Server::VERSION;
                header.package = access().package();
                header.type = access().type();
                header.version = access().version();
                std::ostringstream headerStream;
                boost::archive::text_oarchive headerArchive(headerStream);
                headerArchive << header;
                
                // serialize the data
                StreamOutput output;
                access().serialize(output);
                
                std::string headerData = headerStream.str();
                std::string textData = output.textData();
                std::string fileData = output.fileData();
                
                // output the sizes of the buffers
                std::ostringstream sizesStream;
                sizesStream << std::setw(SerializationHeader::NUM_SIZE_DIGITS) << std::hex << headerData.size();
                sizesStream << std::setw(SerializationHeader::NUM_SIZE_DIGITS) << std::hex << textData.size();
                sizesStream << std::setw(SerializationHeader::NUM_SIZE_DIGITS) << std::hex << fileData.size();
                
                // construct a buffer sequence
                std::vector<const_buffer> buffers;
                buffers.push_back(buffer(sizesStream.str()));
                buffers.push_back(buffer(headerData));
                buffers.push_back(buffer(textData));
                buffers.push_back(buffer(fileData));
                
                boost::asio::async_write(m_socket, buffers, boost::bind(&Connection::handleWrite, this, 
                                                                        placeholders::error, placeholders::bytes_transferred));
                
            } 
            
            void Connection::handleWrite(const boost::system::error_code& error, size_t bytes_transferred)
            {
                if (error)
                    m_server->removeConnection(this);
                else
                    m_server->setConnectionActive(this, false);
            }

            Server::Server(unsigned int port)
              : m_acceptor(m_ioService, ip::tcp::endpoint(ip::tcp::v4(), port)),
                m_thread(boost::bind(&Server::run, this))
            {}
            
            Server::~Server()
            {
                stop();
                
                for (std::set<Connection*>::const_iterator iter = m_connections.begin();
                    iter != m_connections.end(); ++iter)
                {
                    delete *iter;
                }
            }
            
            void Server::send(const DataContainer& data)
            {
                Connection* connection = 0;
                
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
                                connection = *iter;
                                connection->setActive(true);
                                break;
                            }
                        }
                        
                        if (! connection)
                            m_cond.wait(l);
                        else
                            break;
                    }
                }
                
                connection->send(data);
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
                delete connection;
                
                m_cond.notify_all();
            }
            
            unsigned int Server::numConnections() const
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

