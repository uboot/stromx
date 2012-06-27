/* 
 *  Copyright 2012 Matthias Fuchs
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

#ifndef STROMX_CORE_EXCEPTIONOBSERVER_H
#define STROMX_CORE_EXCEPTIONOBSERVER_H

#include "OperatorException.h"

namespace stromx
{
    namespace core
    {
        class Thread;

        /** \brief Abstract base observer of operator exceptions. */
        class ExceptionObserver
        {
        public:
            /** 
             * The different phases in which an operator can cause exceptions
             * to be thrown.
             */
            enum Phase
            {
                /** Initialization of the operator. */
                INITIALIZATION,
                /** Activation of the operator. */
                ACTIVATION,
                /** Execution of the operator. */
                EXECUTION,
                /** Deactivation of the operator. */
                DEACTIVATION,
                /** Deinitialization of the operator. */
                DEINITIALIZATION,
                /** Accessing a parameter. */
                PARAMETER_ACCESS
            };
            
            /** 
             * Informs the observer that an operator has thrown an exception.
             * 
             * \param phase The phase during which the exception was thrown.
             * \param ex The exception.
             * \param thread The thread in which context the exception was thrown or 0 if the 
             *               the thread is not a stromx::Thread object.
             */
            virtual void observe(const Phase phase, const OperatorError & ex, const Thread* const thread) const = 0;
        };
    }
}

#endif // STROMX_CORE_EXCEPTIONOBSERVER_H
