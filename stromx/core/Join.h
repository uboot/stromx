/* 
 *  Copyright 2012 Matthias Fuchs
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

#ifndef STROMX_CORE_JOIN_H
#define STROMX_CORE_JOIN_H

#include "OperatorKernel.h"

namespace stromx
{
    namespace core
    {
        /** \brief Distributes input data to several outputs. */
        class STROMX_CORE_API Join : public OperatorKernel
        {
        public:
            enum OutputId
            {
                OUTPUT
            };
            
            enum ParameterId
            {
                NUM_INPUTS
            };
            
            Join();
            
            virtual OperatorKernel* clone() const { return new Join; }
            virtual void setParameter(const unsigned int id, const Data& value);
            const DataRef getParameter(const unsigned int id) const;
            virtual void initialize();
            virtual void execute(DataProvider& provider);
            
        private:
            static const std::vector<const Description*> setupOutputs();
            static const std::vector<const Parameter*> setupParameters();
            const std::vector<const Description*> setupInputs() const;
            
            static const std::string TYPE;
            static const std::string PACKAGE;
            static const Version VERSION;
            
            static const unsigned int MIN_INPUTS;
            static const unsigned int MAX_INPUTS;
            
            UInt32 m_numInputs;
        };
    }
}

#endif // STROMX_CORE_JOIN_H
