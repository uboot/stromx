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

#include <boost/assert.hpp>
#include <boost/thread.hpp>
#include "stromx/runtime/Enum.h"
#include "stromx/runtime/Exception.h"
#include "stromx/runtime/ExceptionObserver.h"
#include "stromx/runtime/None.h"
#include "stromx/runtime/Operator.h"
#include "stromx/runtime/OperatorKernel.h"
#include "stromx/runtime/Primitive.h"
#include "stromx/runtime/Registry.h"
#include "stromx/runtime/Stream.h"
#include "stromx/runtime/Thread.h"
#include "stromx/runtime/impl/InputNode.h"
#include "stromx/runtime/impl/MutexHandle.h"
#include "stromx/runtime/impl/Network.h"

namespace stromx
{
    namespace runtime
    {
        /** \cond */
        class Stream::InternalThreadObserver : public impl::ThreadImplObserver
        {
        public:
            InternalThreadObserver(const Stream* const stream, const Thread* const thread) 
              : m_stream(stream),
                m_thread(thread)
            {}
            
            virtual void observe(const OperatorError & ex) const
            {
                m_stream->observeException(ExceptionObserver::EXECUTION, ex, m_thread);
            }
            
        private:
            const Stream* m_stream;
            const Thread* m_thread;
        };
        
        class Stream::InternalNetworkObserver : public impl::NetworkObserver
        {
        public:
            InternalNetworkObserver(const Stream* const stream) 
              : m_stream(stream)
            {}
            
            virtual void observe(const OperatorError & ex, const ExceptionObserver::Phase phase) const
            {
                m_stream->observeException(phase, ex, 0);
            }
            
        private:
            const Stream* m_stream;
        };
        /** \endcond */

        Stream::Stream()
          : m_network(new impl::Network()),
            m_threads(0),
            m_observerMutex(new impl::MutexHandle),
            m_status(INACTIVE),
            m_factory(0),
            m_delayMutex(new impl::MutexHandle),
            m_delay(0)
        {
            InternalNetworkObserver* observer = new InternalNetworkObserver(this);
            m_network->setObserver(observer);
        }
        
        Stream::~Stream()
        {
            // stop all visible threads
            for (std::vector<Thread*>::iterator iter = m_threads.begin();
                iter != m_threads.end();
                ++iter) 
            {
                (*iter)->stop();
            }
            
            // interrupt the network
            m_network->interrupt();
            
            // in the meantime delete all uninitialized operators       
            for (std::set<Operator*>::iterator iter = m_uninitializedOperators.begin();
                iter != m_uninitializedOperators.end();
                ++iter) 
            {
                delete (*iter);
            }
            m_uninitializedOperators.clear();
            
            // ...the same for the hidden operators...
            for (std::set<Operator*>::iterator iter = m_hiddenOperators.begin();
                iter != m_hiddenOperators.end();
                ++iter) 
            {
                delete (*iter);
            }
            m_hiddenOperators.clear();
            
            // ...and the hidden threads
            for (std::set<Thread*>::iterator iter = m_hiddenThreads.begin();
                iter != m_hiddenThreads.end();
                ++iter) 
            {
                delete (*iter);
            }
            m_hiddenThreads.clear();
            
            // now join and delete the threads
            for (std::vector<Thread*>::iterator iter = m_threads.begin();
                iter != m_threads.end();
                ++iter) 
            {
                delete (*iter);
            }
            
            // delete the rest
            delete m_network;
            delete m_observerMutex;
            delete m_delayMutex;
        }
        
        const std::vector<Operator*>& Stream::operators() const
        { 
            return m_operators;
        }
        
        void Stream::setFactory(const AbstractFactory*const factory)
        {
            if (m_status != INACTIVE)
                throw WrongState("Factory can not be set if the stream is not inactive.");
                
            m_factory = factory;
            
            for (std::vector<Operator*>::iterator iter = m_operators.begin();
                 iter != m_operators.end();
                 ++iter)
            {
                (*iter)->setFactory(factory);
            }
        }
        
        const std::vector<Operator*>& Stream::initializedOperators() const
        { 
            return m_network->operators();
        }
        
        void Stream::start()
        {
            if (m_status != INACTIVE)
            {
                throw WrongState("Stream object not inactive.");
            }
        
            try
            {
                m_network->activate();
                for (std::vector<Thread*>::iterator iter = m_threads.begin();
                    iter != m_threads.end();
                    ++iter)
                {
                    (*iter)->start();
                }
                
                m_status = ACTIVE;
            }
            catch(OperatorError &)
            {
                // an error occurred while activating the network
                // make sure all operators are deactivated
                m_network->deactivate();
                throw;
            }
        }
        
        void Stream::pause()
        {
            if (m_status != ACTIVE)
            {
                throw WrongState("Stream object not active.");
            }
            
            for (std::vector<Thread*>::iterator iter = m_threads.begin();
                iter != m_threads.end();
                ++iter)
            {
                (*iter)->pause();
            }
            
            m_status = PAUSED;
        }

        void Stream::resume()
        {
            if (m_status != PAUSED)
            {
                throw WrongState("Stream object not active.");
            }
            
            for (std::vector<Thread*>::iterator iter = m_threads.begin();
                iter != m_threads.end();
                ++iter)
            {
                (*iter)->resume();
            }
            
            m_status = ACTIVE;
        }
        
        void Stream::join()
        {
            if (m_status == INACTIVE)
            {
                return;
            }
            
            if (m_status != DEACTIVATING)
            {
                throw WrongState("Stream object not deactivating.");
            }
            
            for (std::vector<Thread*>::iterator iter = m_threads.begin();
                iter != m_threads.end();
                ++iter)
            {
                (*iter)->join();
                BOOST_ASSERT((*iter)->status() == Thread::INACTIVE);
            }
            
            try
            {
                m_network->deactivate();
            }
            catch(OperatorError & ex)
            {
                // send any operator exceptions to the observers
                observeException(ExceptionObserver::DEACTIVATION, ex, 0);
            }
            
            m_status = INACTIVE;
        }

        void Stream::stop()
        {
            if (m_status == INACTIVE)
            {
                return;
            }
            
            for (std::vector<Thread*>::iterator iter = m_threads.begin();
                iter != m_threads.end();
                ++iter)
            {
                (*iter)->stop();
            }
            
            m_network->interrupt();
            
            m_status = DEACTIVATING;
        }
        
        Thread* Stream::addThread()
        {
            if (m_status != INACTIVE)
                throw WrongState("Cannot add thread while the stream is active.");
                
            Thread* thread = new Thread(m_network);
            attachThread(thread);
            return thread;
        }
        
        void Stream::removeThread(Thread* const thread)
        {
            detachThread(thread);
            delete thread;
        }
        
        const std::vector<Thread*> & Stream::threads() const
        {
            return m_threads;
        }
        
        void Stream::connect(Operator* const sourceOp, const unsigned int outputId, 
                             Operator* const targetOp, const unsigned int inputId) const
        {
            if (targetOp == 0 || sourceOp == 0)
                throw WrongArgument("Operator must not be null.");
            
            if (! isPartOfStream(targetOp) || ! isPartOfStream(sourceOp))
                throw WrongArgument("Operator has not been added to stream.");
            
            if (m_status != INACTIVE)
                throw WrongState("Cannot connect operators while the stream is active.");
            
            if (targetOp->status() == Operator::NONE || sourceOp->status() == Operator::NONE)
                throw WrongState("Operator must be initialized.");
            
            m_network->connect(sourceOp, outputId, targetOp, inputId);
        }

        void Stream::disconnect(Operator* const targetOp, const unsigned int inputId) const
        {
            
            if (targetOp == 0)
                throw WrongArgument("Operator must not be null.");
            
            if (! isPartOfStream(targetOp))
                throw WrongArgument("Operator has not been added to stream.");
            
            if (m_status != INACTIVE)
                throw WrongState("Cannot disconnect operators while the stream is active.");
            
            if (targetOp->status() == Operator::NONE)
                throw WrongState("Operator must be initialized.");
            
            m_network->disconnect(targetOp, inputId);
        }
                
        unsigned int Stream::delay() const
        {
            boost::lock_guard<boost::mutex> lock(m_delayMutex->mutex());
            
            return m_delay;
        }

        void Stream::setDelay(const unsigned int delay)
        {
            boost::lock_guard<boost::mutex> lock(m_delayMutex->mutex());
            
            m_delay = delay;
            
            for(std::vector<Thread*>::const_iterator iter = m_threads.begin();
                iter != m_threads.end();
                ++iter)
            {
                (*iter)->setDelay(delay);
            }
        }
        
        Operator* Stream::addOperator(OperatorKernel* const op)
        {
            if (op == 0)
                throw WrongArgument("Operator must not be null");
             
            if (m_status != INACTIVE)
                throw WrongState("Cannot add operator while the stream is active.");
            
            if (isPartOfStream(op))
                throw WrongArgument("Operator has already been added to the stream.");
            
            Operator* newOp = new Operator(op);
            attachOperator(newOp);
            return newOp;
        }
        
        void Stream::removeOperator(Operator* const op)
        {
            detachOperator(op);
            delete op;
        }
        
        void Stream::initializeOperator(Operator*const op)
        {
            if (op == 0)
                throw WrongArgument("Operator must not be null");
            
            if (m_status != INACTIVE)
                throw WrongState("Cannot initialize operator while the stream is active.");
            
            if (! isPartOfStream(op))
                throw WrongArgument("Operator is not part of the stream.");
            
            op->initialize();
            m_network->addOperator(op); 
            m_uninitializedOperators.erase(op);
        }

        void Stream::deinitializeOperator(Operator*const op)
        {
            if (op == 0)
                throw WrongArgument("Operator must not be null");
            
            if (m_status != INACTIVE)
                throw WrongState("Cannot deinitialize operator while the stream is active.");
            
            if (isPartOfUninitializedStream(op))
                return;
            
            m_network->removeOperator(op);
            m_uninitializedOperators.insert(op);
            
            for (std::vector<Thread*>::iterator iter = m_threads.begin();
                    iter != m_threads.end();
                    ++iter)
            {
                (*iter)->removeOperator(op);
            }
            
            op->deinitialize();
        }
        
        const Output Stream::connectionSource(const Operator* const targetOp, const unsigned int inputId) const
        {
            if (targetOp == 0)
                throw WrongArgument("Operator must not be null.");
            
            if (! isPartOfStream(targetOp))
                throw WrongArgument("Operator has not been added to stream.");
            
            if (targetOp->status() == Operator::NONE)
                throw WrongState("Operator must be initialized.");
            
            return m_network->connectionSource(targetOp, inputId);
        }
        
        void Stream::addObserver(const ExceptionObserver*const observer)
        {
            boost::lock_guard<boost::mutex> lock(m_observerMutex->mutex());
            
            if(! observer)
                throw WrongArgument("Observer must not be null.");
            
            m_observers.insert(observer);
        }

        void Stream::removeObserver(const ExceptionObserver*const observer)
        {
            boost::lock_guard<boost::mutex> lock(m_observerMutex->mutex());
            
            if(m_observers.erase(observer) != 1)
                throw WrongArgument("Observer has not been added to stream.");
        }
                
        void Stream::observeException(const ExceptionObserver::Phase phase, const OperatorError& ex, const Thread* const thread) const
        {
            boost::lock_guard<boost::mutex> lock(m_observerMutex->mutex());
            
            for(std::set<const ExceptionObserver*>::const_iterator iter = m_observers.begin();
                iter != m_observers.end();
                ++iter)
            {
                try
                {
                    (*iter)->observe(phase, ex, thread);
                }
                catch(...)
                {
                    // catch all exceptions which are thrown while observing exceptions...
                }
            }
        }
        
        bool Stream::isPartOfStream(const OperatorInfo*const op) const
        {
            for (std::vector<Operator*>::const_iterator iter = operators().begin();
                 iter != operators().end();
                 ++iter)
            {
                if (&(*iter)->info() == op)
                    return true;
            }
            
            return false;
        }
        
        bool Stream::isPartOfStream(const Operator*const op) const
        {
            return isPartOfInitializedStream(op) || isPartOfUninitializedStream(op);
        }
        
        bool Stream::isPartOfInitializedStream(const Operator*const op) const
        {
            std::vector<Operator*>::const_iterator iter = 
                std::find(m_network->operators().begin(), m_network->operators().end(), op);
            
            return iter != m_network->operators().end();
        }
        
        bool Stream::isPartOfUninitializedStream(const Operator*const op) const
        {
            std::set<Operator*>::const_iterator iter = 
                std::find(m_uninitializedOperators.begin(), m_uninitializedOperators.end(), op);
            
            return iter != m_uninitializedOperators.end();
        }
        
        void Stream::hideOperator(Operator*const op)
        {
            detachOperator(op);
            m_hiddenOperators.insert(op);
        }

        void Stream::showOperator(Operator*const op)
        {
            if (op == 0)
                throw WrongArgument("Operator must not be null");
             
            if (m_status != INACTIVE)
                throw WrongState("Cannot add operator while the stream is active.");
            
            if (isPartOfStream(op))
                throw WrongArgument("Operator has already been added to the stream.");
            
            std::set<Operator*>::iterator iter = 
                std::find(m_hiddenOperators.begin(), m_hiddenOperators.end(), op);
                
            if (iter == m_hiddenOperators.end())
                throw WrongArgument("Operator has not been hidden.");
            
            m_hiddenOperators.erase(iter);
            attachOperator(op);
        }

        void Stream::hideThread(Thread*const thread)
        {
            detachThread(thread);
            m_hiddenThreads.insert(thread);
        }

        void Stream::showThread(Thread*const thread)
        {
            if (m_status != INACTIVE)
                throw WrongState("Cannot show thread while the stream is active.");
                
            std::set<Thread*>::iterator iter = 
                std::find(m_hiddenThreads.begin(), m_hiddenThreads.end(), thread);
                
            if (iter == m_hiddenThreads.end())
                throw WrongArgument("Thread has not been hidden.");

            m_hiddenThreads.erase(iter);
            attachThread(thread);
        }
        
        void Stream::attachOperator(Operator*const op)
        {
            m_uninitializedOperators.insert(op);
            op->setFactory(m_factory);
            m_operators.push_back(op);
        }

        void Stream::attachThread(Thread*const thread)
        {
            impl::ThreadImplObserver* observer = new InternalThreadObserver(this, thread);
            thread->setObserver(observer);
            thread->setDelay(m_delay);
            
            m_threads.push_back(thread);
        }
        
        void Stream::detachOperator(Operator*const op)
        {
            if (op == 0)
                throw WrongArgument("Operator must not be null");
            
            if (m_status != INACTIVE)
                throw WrongState("Cannot remove operator while the stream is active.");
            
            if (! isPartOfStream(op))
                throw WrongArgument("Operator is not part of the stream.");
            
            deinitializeOperator(op);
            m_uninitializedOperators.erase(op);
            
            std::vector<Operator*>::iterator iter = 
                std::find(m_operators.begin(), m_operators.end(), op);
            
            (*iter)->setFactory(0);
            m_operators.erase(iter);
        }

        void Stream::detachThread(Thread*const thread)
        {
            if (thread == 0)
                throw WrongArgument("Thread must not be null");
            
            if (m_status != INACTIVE)
                throw WrongState("Cannot remove thread while the stream is active.");
            
            bool foundThread = false;
            for (std::vector<Thread*>::iterator iter = m_threads.begin();
                iter != m_threads.end();
                ++iter)
            {
                if ((*iter) == thread)
                {
                    m_threads.erase(iter);
                    foundThread = true;
                    break;
                }
            }
            
            if (! foundThread)
                throw WrongArgument("Thread does not exists");
        }
    }
}

