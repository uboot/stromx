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

#ifndef STROMX_RUNTIME_THREAD_H
#define STROMX_RUNTIME_THREAD_H

#include <string>
#include <vector>
#include "stromx/runtime/Color.h"
#include "stromx/runtime/Config.h"
#include "stromx/runtime/InputConnector.h"

namespace stromx
{
    namespace python
    {
        class ThreadUtilities;
    }
    
    namespace runtime
    {
        class Operator;
        
        namespace impl
        {
            class Network;
            class ThreadImpl;
            class ThreadImplObserver;
        }
        
        /** 
         * \brief A thread which visits input nodes.
         * 
         * A thread visits a given list of operator inputs. For each input 
         * in the list it obtains the input data from the connected output and
         * passes it to the input. When data is passed as input to an operator
         * or output data is obtained from the operator the operator is implicitely
         * executed.
         */
        class STROMX_RUNTIME_API Thread
        {    
            friend class Stream;
            friend class ThreadTest;
            friend class stromx::python::ThreadUtilities;
            
        public:
            /** The possible states of a thread. */
            enum Status
            {
                /** The thread is inactive. */
                INACTIVE,
                /** The thread is active. */
                ACTIVE,
                /** The thread was stopped and is waiting to become inactive. */
                DEACTIVATING,
                /** The thread was paused and be resumed or stopped. */
                PAUSED
            };
            
            virtual ~Thread();
            
            /** Returns the current state of the thread. */
            Status status() const;
            
            /** Returns the name of the thread. */
            const std::string & name() const;
            
            /** Sets the name of the thread. */
            void setName(const std::string& name);
            
            /** Returns the color of the thread. */
            const Color & color() const;
            
            /** 
             * Sets the color of the thread. The color is used only for 
             * visualizing the stream and does not have any effect on the functionality
             * of the thread. */
            void setColor(const Color & color);
            
            /** Returns a list of the operator inputs which are assigned to this thread. */
            const std::vector<InputConnector> & inputSequence() const;
            
            /** 
             * Adds the input \c inputId of the operator \c op to the list of currently
             * visited inputs. The operator must be initialized.
             * \throws WrongState If the thread state is not INACTIVE.
             * \throws WrongState If the operator is not initialized.
             * \throws WrongArgument If \c op is null.
             */
            void addInput(Operator* const op, const unsigned int inputId);
            
            /** 
             * Inserts the input \c inputId of the operator \c op into the list of currently
             * visited inputs at \c position. The operator must be initialized.
             * \throws WrongState If the thread state is not INACTIVE.
             * \throws WrongState If the operator is not initialized.
             * \throws WrongArgument If \c op is null or if there is no input at \c position.
             */
            void insertInput(const unsigned int position, Operator* const op, const unsigned int inputId);
            
            /** 
             * Removes the input at \c position from the list of currently
             * visited inputs.
             * \throws WrongState If the thread state is not INACTIVE.
             * \throws WrongArgument If there is no input at \c position.
             */
            void removeInput(const unsigned int position);
            
            /** 
             * Removes the input \c inputId of the operator \c op from the list of currently
             * visited inputs.
             * \throws WrongState If the thread state is not INACTIVE.
             * \throws WrongArgument If \c op is null.
             */
            void removeInput(Operator* const op, const unsigned int inputId);
                
        private:
            explicit Thread(const impl::Network* const network);
            
            void start();
            void stop();
            void join();
            void pause();
            void resume();
            
            void setDelay(const unsigned int delay);
            
            void setObserver(const impl::ThreadImplObserver* const observer);
            
            /**
             * Utility flag for Python wrappers
             *
             * If an interrupt exception is thrown in a virtual function call which is
             * implemented in Python the exception is caught on the Python side an not
             * passed on to C++. As a workaround a flag is set when an interrupt exception passes
             * the boundary from C++ to Python. This flag is checked again at the boundary 
             * Python to C++.
             */
            void setInterruptedFlag(const bool value) { m_interruptedFlag = value; }
            bool interruptedFlag() const { return m_interruptedFlag; }
            
            impl::ThreadImpl* m_thread;
            std::string m_name;
            const impl::Network* m_network;
            std::vector<InputConnector> m_inputSequence;
            Color m_color;
            bool m_interruptedFlag;
        };
    }
}

#endif // STROMX_RUNTIME_THREAD_H
