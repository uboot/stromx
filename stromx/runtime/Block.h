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

#ifndef STROMX_RUNTIME_BLOCK_H
#define STROMX_RUNTIME_BLOCK_H

#include "stromx/runtime/Runtime.h"
#include "stromx/runtime/OperatorKernel.h"
#include "stromx/runtime/Primitive.h"

namespace stromx
{
    namespace runtime
    {
        namespace impl
        {
            struct BoostConditionVariable;
        }
        
        /** \brief Blocks the execution until a trigger signal is received. */
        class STROMX_RUNTIME_API Block : public OperatorKernel
        {
        public:
            enum DataId
            {
                INPUT,
                TRIGGER_DATA,
                OUTPUT,
                TRIGGER,
                STATE,
                TRIGGER_INPUT
            };
            
            enum BlockState
            {
                PASS_ALWAYS,
                BLOCK_ALWAYS,
                TRIGGER_ACTIVE
            };
            
            Block();
            virtual ~Block();
            
            virtual OperatorKernel* clone() const { return new Block; }
            virtual void setParameter(const unsigned int id, const runtime::Data& value);
            virtual const DataRef getParameter(const unsigned int id) const;
            virtual void execute(runtime::DataProvider& provider);
            virtual void initialize();
            virtual void activate();
            
        private:
            static const std::vector<const runtime::Parameter*> setupInitParameters();
            static const std::vector<const runtime::Output*> setupOutputs();
            const std::vector<const runtime::Input*> setupInputs();
            const std::vector<const runtime::Parameter*> setupParameters();
            
            static const std::string TYPE;
            static const std::string PACKAGE;
            static const runtime::Version VERSION; 
            
            impl::BoostConditionVariable* m_cond;
            bool m_wasTriggered;
            runtime::Enum m_state;
            runtime::Bool m_triggerInput;
        };       
    }
}

#endif // STROMX_RUNTIME_BLOCK_H
