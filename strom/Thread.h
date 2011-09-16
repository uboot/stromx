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

#include "Node.h"

#include <string>
#include <vector>

namespace strom
{
    namespace impl
    {
        class Network;
        class ThreadImpl;
    }
    
    class Thread
    {    
        friend class Stream;
        friend class ThreadTest;
        
    public:
        enum Status
        {
            INACTIVE,
            ACTIVE,
            DEACTIVATING,
        };
        
        virtual ~Thread();
        
        const Status status() const;
        
        const std::string & name() const;
        void setName(const std::string& name);
        const std::vector<Node> & nodeSequence() const;
        
        void addNode(Operator* const op, const unsigned int inputId);
        void insertNode(const unsigned int position, Operator* const op, const unsigned int inputId);
        void removeNode(const unsigned int position);
               
    private:
        Thread(const impl::Network* const network);
        
        void start();
        void stop();
        void join();
        
        impl::ThreadImpl* m_thread;
        std::string m_name;
        const impl::Network* m_network;
        std::vector<Node> m_nodeSequence;
    };
}

#endif // STREAM_THREAD_H
