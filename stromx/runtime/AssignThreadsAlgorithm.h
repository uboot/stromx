/* 
*  Copyright 2015 Matthias Fuchs
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

#ifndef STROMX_RUNTIME_ASSIGNTHREADSALGORITHM_H
#define STROMX_RUNTIME_ASSIGNTHREADSALGORITHM_H

#include "stromx/runtime/Config.h"

namespace stromx
{
    namespace runtime
    {
        class Stream;
        
        /** \brief Algorithm which assigns threads to the stream. */
        class STROMX_RUNTIME_API AssignThreadsAlgorithm
        {
        public:
            /** Constructs a thread assigning algorithm. */
            AssignThreadsAlgorithm() {}
            
            /**
             * Creates threads and assigns the connected inputs of the stream to
             * them such that connectors with different operator threads are 
             * never part of the same thread.
             */ 
            void apply(Stream & stream);
        };
    }
}

#endif // STROMX_RUNTIME_ASSIGNTHREADSALGORITHM_H
