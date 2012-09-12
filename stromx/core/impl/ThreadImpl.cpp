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

#include <boost/bind.hpp>
#include <set>
#include "InputNode.h"
#include "ThreadImpl.h"
#include "ThreadImplObserver.h"
#include "../OperatorException.h"
#include "../OperatorInfo.h"
#include "../Input.h"
#include "../Operator.h"

#include <iostream>

namespace stromx
{
    namespace core
    {
        namespace impl
        {
            ThreadImpl::ThreadImpl()
              : m_status(INACTIVE),
                m_thread(0),
                m_observer(0),
                m_delay(0)
            {
            }
            
            ThreadImpl::~ThreadImpl()
            {
                stop();
                join();
                
                delete m_observer;
            }
            
            void ThreadImpl::addInput(InputNode* const op)
            {
                if(m_status != INACTIVE)
                    throw WrongState("Thread must be inactive.");
                
                if(! op)
                    throw WrongArgument("Passed null as input node.");
                
                m_inputSequence.push_back(op);
            }

            void ThreadImpl::insertInput(const unsigned int position, InputNode* const op)
            {
                if(m_status != INACTIVE)
                    throw WrongState("Thread must be inactive.");
                
                if(! op)
                    throw WrongArgument("Passed null as input node.");
                
                if(position > m_inputSequence.size())
                    throw WrongArgument("Can only insert at an existing position of at the end of the node sequence.");
                
                m_inputSequence.insert(m_inputSequence.begin() + position, op);
            }

            void ThreadImpl::removeInput(const unsigned int position)
            {
                if(m_status != INACTIVE)
                    throw WrongState("Thread must be inactive.");
                
                if(position >= m_inputSequence.size())
                    throw WrongArgument("No node at this position.");
                
                m_inputSequence.erase(m_inputSequence.begin() + position);
            }
            
            void ThreadImpl::removeOperator(const Operator* op)
            {
                if(m_status != INACTIVE)
                    throw WrongState("Thread must be inactive.");
                
                typedef std::set< std::vector<InputNode*>::iterator > IndexSet;
                
                IndexSet toBeErased;
                
                for(std::vector<InputNode*>::iterator iter = m_inputSequence.begin();
                    iter != m_inputSequence.end();
                    ++iter)
                {
                    if((*iter)->op() == op)
                        toBeErased.insert(iter);
                }
                
                for(IndexSet::reverse_iterator iter = toBeErased.rbegin();
                    iter != toBeErased.rend();
                    ++iter)
                {
                    m_inputSequence.erase(*iter);
                }
            }
            
            void ThreadImpl::setDelay(const unsigned int delay)
            {
                lock_t lock(m_mutex);
                m_delay = delay;
            }

            void ThreadImpl::start()
            {
                if(m_status != INACTIVE)
                    throw WrongState("Thread must be inactive.");
                
                BOOST_ASSERT(! m_thread);
                
                m_thread = new boost::thread(boost::bind(&ThreadImpl::loop, this));
                
                m_status = ACTIVE;
            }

            void ThreadImpl::stop()
            {
                if(m_status == INACTIVE)
                    return;
                
                BOOST_ASSERT(m_thread);
                
                m_thread->interrupt();
                
                m_status = DEACTIVATING;
            }

            void ThreadImpl::join()
            {
                if(m_status == INACTIVE)
                {
                    BOOST_ASSERT(m_thread == 0);
                    return;
                }
                
                if(m_status != DEACTIVATING)
                    throw WrongState("Thread must have been stopped.");
                
                BOOST_ASSERT(m_thread);
                
                m_thread->join();
                
                delete m_thread;
                m_thread = 0;
                
                m_status = INACTIVE;
            }
            
            void ThreadImpl::pause()
            {
                lock_t lock(m_mutex);
                
                if(m_status != ACTIVE)
                    throw WrongState("Thread must have been paused.");
                
                m_status = PAUSED;
            }

            void ThreadImpl::resume()
            {
                lock_t lock(m_mutex);
                
                if(m_status != PAUSED)
                    throw WrongState("Thread must have been paused.");
                
                m_status = ACTIVE;
                
                m_pauseCond.notify_all();
            }
            
            void ThreadImpl::setObserver(const ThreadImplObserver*const observer)
            {
                m_observer = observer;
            }
            
            void ThreadImpl::loop()
            {
                // return immediately if there are no inputs to process
                if(! m_inputSequence.size())
                    return;
                
                try
                {
                    while(true)
                    {
                        for(std::vector<InputNode*>::iterator node = m_inputSequence.begin();
                                node != m_inputSequence.end();
                                ++node)
                        {
                            try
                            {
                                (*node)->setInputData();
                                
                                // obtain the delay state in a thread-safe way
                                unsigned int delay = 0;
                                {
                                    lock_t lock(m_mutex);
                                    
                                    delay = m_delay;
                                }
                                
                                if(delay)
                                    boost::this_thread::sleep(boost::posix_time::millisec(delay));
                            }
                            catch(Interrupt &)
                            {
                                // re-throw interrupt exceptions to stop the thread
                                throw;
                            }
                            catch(OperatorError & ex)
                            {
                                // send all operator errors to the observer-mechanism
                                // but do not stop the thread
                                if(m_observer)
                                    m_observer->observe(ex);
                            }
                            
                            try
                            {
                                unique_lock_t lock(m_mutex);
                                
                                if(m_status == PAUSED)
                                    m_pauseCond.wait(lock);
                                else
                                    boost::this_thread::interruption_point();
                            }
                            catch(boost::thread_interrupted&)
                            {
                                throw Interrupt();
                            } 
                        }
                    }
                }
                catch(Interrupt&)
                {
                    std::cout << "Caught interrupt" << std::endl;
                }
            }
        }
    }
}
