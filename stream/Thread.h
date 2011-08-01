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

#include <vector>
#include <string>

#include <boost/thread/mutex.hpp>
#include <boost/thread.hpp>

namespace stream
{
    class InputNode;
    
    class Thread
    {    
        friend class Stream;
        friend class StreamTest;
        friend class ThreadTest;
        
    public:
        enum Status
        {
            INACTIVE,
            ACTIVE,
            DEACTIVATING,
        };
        
        ~Thread();
        
        const Status status() const { return m_status; }
        
        const std::string & name() const { return m_name; }
        void setName(const std::string& name) { m_name = name; }
        const std::vector<InputNode*> & nodeSequence() const { return m_nodeSequence; }
        
        void addNode(InputNode* const op);
        void insertNode(const unsigned int position, InputNode* const op);
        void removeNode(const unsigned int position);
               
    private:
        Thread();
        void loop();
        
        void start();
        void stop();
        void join();
        
        Status m_status;
        boost::thread* m_thread;
        std::vector<InputNode*> m_nodeSequence;
        std::string m_name;
    };
}

#endif // STREAM_THREAD_H
