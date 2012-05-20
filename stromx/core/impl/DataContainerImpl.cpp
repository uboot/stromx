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

#include <boost/assert.hpp>
#include "DataContainerImpl.h"
#include "../Data.h"
#include "../Exception.h"
#include "../Recycler.h"

namespace stromx
{
    namespace core
    {
        namespace impl
        {
            DataContainerImpl::DataContainerImpl(Data* const data)
            : m_readAccessCounter(0),
                m_writeAccess(false),
                m_recycleAccess(0),
                m_data(data)
            {
                if(! data)
                    throw WrongArgument();
            }
            
            void DataContainerImpl::getReadAccess(const bool waitWithTimeout, const unsigned int timeout)
            {
                unique_lock_t lock(m_mutex);
                
                try
                {
                    if(waitWithTimeout)
                    {
                        boost::system_time const finish = boost::get_system_time() + boost::posix_time::millisec(timeout);
                
                        while(m_writeAccess)
                        {
                            if(! m_cond.timed_wait(lock, finish))
                                throw Timeout();
                        }
                    }
                    else
                    {
                        while(m_writeAccess)
                            m_cond.wait(lock);
                    }
                }
                catch(boost::thread_interrupted&)
                {
                    throw Interrupt();
                } 
                
                m_readAccessCounter++;
            }
            
            DataContainerImpl::~DataContainerImpl()
            {
                recycle();
                
                delete m_data;
            }

            void DataContainerImpl::returnReadAccess()
            {
                lock_t lock(m_mutex);
                
                BOOST_ASSERT(m_readAccessCounter);
                m_readAccessCounter--;
                
                m_cond.notify_all();
            }
                
            void DataContainerImpl::getWriteAccess(const bool waitWithTimeout, const unsigned int timeout)
            {
                unique_lock_t lock(m_mutex);
                
                try
                {
                    if(waitWithTimeout)
                    {
                        boost::system_time const finish = boost::get_system_time() + boost::posix_time::millisec(timeout);
                
                        while(m_readAccessCounter || m_writeAccess)
                        {
                            if(! m_cond.timed_wait(lock, finish))
                                throw Timeout();
                        }
                    }
                    else
                    {
                        while(m_readAccessCounter || m_writeAccess)
                            m_cond.wait(lock);
                    }
                }
                catch(boost::thread_interrupted&)
                {
                    throw Interrupt();
                } 
                    
                m_writeAccess = true;
            }

            void DataContainerImpl::returnWriteAccess()
            {
                lock_t lock(m_mutex);
                
                BOOST_ASSERT(m_writeAccess);
                m_writeAccess = false;
                
                m_cond.notify_all();
            }

            void DataContainerImpl::getRecycleAccess(Recycler*const recycler)
            {        
                if(! recycler)
                    throw WrongArgument();
                
                unique_lock_t lock(m_mutex);
                
                try
                {
                    while(m_recycleAccess)
                        m_cond.wait(lock);
                }
                catch(boost::thread_interrupted&)
                {
                    throw Interrupt();
                } 
                    
                m_recycleAccess = recycler;
            }

            void DataContainerImpl::returnRecycleAccess()
            {
                lock_t lock(m_mutex);
                
                BOOST_ASSERT(m_recycleAccess);
                m_recycleAccess = 0;
                
                m_cond.notify_all();
            }
            
            void DataContainerImpl::recycle()
            {
                lock_t lock(m_mutex);
                
                if(m_recycleAccess)
                {
                    m_recycleAccess->recycle(this);
                    m_data = 0;
                }        
            }
        }
    } 
}
