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

#ifndef STROMX_RUNTIME_DATAOPERATORBASE_H
#define STROMX_RUNTIME_DATAOPERATORBASE_H

#include "stromx/runtime/Config.h"
#include "stromx/runtime/OperatorKernel.h"
#include "stromx/runtime/Primitive.h"
#include "stromx/runtime/RecycleAccess.h"

namespace stromx
{
    namespace runtime
    {
        /** \brief Outputs a constant value. */
        class STROMX_RUNTIME_API DataOperatorBase : public OperatorKernel
        {
        public:
            enum OutputId
            {
                OUTPUT
            };
            
            enum ParameterId
            {
                DATA_TYPE,
                VALUE,
                NUM_BASE_PARAMETERS
            };
            
            DataOperatorBase(const std::string & type, const std::string & package,
                const Version & version, const std::vector<const Parameter*>& parameters);
            virtual ~DataOperatorBase();
            
            virtual void setParameter(const unsigned int id, const runtime::Data& value);
            virtual const DataRef getParameter(const unsigned int id) const;
            virtual void initialize();
            
        protected:
            static const std::vector<const runtime::Parameter*> setupInitParameters();
            
            virtual const std::vector<const runtime::Parameter*> setupParameters();
            const Data* valuePtr() const { return m_value; }
            Data* resetValuePtr();
            
        private:
            static const std::vector<const runtime::Description*> setupInputs();
            const std::vector<const runtime::Description*> setupOutputs();
            void setDataType(const runtime::Enum & value);
            
            Enum m_type;
            Data* m_value;
        };       
    }
}

#endif // STROMX_RUNTIME_DATAOPERATORBASE_H
