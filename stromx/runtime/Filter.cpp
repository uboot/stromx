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

#include "stromx/runtime/Filter.h"

#include "stromx/runtime/DataContainer.h"
#include "stromx/runtime/DataProvider.h"
#include "stromx/runtime/Id2DataPair.h"
#include "stromx/runtime/Id2DataComposite.h"
#include "stromx/runtime/Locale.h"
#include "stromx/runtime/ReadAccess.h"

namespace 
{
    const static unsigned int INPUT_THREAD = 0;
    const static unsigned int OUTPUT_THREAD = 1;
}

namespace stromx
{
    using namespace runtime;

    namespace runtime
    {
        const std::string Filter::TYPE("Filter");
        
        const std::string Filter::PACKAGE(STROMX_RUNTIME_PACKAGE_NAME);
        const Version Filter::VERSION(0, 1, 0);
        
        Filter::Filter()
          : OperatorKernel(TYPE, PACKAGE, VERSION, setupInputs(), setupOutputs())
        {
        }
        
        void Filter::execute(DataProvider& provider)
        {
            Id2DataPair inputMapper(INPUT);
            Id2DataPair flagMapper(FILTER_FLAG);
            
            provider.receiveInputData(inputMapper && flagMapper);
            
            Bool flag = ReadAccess<Bool>(flagMapper.data())();
            
            if (flag)
            {
                Id2DataPair outputMapper(OUTPUT, inputMapper.data());
                provider.sendOutputData(outputMapper);
            }
        }
        
        const std::vector<const runtime::Description*> Filter::setupInputs()
        {
            std::vector<const Description*> inputs;
            
            Description* input = new Description(INPUT, Variant::DATA);
            input->setTitle("Input");
            input->setOperatorThread(INPUT_THREAD);
            inputs.push_back(input);
            
            Description* filterFlag = new Description(FILTER_FLAG, Variant::BOOL);
            filterFlag->setTitle(L_("Filter flag"));
            filterFlag->setOperatorThread(INPUT_THREAD);
            inputs.push_back(filterFlag);
            
            return inputs;
        }
        
        const std::vector<const Description*> Filter::setupOutputs()
        {
            std::vector<const Description*> outputs;
            
            Description* output = new Description(OUTPUT, Variant::DATA);
            output->setTitle("Output");
            output->setOperatorThread(OUTPUT_THREAD);
            outputs.push_back(output);
            
            return outputs;
        }
    } 
}
