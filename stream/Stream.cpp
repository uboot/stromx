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

#include "Stream.h"

#include "Thread.h"
#include "Exception.h"
#include "Network.h"

namespace stream
{
    Stream::Stream(Network* const network)
      : m_network(network),
        m_threads(0),
        m_status(INACTIVE)
    {
        if (m_network == 0)
        {
            throw ArgumentException("Invalid argument: Null pointer");
        }
    }
    
    Stream::~Stream()
    {
        for (std::vector<Thread*>::iterator iter = m_threads.begin();
            iter != m_threads.end();
            ++iter) 
        {
            delete (*iter);
        }
        
        delete m_network;
    }
    
    Network* const Stream::network() const
    {
        return m_network;
    }
    
    void Stream::start()
    {
        if (m_status == ACTIVE)
        {
            throw WrongState("Stream object already active. Can't use start().");
        }
       
        m_network->activate();
        for (std::vector<Thread*>::iterator iter = m_threads.begin();
            iter != m_threads.end();
            ++iter)
        {
            (*iter)->start();
        }
        
        m_status = ACTIVE;
    }
    
    void Stream::join()
    {
        if (m_status != ACTIVE)
        {
            throw WrongState("Stream object not active. Can't use join().");
        }
        
        m_status = DEACTIVATING;
        for (std::vector<Thread*>::iterator iter = m_threads.begin();
            iter != m_threads.end();
            ++iter)
        {
            (*iter)->join();
        }
    }

    void Stream::stop()
    {
        if (m_status != ACTIVE)
        {
            throw WrongState("Stream object not active. Can't use stop().");
        }
        
        for (std::vector<Thread*>::iterator iter = m_threads.begin();
            iter != m_threads.end();
            ++iter)
        {
            (*iter)->stop();
        }
        
        this->join();
        m_network->deactivate();
        m_status = INACTIVE;
    }
    
    void Stream::addThread(Thread* const thr)
    {
        if (thr == 0)
        {
            throw ArgumentException("Invalid argument: Null pointer");
        }
        
        for (std::vector<Thread*>::iterator iter = m_threads.begin();
            iter != m_threads.end();
            ++iter)
        {
            if ((*iter) == thr)
            {
                throw ArgumentException("Thread already exists");
            }
        }
        
        m_threads.push_back(thr);
    }
    
    void Stream::removeThread(Thread* const thr)
    {
        if (thr == 0)
        {
            throw ArgumentException("Invalid argument: Null pointer");
        }
        
        for (std::vector<Thread*>::iterator iter = m_threads.begin();
            iter != m_threads.end();
            ++iter)
        {
            if ((*iter) == thr)
            {
                m_threads.erase(iter);
                return;
            }
        }
        
        throw ArgumentException("Thread does not exists");
    }
    
    const std::vector<Thread*> & Stream::threads()
    {
        return m_threads;
    }
    
    const stream::Stream::Status Stream::status()
    {
        return m_status;
    }

    
}