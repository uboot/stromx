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

#ifndef STROMX_RUNTIME_PUSH_H
#define STROMX_RUNTIME_PUSH_H

#include "stromx/runtime/Runtime.h"
#include "stromx/runtime/Primitive.h"
#include "stromx/runtime/impl/DataOperatorBase.h"

namespace stromx
{
    namespace runtime
    {
        namespace impl
        {
            struct BoostConditionVariable;
        }
        
        /** \brief Blocks the execution until the value parameter is set. */
        class STROMX_RUNTIME_API Push : public DataOperatorBase
        {
            STROMX_OPERATOR_KERNEL
            
        public:            
            Push();
            virtual ~Push();
            
            virtual OperatorKernel* clone() const { return new Push; }
            virtual void setParameter(const unsigned int id, const runtime::Data& value);
            virtual void execute(runtime::DataProvider& provider);
            virtual void initialize();
            virtual void activate();
            
        private:
            static const std::string TYPE;
            static const std::string PACKAGE;
            static const runtime::Version VERSION; 
            
            impl::BoostConditionVariable* m_cond;
        };       
    }
}

#endif // STROMX_RUNTIME_PUSH_H
