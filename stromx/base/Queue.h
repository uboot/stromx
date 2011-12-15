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

#ifndef STROMX_BASE_QUEUE_H
#define STROMX_BASE_QUEUE_H

#include <deque>
#include "Config.h"
#include <stromx/core/DataContainer.h>
#include <stromx/core/OperatorKernel.h>
#include <stromx/core/Primitive.h>

namespace stromx
{
    namespace base
    {
        /** \brief A data queue of configurable length. */
        class STROMX_BASE_API Queue : public core::OperatorKernel
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
            
            virtual OperatorKernel* const clone() const { return new Queue; }
            virtual void setParameter(const unsigned int id, const core::Data& value);
            virtual const core::Data& getParameter(const unsigned int id) const;
            virtual void deactivate();
            virtual void execute(core::DataProvider& provider);
            
        private:
            static const std::vector<const core::Description*> setupInputs();
            static const std::vector<const core::Description*> setupOutputs();
            static const std::vector<const core::Parameter*> setupParameters();
            
            static const std::string TYPE;
            static const std::string PACKAGE;
            static const core::Version VERSION;                       
            
            core::UInt32 m_size;
            
            std::deque<core::DataContainer> m_deque;
        };
    }
}

#endif // STROMX_BASE_QUEUE_H
