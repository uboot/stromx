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

#ifndef STROMX_RUNTIME_REPEAT_H
#define STROMX_RUNTIME_REPEAT_H

#include "stromx/runtime/DataContainer.h"
#include "stromx/runtime/OperatorKernel.h"
#include "stromx/runtime/Primitive.h"
#include "stromx/runtime/Runtime.h"

namespace stromx
{
    namespace runtime
    {
        class DataContainer;
    }

    namespace runtime
    {
        /** \brief Repeats outputting the input data. */
        class STROMX_RUNTIME_API Repeat : public OperatorKernel
        {
            STROMX_OPERATOR_KERNEL
            
        public:
            enum DataId
            {
                INPUT,
                NUM_ITERATIONS,
                OUTPUT,
                OUTPUT_OFFSET = 2,
                PARAMETER_OFFSET = 3
            };
            
            Repeat();
            
            virtual OperatorKernel* clone() const { return new Repeat; }
            virtual void activate();
            virtual void deactivate();
            virtual void execute(runtime::DataProvider& provider);
            
        private:
            static const std::vector<const runtime::Description*> setupOutputs();
            const std::vector<const runtime::Description*> setupInputs();
            
            static const std::string TYPE;
            static const std::string PACKAGE;
            static const runtime::Version VERSION;
            
            
            uint64_t m_numIterations;
            uint64_t m_currentIteration;
            DataContainer m_currentData;
        };       
    }
}

#endif // STROMX_RUNTIME_REPEAT_H
