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

#ifndef STROMX_RUNTIME_QUEUE_H
#define STROMX_RUNTIME_QUEUE_H

#include <deque>
#include "Config.h"
#include "DataContainer.h"
#include "OperatorKernel.h"
#include "Primitive.h"

namespace stromx
{
    namespace runtime
    {
        /** \brief A data queue of configurable length. */
        class STROMX_RUNTIME_API Queue : public OperatorKernel
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
                SIZE
            };
            
            Queue();
            
            virtual OperatorKernel* clone() const { return new Queue; }
            virtual void setParameter(const unsigned int id, const runtime::Data& value);
            virtual const DataRef getParameter(const unsigned int id) const;
            virtual void deactivate();
            virtual void execute(runtime::DataProvider& provider);
            
        private:
            static const std::vector<const runtime::Description*> setupInputs();
            static const std::vector<const runtime::Description*> setupOutputs();
            static const std::vector<const runtime::Parameter*> setupParameters();
            
            static const std::string TYPE;
            static const std::string PACKAGE;
            static const runtime::Version VERSION;                       
            
            runtime::UInt32 m_size;
            
            std::deque<runtime::DataContainer> m_deque;
        };
    }
}

#endif // STROMX_RUNTIME_QUEUE_H
