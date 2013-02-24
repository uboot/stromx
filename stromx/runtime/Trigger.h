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

#ifndef STROMX_EXAMPLE_TRIGGER_H
#define STROMX_EXAMPLE_TRIGGER_H

#include "stromx/runtime/Config.h"
#include "stromx/runtime/OperatorKernel.h"
#include "stromx/runtime/Primitive.h"

namespace stromx
{
    namespace runtime
    {
        class DataContainer;
    }

    namespace runtime
    {
        namespace impl
        {
            struct BoostConditionVariable;
        }
        
        /** \brief Delays the execution until a trigger signal is received. */
        class STROMX_RUNTIME_API Trigger : public OperatorKernel
        {
        public:
            enum InputId
            {
                INPUT
            };
            
            enum OutputId
            {
                OUTPUT
            };
            
            enum ParameterId
            {
                TRIGGER,
                STATE
            };
            
            enum TriggerState
            {
                ALWAYS_PASS,
                ALWAYS_STOP,
                TRIGGER_ACTIVE
            };
            
            Trigger();
            virtual ~Trigger();
            
            virtual OperatorKernel* clone() const { return new Trigger; }
            virtual void setParameter(const unsigned int id, const runtime::Data& value);
            virtual const DataRef getParameter(const unsigned int id) const;
            virtual void execute(runtime::DataProvider& provider);
            
        private:
            explicit Trigger(const Trigger &);
            
            static const std::vector<const runtime::Description*> setupInputs();
            static const std::vector<const runtime::Description*> setupOutputs();
            static const std::vector<const runtime::Parameter*> setupParameters();
            
            static const std::string TYPE;
            static const std::string PACKAGE;
            static const runtime::Version VERSION; 
            
            impl::BoostConditionVariable* m_cond;
            runtime::Enum m_state;
        };       
        
        /** \cond */
        template <>
        class data_traits<Trigger>
        {
        public:
            static const DataVariant & variant() { return DataVariant::TRIGGER; }
        };  
        /** \endcond */
    }
}

#endif // STROMX_EXAMPLE_TRIGGER_H
