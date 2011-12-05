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

#ifndef STROMX_CORE_DATAPROVIDER_H
#define STROMX_CORE_DATAPROVIDER_H

namespace stromx
{
    namespace core
    {
        class Data;
        class DataContainer;
        class Id2DataMapper;
    
        /** \brief Provider of functions to receive and send data.
         *
         * Data providers are passed as parameter to OperatorKernel::execute() and
         * provides ways for the executing kernel to communicate with its 
         * surroundings.
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
             * \throws Interrupt 
             */
            virtual void testForInterrupt() = 0;
            
            /**
             * Sleep for the given amount of microseconds. The function
             * throws Interrup if the stream has been stopped while sleeping.
             * \throws Interrupt
             */
            virtual void sleep(const unsigned int microseconds) = 0;
            
            /**
             * Receives input data from the provider. The functions waits until
             * the requirements formulated in \c mapper have are fulfilled.
             * If the stream is stopped during waiting Interrupt is thrown. 
             * \throws Interrupt
             */
            virtual void receiveInputData(const Id2DataMapper& mapper) = 0;
            
            /**
             * Sends input data from the provider. The functions waits until
             * the requirements formulated in \c mapper have are fulfilled.
             * If the stream is stopped during waiting Interrupt is thrown. 
             * \throws Interrupt
             */
            virtual void sendOutputData(const Id2DataMapper& mapper) = 0;
        };
    }
}

#endif // STROMX_CORE_DATAPROVIDER_H