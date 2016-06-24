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

#ifndef STROMX_RUNTIME_COMPARE_H
#define STROMX_RUNTIME_COMPARE_H

#include "stromx/runtime/Config.h"
#include "stromx/runtime/OperatorKernel.h"
#include "stromx/runtime/Primitive.h"

namespace stromx
{
    namespace runtime
    {
        /** \brief Compare operation of floating point values. */
        class STROMX_RUNTIME_API Compare : public OperatorKernel
        {
        public:
            
            enum DataId
            {
                RESULT,
                NUMBER_1,
                NUMBER_2,
                COMPARE_TO_INPUT,
                COMPARISON_TYPE,
                EPSILON,
                PARAMETER_VALUE
            };
            
            enum CompareType
            {
                LESS,
                LESS_OR_EQUAL,
                GREATER,
                GREATER_OR_EQUAL,
                EQUAL,
                NOT_EQUAL
            };
            
            Compare();
            
            virtual OperatorKernel* clone() const { return new Compare; }
            virtual void setParameter(const unsigned int id, const runtime::Data& value);
            virtual const DataRef getParameter(const unsigned int id) const;
            virtual void execute(runtime::DataProvider& provider);
            virtual void initialize();
            
        private:
            static const std::vector<const runtime::Output*> setupOutputs();
            static const std::vector<const runtime::Parameter*> setupInitParameters();
            
            static const std::string TYPE;
            static const std::string PACKAGE;
            static const runtime::Version VERSION;
            
            const std::vector<const runtime::Input*> setupInputs();
            const std::vector<const runtime::Parameter*> setupParameters();
            
            Bool m_compareToInput;
            Enum m_comparisonType;
            Float64 m_value;
            Float64 m_epsilon;
        };
    }
}

#endif // STROMX_RUNTIME_COMPARE_H
