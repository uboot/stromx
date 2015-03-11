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

#include "stromx/runtime/Merge.h"

#include "stromx/runtime/DataProvider.h"
#include "stromx/runtime/Id2DataComposite.h"
#include "stromx/runtime/Id2DataPair.h"
#include "stromx/runtime/Locale.h"
#include "stromx/runtime/NumericParameter.h"
#include "stromx/runtime/OperatorException.h"

namespace stromx
{
    namespace runtime
    {
        const std::string Merge::TYPE("Merge");
        const std::string Merge::PACKAGE(STROMX_RUNTIME_PACKAGE_NAME);
        const Version Merge::VERSION(0, 1, 0);
        
        Merge::Merge() : OperatorKernel(TYPE, PACKAGE, VERSION)
        {
        }
        
        void Merge::setParameter(unsigned int id, const runtime::Data& /*value*/)
        {
            switch(id)
            {
            default:
                throw WrongParameterId(id, *this);
            }
        }

        const DataRef Merge::getParameter(const unsigned int id) const
        {
            switch(id)
            {
            default:
                throw WrongParameterId(id, *this);
            }
        }
        
        void Merge::execute(DataProvider& provider)
        {
            Id2DataPair input(INPUT);
            
            provider.receiveInputData(input);
        }
        
        const std::vector<const Description*> Merge::setupInputs()
        {
            std::vector<const Description*> inputs;
            Description* input = new Description(INPUT, Variant::LIST);
            input->setTitle(L_("Input"));
            inputs.push_back(input);
            
            return inputs;
        }
        
        const std::vector<const Description*> Merge::setupOutputs() const
        {
            std::vector<const Description*> outputs;
            
            Description* output = new Description(OUTPUT, Variant::DATA);
            output->setTitle(L_("List elements"));
            outputs.push_back(output);
            
            return outputs;
        }
    }
}
