/* 
*  Copyright 2011 Thomas Fidler
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

#ifndef STREAM_STREAM_H
#define STREAM_STREAM_H

#include <vector> 
#include <string>
#include "Network.h"

namespace stream
{
    class Thread;
    class Network;
    class Operator;
    
    class Stream
    {
    public:
        enum Status
        {
            INACTIVE,
            ACTIVE,
            DEACTIVATING,
        };
        
        Stream();
        ~Stream();
        const std::string & name() { return m_name; }
        void setName(const std::string name) { m_name = name; }
        
        void addOperator(Operator* const op) { m_network->addOperator(op); }
        
        const std::vector<Operator*> operators() const { return m_network->operators(); }
        void connect(const Node & target, const Node & source);
        void disconnect(const Node & target);
        
        Thread* const addThread();
        void removeThread(Thread* const thr);
        const std::vector<Thread*> & threads();
        const Status status();
        
        void start();
        void join();
        void stop();
        
    private:
        std::string m_name; 
        Network* const m_network;
        std::vector<Thread*> m_threads;
        Status m_status;
    };
}


#endif // STREAM_STREAM_H