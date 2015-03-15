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

#ifndef STROMX_RUNTIME_ITERATE_H
#define STROMX_RUNTIME_ITERATE_H

#include "stromx/runtime/DataContainer.h"
#include "stromx/runtime/OperatorKernel.h"

#include <deque>

namespace stromx
{
    namespace runtime
    {
        /** \brief Iterates over the entries of an input list. */
        class STROMX_RUNTIME_API Iterate : public OperatorKernel
        {
        public:
            enum InputId
            {
                INPUT
            };
            
            enum OutputId
            {
                OUTPUT_DATA,
                OUTPUT_NUM_ITEMS
            };
            
            Iterate();
            
            virtual OperatorKernel* clone() const { return new Iterate; }
            virtual void setParameter(const unsigned int id, const Data& value);
            const DataRef getParameter(const unsigned int id) const;
            virtual void execute(DataProvider& provider);
            virtual void activate();
            virtual void deactivate();
            
        private:
            static const std::vector<const Description*> setupInputs();
            const std::vector<const Description*> setupOutputs() const;
            
            static const std::string TYPE;
            static const std::string PACKAGE;
            static const Version VERSION;
            
            std::deque<DataContainer> m_storedItems;
        };
    }
}

#endif // STROMX_RUNTIME_ITERATE_H
