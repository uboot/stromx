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

namespace stream
{
    class ThreadImpl;
    class Network;
    
    class Stream
    {
    public:
        enum Status
        {
            INACTIVE,
            ACTIVE,
            DEACTIVATING,
        };
        
        Stream(Network* const m_network);
        ~Stream();
        Network* const network() const;
        void start();
        void join();
        void stop();
        void addThread(ThreadImpl* const thr);
        void removeThread(ThreadImpl* const thr);
        const std::vector<ThreadImpl*> & threads();
        const Status status();
        
    private:
        Network* const m_network;
        std::vector<ThreadImpl*> m_threads;
        Status m_status;
    };
}


#endif // STREAM_STREAM_H