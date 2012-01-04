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

#ifndef STROMX_CORE_STREAM_H
#define STROMX_CORE_STREAM_H

#include <set>
#include <string>
#include <vector>
#include "Config.h"
#include "Output.h"
#include "impl/ThreadImplObserver.h"

namespace stromx
{
    namespace core
    {
        class Operator;
        class Registry;
        class Thread;
        class ExceptionObserver;
        
        namespace impl
        {
            class Network;
        }
        
        /** \brief The core data processing pipeline of core. */
        class STROMX_CORE_API Stream
        {
        public:
            /** The possible states of a stream. */
            enum Status
            {
                /** The stream is inactive. */
                INACTIVE,
                /** The stream is active. */
                ACTIVE,
                /** The stream was stopped and is waiting to become inactive. */
                DEACTIVATING,
                /** The stream was paused and can be resumed or stopped. */
                PAUSED
            };
            
            /** Constructs a stream object */
            Stream();
            ~Stream();
            
            /** Returns the name of the stream. */
            const std::string& name() const { return m_name; }
            
            /** Sets the name of the stream. */
            void setName(const std::string& name) { m_name = name; }
            
            /** Returns the current state of the stream. */
            const Status status() const { return m_status; }
            
            /** Returns a list of operators assigned to the stream */
            const std::vector<Operator*>& operators() const;           
            
            /**
             * \throws WrongState If the stream is not INACTIVE.
             */
            void connect(Operator* const sourceOp, const unsigned int outputId, 
                         Operator* const targetOp, const unsigned int inputId) const;
            void disconnect(Operator* const targetOp, const unsigned int inputId) const;
            const Output connectionSource(const Operator* const targetOp, const unsigned int inputId) const;
            
            /** Adds the operator \c op to the stream.
             * \throws WrongState If the stream is not INACTIVE.
             * \throws WrongArgument If the operator pointer \c op is not valid (equal to 0).
             * \throws WrongArgument If the object referenced by the pointer \c op is not initialized.
             * \throws WrongArgument If the object referenced by the pointer \c op has already been added to the stream.
             */
            void addOperator(Operator* const op);
            
            /** Removes the operator \c op from the stream and disconnects it from all other connected sources and targets.
             * In addition, if the operator \c op is used by any thread it is automatically removed from this thread. 
             * \throws WrongState If the stream is not INACTIVE.
             * \throws WrongState If the operator \c op is processed by a thread which is not INACTIVE.
             * \throws WrongArgument If the operator pointer \c op is not valid (equal to 0).
             * \throws WrongArgument If the operator referenced by the pointer \c op is not known by the stream.
             */
            void removeOperator(Operator* const op);
            
            Thread* const addThread();
            void removeThread(Thread* const thr);
            const std::vector<Thread*> & threads() const;
            
            void addObserver(const ExceptionObserver* const observer);
            void removeObserver(const ExceptionObserver* const observer);
            
            void start();
            void join();
            void stop();
            void pause();
            void resume();
            
        private:
            class InternalObserver : public impl::ThreadImplObserver
            {
            public:
                InternalObserver(const Stream* const stream, const Thread* const thread);
                virtual void observe(const std::exception & ex) const;
                
            private:
                const Stream* m_stream;
                const Thread* m_thread;
            };
            
            void observeException(const Thread & thread, const std::exception & ex) const;
            
            std::string m_name; 
            impl::Network* const m_network;
            std::vector<Thread*> m_threads;
            std::set<const ExceptionObserver*> m_observers;
            Status m_status;
        };
    }
}

#endif // STROMX_CORE_STREAM_H
