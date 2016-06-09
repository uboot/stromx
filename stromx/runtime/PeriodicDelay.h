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

#ifndef STROMX_EXAMPLE_PERIODICDELAY_H
#define STROMX_EXAMPLE_PERIODICDELAY_H

#include "stromx/runtime/Config.h"
#include "stromx/runtime/OperatorKernel.h"
#include "stromx/runtime/Primitive.h"

namespace stromx
{
    namespace runtime
    {
        class DataContainer;
        
        namespace impl
        {
            struct BoostSystemTime;
        }
        
        /** \brief Periodically delays the execution for a defined amount of time. */
        class STROMX_RUNTIME_API PeriodicDelay : public OperatorKernel
        {
            STROMX_OPERATOR_KERNEL
            
        public:
            enum DataId
            {
                INPUT,
                OUTPUT,
                PERIOD,
                OUTPUT_OFFSET = 1,
                PARAMETER_OFFSET = 2
            };
            
            PeriodicDelay();
            virtual ~PeriodicDelay();
            
            virtual OperatorKernel* clone() const { return new PeriodicDelay; }
            virtual void setParameter(const unsigned int id, const runtime::Data& value);
            virtual const DataRef getParameter(const unsigned int id) const;
            virtual void execute(runtime::DataProvider& provider);
            virtual void activate();
            
        private:
            explicit PeriodicDelay(const PeriodicDelay &);
            
            static const std::vector<const runtime::Description*> setupInputs();
            static const std::vector<const runtime::Description*> setupOutputs();
            static const std::vector<const runtime::Parameter*> setupParameters();
            
            static const std::string TYPE;
            static const std::string PACKAGE;
            static const runtime::Version VERSION; 
            
            runtime::UInt32 m_period;
            impl::BoostSystemTime* m_nextTrigger;
        };
    }
}

#endif // STROMX_EXAMPLE_PERIODICDELAY_H
