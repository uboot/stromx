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

#include "Client.h"

#include <boost/archive/text_iarchive.hpp>
#include <boost/bind.hpp>
#include "stromx/runtime/Data.h"
#include "stromx/runtime/AbstractFactory.h"
#include "stromx/runtime/InputProvider.h"
#include "stromx/runtime/Version.h"

using namespace boost::asio;

namespace
{
    class StreamInput : public stromx::runtime::InputProvider
    {            
    public:
        StreamInput(std::string & textData, std::string & fileData)
          : m_textStream(textData),
            m_fileStream(fileData),
            m_hasFile(fileData.size() != 0)
        {}
        
        std::istream & text()
        {
            return m_textStream;
        }
        
        bool hasFile() const
        {
            return m_hasFile;
        }
        
        std::istream & openFile(const OpenMode /*mode*/)
        {
            return m_fileStream;
        }
        
        std::istream & file()
        {
            return m_fileStream;
        }
        
    private:
        std::istringstream m_textStream;
        std::istringstream m_fileStream;
        bool m_hasFile;
    };
    
    unsigned int sizeFromBuffer(const char* data)
    {
        std::istringstream is(std::string(data, stromx::runtime::impl::SerializationHeader::NUM_SIZE_DIGITS));
        unsigned int size;
        is >> std::hex >> size;
        return size;
    }
}

namespace stromx
{
    namespace runtime
    {
        namespace impl
        {            
            Client::Client(const std::string& url, const std::string& port)
              : m_socket(m_ioService)
            {
                try
                {
                    ip::tcp::resolver resolver(m_ioService);
                    ip::tcp::resolver::query query(url, port);
                    ip::tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);

                    boost::asio::connect(m_socket, endpoint_iterator);
                }
                catch (boost::system::system_error &)
                {
                    throw NoConnection();
                }
            }
            
            Client::~Client()
            {
                stop();
            }
            
            const DataContainer Client::receive(const AbstractFactory& factory)
            {
                m_ioService.reset();
                m_error.clear();
                asyncReceive();
                m_ioService.run();
                
                if (m_error)
                    throw NoConnection();
                
                return deserializeData(factory);
            }

            void Client::stop()
            {
                m_ioService.stop();
            }
            
            void Client::asyncReceive()
            {
                std::vector<mutable_buffer> sizeBuffers;
                sizeBuffers.push_back(buffer(m_headerSizeBuffer));
                sizeBuffers.push_back(buffer(m_textSizeBuffer));
                sizeBuffers.push_back(buffer(m_fileSizeBuffer));
                
                async_read(m_socket, sizeBuffers, boost::bind(&Client::handleHeaderRead, this,
                                                              placeholders::error, placeholders::bytes_transferred)); 
            }
            
            void Client::handleHeaderRead(const boost::system::error_code& error, size_t /*bytes_transferred*/)
            {
                if (error)
                {
                    m_error = error;
                    return;
                }
                
                unsigned int headerSize = sizeFromBuffer(m_headerSizeBuffer.data());
                unsigned int textSize = sizeFromBuffer(m_textSizeBuffer.data());
                unsigned int fileSize = sizeFromBuffer(m_fileSizeBuffer.data());
                
                m_headerData.resize(headerSize);
                m_textData.resize(textSize);
                m_fileData.resize(fileSize);
                
                std::vector<mutable_buffer> dataBuffers;
                dataBuffers.push_back(buffer(m_headerData));
                dataBuffers.push_back(buffer(m_textData));
                dataBuffers.push_back(buffer(m_fileData));
                
                async_read(m_socket, dataBuffers, boost::bind(&Client::handleDataRead, this,
                                                              placeholders::error, placeholders::bytes_transferred)); 
            }
            
            void Client::handleDataRead(const boost::system::error_code& error, size_t /*bytes_transferred*/)
            {
                if (error)
                    m_error = error;
            }
            
            const DataContainer Client::deserializeData(const AbstractFactory& factory)
            {
                SerializationHeader header;
                std::istringstream headerStream(std::string(m_headerData.data(), m_headerData.size()));
                boost::archive::text_iarchive headerArchive(headerStream);
                headerArchive >> header;
                
                // create the data object and store in a container
                Data* data = factory.newData(header.package, header.type);
                DataContainer container(data);
                
                std::string textString(m_textData.data(), m_textData.size());
                std::string fileString(m_fileData.data(), m_fileData.size());
                
                StreamInput input(textString, fileString);
                data->deserialize(input, header.version);
                
                return container;
            }
        }
    }
}
