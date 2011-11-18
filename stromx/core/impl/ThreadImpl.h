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

#ifndef STROMX_CORE_IMPL_THREADIMPL_H
#define STROMX_CORE_IMPL_THREADIMPL_H

#include <vector>
#include <string>

#include <boost/thread/mutex.hpp>
#include <boost/thread.hpp>

namespace stromx
{
    namespace core
    {
        namespace impl
        {
            class InputNode;
            
            class ThreadImpl
            {    
            public:
                enum Status
                {
                    INACTIVE,
                    ACTIVE,
                    DEACTIVATING
                };
                
                ThreadImpl();
                ~ThreadImpl();
                
                const Status status() const { return m_status; }
                const std::vector<InputNode*> & nodeSequence() const { return m_nodeSequence; }
                
                void addNode(InputNode* const op);
                void insertNode(const unsigned int position, InputNode* const op);
                void removeNode(const unsigned int position);
                
                void start();
                void stop();
                void join();
                    
            private:
                void loop();
                
                Status m_status;
                boost::thread* m_thread;
                std::vector<InputNode*> m_nodeSequence;
            };
        }
    }
}

#endif // STROMX_CORE_IMPL_THREADIMPL_H
