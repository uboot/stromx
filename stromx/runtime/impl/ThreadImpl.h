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

#ifndef STROMX_RUNTIME_IMPL_THREADIMPL_H
#define STROMX_RUNTIME_IMPL_THREADIMPL_H

#include <boost/thread/condition_variable.hpp>
#include <boost/thread/locks.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/thread.hpp>
#include <string>
#include <vector>

namespace stromx
{
    namespace runtime
    {
        class Exception;
        class Operator;
        class Thread;
        
        namespace impl
        {
            class InputNode;
            class ThreadImplObserver;
            
            class ThreadImpl
            {    
            public:
                enum Status
                {
                    INACTIVE,
                    ACTIVE,
                    DEACTIVATING,
                    PAUSED
                };
                
                ThreadImpl(Thread* thread = 0);
                ~ThreadImpl();
                
                Status status() const { return m_status; }
                const std::vector<InputNode*> & inputSequence() const { return m_inputSequence; }
                
                void addInput(InputNode* const op);
                void insertInput(const unsigned int position, InputNode* const op);
                void removeInput(const unsigned int position);
                void removeOperator(const Operator* op);
                
                void setDelay(const unsigned int delay);
                
                void start();
                void stop();
                void join();
                void pause();
                void resume();
                
                void setObserver(const ThreadImplObserver* const observer);
                    
            private:
                typedef boost::lock_guard<boost::mutex> lock_t;
                typedef boost::unique_lock<boost::mutex> unique_lock_t;
                
                void loop();
                
                Status m_status;
                boost::thread* m_thread;
                boost::mutex m_mutex;
                boost::condition_variable m_pauseCond;
                std::vector<InputNode*> m_inputSequence;
                const ThreadImplObserver* m_observer;
                unsigned int m_delay;
                Thread* m_parentThread;
            };
        }
    }
}

#endif // STROMX_RUNTIME_IMPL_THREADIMPL_H
