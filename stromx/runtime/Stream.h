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

#ifndef STROMX_RUNTIME_STREAM_H
#define STROMX_RUNTIME_STREAM_H

#include <set>
#include <string>
#include <vector>
#include "stromx/runtime/Config.h"
#include "stromx/runtime/ExceptionObserver.h"
#include "stromx/runtime/Output.h"
#include "stromx/runtime/impl/ThreadImplObserver.h"

namespace stromx
{
    namespace runtime
    {
        class AbstractFactory;
        class Operator;
        class OperatorError;
        class OperatorKernel;
        class Registry;
        class Thread;
        
        namespace impl
        {
            class Network;
        }
        
        /** \brief The core data processing pipeline of stromx. */
        class STROMX_RUNTIME_API Stream
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
                /** The stream was paused and is waiting to be resumed or stopped. */
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
            Status status() const { return m_status; }
            
            /** Returns a list of the operators of the stream */
            const std::vector<Operator*>& operators() const;     
            
            /** Returns a list of the initialized operators of the stream */
            const std::vector<Operator*>& initializedOperators() const;    
            
            /**
             * Returns the current factory of the stream. The factory is passed 
             * to each operator kernel during the execution and can be used by the
             * operator kernel to instantiate new data objects or operators. Returns null if no
             * current factory is set. If the stream was instantiated by a factory
             * the operator factory is initially set to the instantiating factory.
             */
            const AbstractFactory* factory() const { return m_factory; }
            
            /**
             * Sets the factory of the stream. This overwrites the factory property
             * of all operators of the stream.  The factory is passed 
             * to each operator kernel during the execution and can be used by the
             * operator kernel to instantiate new data objects or operators. Returns null if no
             * current factory is set. If the stream was instantiated by a factory
             * the operator factory is initially set to the instantiating factory.
             * 
             * \param factory A pointer to the factory is stored but not owned by the operator.
             *                Pass null to reset the factory of this operator.
             * \throws WrongState If the stream is not inactive.
             */
            void setFactory(const AbstractFactory* const factory);      
            
            /**
             * Connects the output \c outputId of the operator \c sourceOp to the input \c inputId of
             * the operator \c targetOp. The operators must be initialized.
             * 
             * \throws WrongArgument If the operators \c sourceOp or \c targetOp do not belong to the stream.
             * \throws WrongArgument If the operators \c sourceOp or \c targetOp do not have 
             *                       inputs \c outputId or \c inputId, respectively.
             * \throws WrongState If the operators \c sourceOp or \c targetOp are not initialized.
             * \throws WrongState If the stream is not INACTIVE.
             */
            void connect(Operator* const sourceOp, const unsigned int outputId, 
                         Operator* const targetOp, const unsigned int inputId) const;
                         
            /**
             * Disconnects the input \c inputId of the operator \c targetOp from any
             * output. The operator must be initialized.
             * 
             * \throws WrongArgument If the operator \c targetOp does not belong to the stream.
             * \throws WrongArgument If the operator \c targetOp does not have an input \c inputId.
             * \throws WrongState If the stream is not inactive.
             * \throws WrongState If the operator \c targetOp is not initialized.
             */
            void disconnect(Operator* const targetOp, const unsigned int inputId) const;
            
            /**
             * Returns the output which is connected to the input \c inputId of the 
             * operator \c targetOp. The operator must be initialized.
             * 
             * \return A valid output if the input is connected to another operator. The 
             *         returned output is invalid if no output is connected to the input. 
             * \throws WrongArgument If the operator \c targetOp does not belong to the stream.
             * \throws WrongArgument If the operator \c targetOp does not have an input \c inputId.
             * \throws WrongState If the operator \c targetOp is not initialized.
             */
            const Output connectionSource(const Operator* const targetOp, const unsigned int inputId) const;
            
            /** 
             * Converts the operator kernel \c op to an operator and adds it to the stream.
             * The ownership of the operator is transfered to the stream, i.e. it must not
             * be deleted by the caller. Returns a pointer to the new operator.
             * 
             * \throws WrongArgument If the operator pointer \c op is null.
             * \throws WrongArgument If the object referenced by the pointer \c op has already been added to the stream.
             * \throws WrongState If the stream is not INACTIVE.
             */
            Operator* addOperator(OperatorKernel* const op);
            
            /** 
             * Removes the operator \c op from the stream and disconnects it from all other connected sources and targets.
             * In addition, if the operator \c op is used by any thread it is automatically removed from this thread.
             * Finally, the operator is \em not deleted. 
             * 
             * \throws WrongState If the stream is not INACTIVE.
             * \throws WrongArgument If the operator pointer \c op is null.
             * \throws WrongArgument If the operator referenced by the pointer \c op is not known by the stream.
             */
            void removeOperator(Operator* const op);
            
            /** 
             * Initializes the operator \c op if its status is Operator::NONE.
             * After a successful call the status is Operator::INITIALIZED.
             * 
             * \throws WrongState If the status of \c op is not Operator::NONE.
             */
            void initializeOperator(Operator* const op);
            
            /** 
             * Deinitializes the operator \c op if its status is Operator::INITIALIZED.
             * After a successful call the status is Operator::NONE.
             * 
             * \throws WrongState If the status of \c op is not Operator::INITIALIZED.
             */
            void deinitializeOperator(Operator* const op);
            
            /**
             * Creates a thread, adds it to the stream and returns a pointer to it.
             * 
             * \return A pointer to the created thread. The thread is owned by the stream and
             *         must not be deleted by the caller.
             */
            Thread* addThread();
            
            /**
             * Removes the thread \c thr from the stream and deletes it.
             * 
             * \param thread The thread which is deleted. The pointer is invalid after 
             *               this function has been called. 
             * 
             * \throws WrongArgument If the thread \c thr is a null pointer or does not belong to the stream.
             */
            void removeThread(Thread* const thread);
            
            /**
             * Returns the threads of the stream.
             */
            const std::vector<Thread*> & threads() const;
            
            /**
             * Adds an observer to the stream. The observer is notified if an exception
             * occurs while executing or deactivating an operator.
             * 
             * \param observer A pointer to the observer is stored but not onwned by the operator.
             * \throws WrongArgument If \c observer is a null pointer.
             */
            void addObserver(const ExceptionObserver* const observer);
            
            /**
             * Removes an observer from the set of current observers of the stream.
             * 
             * \param observer The observer to be removed.
             * \throws WrongArgument If the observer has not been added to the stream before.
             * 
             * \sa addObserver()
             */
            void removeObserver(const ExceptionObserver* const observer);
            
            /** 
             * Returns the current processing delay in milliseconds. The threads of the 
             * stream delay the processing of an operator input by this amount of time.
             * 
             * \sa setDelay()
             */
            unsigned int delay() const;
            
            /** 
             * Sets the current processing delay in milliseconds. The threads of the 
             * stream delay the processing of an operator input by this amount of time.
             * Normally the processing delay should be set to 0 to ensure all input data
             * is processed as soon as it becomes available. For debugging it might however
             * make sense to choose a positive processing delay.
             * 
             * \sa setDelay()
             */
            void setDelay(const unsigned int delay);
            
            /**
             * Activates each operator of the stream and starts all threads. 
             * \throws WrongState If the stream is not inactive.
             * \throws OperatorError If an exception was thrown during the activation of an operator.
             */
            void start();
            
            /**
             * Signals all threads of the stream to stop if the stream is active or paused.
             * If active the stream is deactivating after a call to this function.
             * Use join() to wait for the threads to stop.
             */
            void stop();
            
            /**
             * Waits for the stream to stop and deactivates all operators. Exceptions
             * which occur during the deactivation of an operator are sent to installed 
             * observers but ignored otherwise.
             * \throws WrongState If the stream is active of paused.
             */
            void join();
            
            /** 
             * Pauses the stream, i.e. each thread of the stream is paused. In contrast
             * to stop() and join() the operators of the stream are \em not deactivated but
             * remain in their current state.
             * \throws WrongState If the stream is not active.
             */
            void pause();
            
            /** 
             * Resumes a paused stream, i.e. each thread of the stream is resumed. The execution
             * of the stream starts in the very same state which it was paused in. 
             * \throws WrongState If the stream is not paused.
             */
            void resume();
            
        private:
            class MutexHandle;
            class InternalObserver;
            
            void observeException(const ExceptionObserver::Phase phase, const OperatorError & ex, const Thread * const thread) const;
            bool isPartOfStream(const Operator* const op) const;
            bool isPartOfStream(const OperatorInfo* const op) const;
            bool isPartOfInitializedStream(const Operator* const op) const;
            bool isPartOfUninitializedStream(const Operator* const op) const;
            
            std::string m_name; 
            impl::Network* const m_network;
            std::vector<Thread*> m_threads;
            std::set<const ExceptionObserver*> m_observers;
            MutexHandle*  m_observerMutex;
            Status m_status;
            const AbstractFactory* m_factory;
            MutexHandle*  m_delayMutex;
            unsigned int m_delay;
            std::set<Operator*> m_uninitializedOperators;
            std::vector<Operator*> m_operators;
        };
    }
}

#endif // STROMX_RUNTIME_STREAM_H
