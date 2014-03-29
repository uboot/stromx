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

#ifndef STROMX_RUNTIME_OPERATOR_H
#define STROMX_RUNTIME_OPERATOR_H

#include <map>
#include <set>
#include <string>
#include "stromx/runtime/ConnectorObserver.h"
#include "stromx/runtime/DataContainer.h"
#include "stromx/runtime/DataRef.h"
#include "stromx/runtime/Exception.h"
#include "stromx/runtime/OperatorInfo.h"
#include "stromx/runtime/Position.h"
#include "stromx/runtime/impl/Id2DataMap.h"

namespace stromx
{
    namespace runtime
    {
        class AbstractFactory;
        class Id2DataMapper;
        class OperatorInfo;
        class OperatorKernel;
        
        namespace impl
        {
            class InputNode;
            class Network;
            class MutexHandle;
            class OutputNode;
            class SynchronizedOperatorKernel;
        }
        
        /**
         * \brief Abstract operator
         * 
         * Operators are the core elements of a stream. The have inputs and outputs which
         * can be connected to other operators of the same stream. Moreover, it is possible
         * to access these inputs and outputs from outside to pass data to the stream or
         * obtain data from it.
         * 
         * Moreover, each operator has a current status and a number of parameters. If the 
         * value of a specific parameter can be read or written depends on the current status
         * of the operators.
         */
        class STROMX_RUNTIME_API Operator
        {
            friend class FactoryTest;
            friend class OperatorTest;
            friend class InputNodeTest;
            friend class NetworkTest;
            friend class OperatorTester;
            friend class OutputNodeTest;
            friend class ThreadImplTest;
            friend class Stream;
            friend class Thread;
            friend class impl::Network;
            
        public:
            /** The possible states of an operator. */
            enum Status
            {
                /** The default status of an operator after instantiation. */
                NONE,
                /** The is initialized but not ready to process data. */
                INITIALIZED,
                /** The operator is ready to process data. */
                ACTIVE,
                /** The operator is actively processing data. */
                EXECUTING
            };
            
            /** 
             * Constructs an operator from an operator kernel.
             * 
             * \param kernel The operator deletes \c kernel upon its destruction.
             */
            explicit Operator(OperatorKernel* const kernel);
            
            virtual ~Operator();
            
            /** Returns the name of the operator. */
            const std::string & name() const { return m_name; }
            
            /** 
             * Sets the name of the operator. The name is for informal
             * use only and does not have any effect on the functionality
             * of the operator.
             */
            void setName(const std::string & name) { m_name = name; }
            
             /** Returns the position of the operator. */
            const Position & position() const { return m_position; }
            
            /** 
             * Sets the position of the operator. The position is used only for 
             * visualizing the stream and does not have any effect on the functionality
             * of the operator.
             */
            void setPosition(const Position & position) { m_position = position; }
            
            /** 
             * Returns information about the inputs, outputs and parameters
             * of the operator.
             */
            const OperatorInfo& info() const;
            
            /** Returns the current status of the operator. */
            Status status() const;
            
            /** 
             * Sets the parameter \c id to \c value. The functions waits until setting
             * the parameter value is possible, i.e. the operator is not executed or accessed 
             * by another thread.
             * 
             * \throws Interrupt
             * \throws WrongParameterType
             * \throws ParameterAccessViolation
             * \throws WrongParameterId 
             */
            void setParameter(const unsigned int id, const Data& value);
            
            /** 
             * Sets the parameter \c id to \c value. The functions waits until setting
             * the parameter value is possible, i.e. the operator is not executed or accessed 
             * by another thread. If the function is not successful within the specified
             * \c timeout the functions throws an exception and returns.
             * 
             * \param id The ID of the parameter to be set.
             * \param value The new parameter value.
             * \param timeout The maximal time to wait in milliseconds.
             * 
             * \throws Interrupt
             * \throws ParameterAccessViolation
             * \throws Timeout If the parameter could not be set during the timeout.
             * \throws WrongParameterId 
             * \throws WrongParameterType
             */
            void setParameter(const unsigned int id, const Data& value, const unsigned int timeout);
            
            /**
             * Gets the current value of the parameter \c id. The functions waits until
             * obtaining the parameter value is possible, i.e. the operator is not
             * executed or accessed by another thread. 
             * 
             * \param id The ID of the parameter to be set.
             * 
             * \throws Interrupt
             * \throws ParameterAccessViolation
             * \throws WrongParameterId 
             */
            DataRef getParameter(const unsigned int id) const;
            
            /**
             * Gets the current value of the parameter \c id. The functions waits until
             * obtaining the parameter value is possible, i.e. the operator is not
             * executed or accessed by another thread. If the function is not successful
             * within the specified \c timeout the functions throws an exception and returns.
             * 
             * \param id The ID of the parameter to be set.
             * \param timeout The maximal time to wait in milliseconds.
             * 
             * \throws Interrupt
             * \throws ParameterAccessViolation
             * \throws Timeout If the parameter could not be set during the timeout.
             * \throws WrongParameterId 
             */
            DataRef getParameter(const unsigned int id, const unsigned int timeout) const;
            
            /**
             * Waits for data at the output ID and returns it. The data is 
             * \em not removed by this function and will still be available
             * and block the output. Use clearOutputData() to remove the output
             * data.
             * 
             * \param id The ID of the output.
             * 
             * \throws Interrupt
             * \throws WrongOperatorState
             */
            const DataContainer getOutputData(const unsigned int id) const;
            
            /** 
             * Waits for the input \c to to become empty and then places \c data
             * at the input.
             * 
             * \throws Interrupt
             * \throws WrongOperatorState
             */
            void setInputData(const unsigned int id, const DataContainer & data);
            
            /**
             * Removes any output data from the output \c id.
             * 
             * \throws WrongOperatorState
             */
            void clearOutputData(const unsigned int id);
            
            /**
             * Adds an observer which is called whenever the data at an input 
             * or output connector changes.
             * 
             * \param observer A pointer to the observer is stored but not owned by the operator.
             * \throws WrongArgument If \c observer is a null pointer.
             */
            void addObserver(const ConnectorObserver* const observer);
            
            /**
             * Removes an observer from the set of current observers of this operator.
             * 
             * \param observer The observer to be removed.
             * \throws WrongArgument If the observer has not been added to the operator before.
             */
            void removeObserver(const ConnectorObserver* const observer);
            
            /**
             * Returns the current factory of the operator. The factory is passed to the operator
             * kernel during the execution and can be used by the operator kernel
             * to instantiate new data objects or operators. Returns null if no
             * current factory is set.
             */
            const AbstractFactory* factory() const;
            
            /**
             * Sets the factory of the operator. The factory is passed to the operator
             * kernel during the execution and can be used by the operator kernel
             * to instantiate new data objects or operators.
             * 
             * \param factory A pointer to the factory is stored but not owned by the operator.
             *                Pass null to reset the factory of this operator.
             * \throws WrongState If the operator is active or executing.
             */
            void setFactory(const AbstractFactory* const factory);
            
        private:
            class InternalObserver;
            
            Operator(const Operator& op);
            
            void initialize();
            void deinitialize();
            impl::InputNode* getInputNode(const unsigned int id) const;
            impl::OutputNode* getOutputNode(const unsigned int id) const;
            void activate();
            void deactivate();
            void interrupt();
            void observeInput(const unsigned int id, const DataContainer & data) const;
            void observeOutput(const unsigned int id, const DataContainer & data) const;
            
            std::string m_name;
            InternalObserver* m_inputObserver;
            InternalObserver* m_outputObserver;
            impl::SynchronizedOperatorKernel* m_kernel;
            std::map<unsigned int, impl::OutputNode*> m_outputs;
            std::map<unsigned int, impl::InputNode*> m_inputs;
            std::set<const ConnectorObserver*> m_observers;
            Position m_position;
            impl::MutexHandle*  m_observerMutex;
        };
    }
}

#endif // STROMX_RUNTIME_OPERATOR_H
