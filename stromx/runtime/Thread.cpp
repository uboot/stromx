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

#include <boost/thread/tss.hpp>

#include "stromx/runtime/Exception.h"
#include "stromx/runtime/Operator.h"
#include "stromx/runtime/Thread.h"
#include "stromx/runtime/impl/InputNode.h"
#include "stromx/runtime/impl/Network.h"
#include "stromx/runtime/impl/ThreadImpl.h"

namespace stromx
{
    namespace runtime
    {
        using namespace impl;
        
        namespace 
        {
            template<typename T>
            void do_release(T*)
            {
            }
        }
        
        STROMX_RUNTIME_API boost::thread_specific_ptr<Thread> gThread(do_release);
        
        Thread::Thread(const Network*const network)
          : m_thread(0),
            m_network(network),
            m_interruptedFlag(false)
        {
            if(! network)
                throw WrongArgument("Passed null pointer as network.");
            
            m_thread = new impl::ThreadImpl(this);
        }
        
        Thread::~Thread()
        {
            delete m_thread;
        }
        
        Thread::Status Thread::status() const 
        { 
            return Status(m_thread->status());
        }
        
        const std::string & Thread::name() const
        {
            return m_name; 
        }
        
        void Thread::setName(const std::string& name)
        { 
            m_name = name; 
        }
        
        const Color& Thread::color() const
        {
            return m_color;
        }

        void Thread::setColor(const Color& color)
        {
            m_color = color;
        }
        
        const std::vector<InputConnector> & Thread::inputSequence() const
        { 
            return m_inputSequence;
        }
        
        void Thread::start()
        {
            m_thread->start();
        }
        
        void Thread::stop()
        {
            m_thread->stop(); 
        }
        
        void Thread::join()
        { 
            m_thread->join();
        }
        
        void Thread::pause()
        {
            m_thread->pause();
        }

        void Thread::resume()
        {
            m_thread->resume();
        }

        void Thread::setDelay(const unsigned int delay)
        {
            m_thread->setDelay(delay);
        }
            
        void Thread::addInput(Operator* const op, const unsigned int inputId)
        {
            if (op == 0)
                throw WrongArgument("Operator must not be null.");
            
            if (op->status() == Operator::NONE)
                throw WrongState("Operator must be initialized.");
            
            InputNode* inputNode = m_network->getInputNode(op, inputId);
            m_thread->addInput(inputNode);
            m_inputSequence.push_back(InputConnector(op, inputId));
        }

        void Thread::insertInput(const unsigned int position, Operator* const op, const unsigned int inputId)
        {
            if (op == 0)
                throw WrongArgument("Operator must not be null.");
            
            if (op->status() == Operator::NONE)
                throw WrongState("Operator be initialized.");
            
            InputNode* inputNode = m_network->getInputNode(op, inputId);
            m_thread->insertInput(position, inputNode);
            m_inputSequence.insert(m_inputSequence.begin() + position, InputConnector(op, inputId));
        }

        void Thread::removeInput(const unsigned int position)
        {
            m_thread->removeInput(position);
            m_inputSequence.erase(m_inputSequence.begin() + position);
        }
        
        void Thread::removeInput(Operator*const op, const unsigned int inputId)
        {
            if (op == 0)
                throw WrongArgument("Operator must not be null.");
            
            for(unsigned int i = 0; i < m_inputSequence.size(); i++)
            {
                if(m_inputSequence[i].op() == op && m_inputSequence[i].id() == inputId)
                {
                    removeInput(i);
                    break;
                }
            }
        }
        
        void Thread::setObserver(const ThreadImplObserver*const observer)
        {
            m_thread->setObserver(observer);
        }
    } 
}
