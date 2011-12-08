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

#ifndef STROMX_CORE_THREAD_H
#define STROMX_CORE_THREAD_H

#include <string>
#include <vector>
#include "Config.h"
#include "Input.h"

namespace stromx
{
    namespace core
    {
        namespace impl
        {
            class Network;
            class ThreadImpl;
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
        class STROMX_CORE_API Thread
        {    
            friend class Stream;
            friend class ThreadTest;
            
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
            const Status status() const;
            
            /** Returns the name of the thread. */
            const std::string & name() const;
            
            /** Sets the name of the thread. */
            void setName(const std::string& name);
            
            /** Returns a list of the operator inputs which are assigned to this thread. */
            const std::vector<Input> & inputSequence() const;
            
            /** 
             * Adds the input \c inputId of the operator \c op to the list of currently
             * visited inputs. 
             */
            void addNode(Operator* const op, const unsigned int inputId);
            
            /** 
             * Inserts the input \c inputId of the operator \c op into the list of currently
             * visited inputs at \c position.
             */
            void insertNode(const unsigned int position, Operator* const op, const unsigned int inputId);
            
            /** 
             * Removes the input at \c position from the list of currently
             * visited inputs.
             */
            void removeNode(const unsigned int position);
                
        private:
            Thread(const impl::Network* const network);
            
            void start();
            void stop();
            void join();
            
            impl::ThreadImpl* m_thread;
            std::string m_name;
            const impl::Network* m_network;
            std::vector<Input> m_inputSequence;
        };
    }
}

#endif // STROMX_CORE_THREAD_H
