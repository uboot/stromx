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

#ifndef STROMX_RUNTIME_SORTINPUTSALGORITHM_H
#define STROMX_RUNTIME_SORTINPUTSALGORITHM_H

#include "stromx/runtime/Config.h"

namespace stromx
{
    namespace runtime
    {
        class Stream;
        
        /** \brief Algorithm which sorts the input sequence of stream threads */
        class STROMX_RUNTIME_API SortInputsAlgorithm
        {
        public:
            /** Constructs a sorting algorithm. */
            SortInputsAlgorithm() {}
            
            /**
             * Sorts the inputs of each thread in \c stream according to the topology
             * of the stream. If a connection \em from an operator \em to another operator exists
             * the inputs of the first operator will be moved in front of the inputs
             * of the second operator.
             */ 
            void apply(Stream & stream);
        };
    }
}

#endif // STROMX_RUNTIME_SORTINPUTSALGORITHM_H
