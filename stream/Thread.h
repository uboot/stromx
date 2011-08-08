/* 
*  Copyright 2011 Matthias Fuchs
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

#ifndef STREAM_THREAD_H
#define STREAM_THREAD_H

#include "impl/Network.h"
#include "Node.h"

#include "impl/ThreadImpl.h"

namespace stream
{
    class Thread
    {    
        friend class Stream;
        
    public:
        enum Status
        {
            INACTIVE,
            ACTIVE,
            DEACTIVATING,
        };
        
        Thread(const Network* const network);
        
        const Status status() const { return Status(m_thread.status()); }
        
        const std::string & name() const { return m_name; }
        void setName(const std::string& name) { m_name = name; }
        const std::vector<Node> & nodeSequence() const { return m_nodeSequence; }
        
        void addNode(Operator* const op, const unsigned int inputId);
        void insertNode(const unsigned int position, Operator* const op, const unsigned int inputId);
        void removeNode(const unsigned int position);
               
    private:
        void start() { m_thread.start(); }
        void stop() { m_thread.stop(); }
        void join() { m_thread.join(); }
        
        ThreadImpl m_thread;
        std::string m_name;
        const Network* m_network;
        std::vector<Node> m_nodeSequence;
    };
}

#endif // STREAM_THREAD_H
