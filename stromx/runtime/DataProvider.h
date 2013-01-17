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

#ifndef STROMX_RUNTIME_DATAPROVIDER_H
#define STROMX_RUNTIME_DATAPROVIDER_H

namespace stromx
{
    namespace runtime
    {
        class Data;
        class DataContainer;
        class Id2DataMapper;
    
        /** \brief Provider of functions to receive and send data.
         *
         * Data providers are passed as parameter to OperatorKernel::execute() and
         * provides ways for the executing kernel to send output data and receive 
         * input data.
         */
        class DataProvider
        {
        public:
            virtual ~DataProvider() {}
            
            /**
             * Throws Interrupt if the stream this operator belongs to has been stopped.
             * If this is the case the current OperatorKernel::execute() must be 
             * exited as soon as possible.
             * This function should be called regularly during a longer computation
             * in OperatorKernel::execute() to ensure that a stream can be stopped
             * within a reasonable amount of time.
             * 
             * \throws Interrupt 
             */
            virtual void testForInterrupt() = 0;
            
            /**
             * Sleep for the given amount of microseconds. The function
             * throws Interrup if the stream has been stopped while sleeping.
             * 
             * \throws Interrupt
             */
            virtual void sleep(const unsigned int microseconds) = 0;
            
            /**
             * Receives input data from the provider. The functions waits until
             * the requirements formulated in \c mapper have are fulfilled.
             * If the stream is stopped during waiting Interrupt is thrown. 
             * 
             * \throws Interrupt
             */
            virtual void receiveInputData(const Id2DataMapper& mapper) = 0;
            
            /**
             * Sends input data from the provider. The functions waits until
             * the requirements formulated in \c mapper have are fulfilled.
             * If the stream is stopped during waiting Interrupt is thrown. 
             * 
             * \throws Interrupt
             */
            virtual void sendOutputData(const Id2DataMapper& mapper) = 0;
            
            /**
             * Allows other threads to read and write parameters of the
             * calling operator until lockParameters() any other blocking function is called.
             * This function should be called if the operator is blocked by a longer operation
             * which is not affected by parameter read or writes. In particular, it must be called
             * if the operator is blocked at a condition which can only be unblocked by setting
             * a parameter.
             * 
             * \throws WrongState If the parameter access has already been unblocked.
             */
            virtual void unlockParameters() = 0;
            
            /**
             * Blocks all read and write access to parameters of the calling operator
             * until unlockParameters() is called or any other blocking function is called.
             * Blocking the parameter access makes sure no parameters are 
             * changed while the operator executes.
             * 
             * \throws WrongState If the parameter access has already been blocked.
             */
            virtual void lockParameters() = 0;
        };
    }
}

#endif // STROMX_RUNTIME_DATAPROVIDER_H