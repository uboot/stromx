/* 
*  Copyright 2014 Matthias Fuchs
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

#ifndef STROMX_RASPI_READGPIO_H
#define STROMX_RASPI_READGPIO_H

#include "stromx/raspi/Config.h"
#include "stromx/runtime/OperatorKernel.h"
#include "stromx/runtime/Primitive.h"

namespace stromx
{
    namespace raspi
    {
        class STROMX_RASPI_API ReadGpio : public runtime::OperatorKernel
        {
        public:
            enum OutputId
            {
                OUTPUT
            };
            
            ReadGpio();
            
            virtual runtime::OperatorKernel* clone() const { return new ReadGpio; }
            virtual void setParameter(const unsigned int id, const runtime::Data& value);
            virtual const runtime::DataRef getParameter(const unsigned int id) const;
            virtual void execute(runtime::DataProvider& provider);
            
        private:
            static const std::vector<const runtime::Description*> setupInputs();
            static const std::vector<const runtime::Description*> setupOutputs();
            static const std::vector<const runtime::Parameter*> setupParameters();
            
            static const std::string TYPE;
            static const std::string PACKAGE;
            static const runtime::Version VERSION;
        };
    }
}

#endif // STROMX_RASPI_READGPIO_H
