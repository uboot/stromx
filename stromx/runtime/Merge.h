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

#ifndef STROMX_RUNTIME_MERGE_H
#define STROMX_RUNTIME_MERGE_H

#include "stromx/runtime/OperatorKernel.h"

#include <boost/assert.hpp>

namespace stromx
{
    namespace runtime
    {
        class List;
        
        /** \brief Merges over the entries of an input list. */
        class STROMX_RUNTIME_API Merge : public OperatorKernel
        {
        public:
            enum InputId
            {
                INPUT_DATA,
                INPUT_NUM_ITEMS
            };
            
            enum ParameterId
            {
                OUTPUT
            };
            
            Merge();
            
            virtual OperatorKernel* clone() const { return new Merge; }
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
            
            uint64_t m_numItems;
            List* m_list;
        };
    }
}

#endif // STROMX_RUNTIME_MERGE_H
