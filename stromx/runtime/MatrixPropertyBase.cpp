/* 
*  Copyright 2016 Matthias Fuchs
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

#include "stromx/runtime/MatrixPropertyBase.h"

#include "stromx/runtime/DataContainer.h"
#include "stromx/runtime/DataProvider.h"
#include "stromx/runtime/Id2DataPair.h"
#include "stromx/runtime/Locale.h"
#include "stromx/runtime/ReadAccess.h"
#include "stromx/runtime/Variant.h"

namespace stromx
{
    namespace runtime
    {
        const std::string MatrixPropertyBase::PACKAGE(STROMX_RUNTIME_PACKAGE_NAME);
        const Version MatrixPropertyBase::VERSION(STROMX_RUNTIME_VERSION_MAJOR, STROMX_RUNTIME_VERSION_MINOR, STROMX_RUNTIME_VERSION_PATCH);
        
        MatrixPropertyBase::MatrixPropertyBase(const std::string & type)
          : OperatorKernel(type, PACKAGE, VERSION)
        {
        }
        
        void MatrixPropertyBase::initialize()
        {
            OperatorKernel::initialize(setupInputs(), setupOutputs(), std::vector<const Parameter*>());
        }
        
        void MatrixPropertyBase::execute(DataProvider& provider)
        {
            Id2DataPair inputMapper(INPUT);
            provider.receiveInputData(inputMapper);
            ReadAccess access(inputMapper.data());
            
            DataContainer outData(getProperty(access.get<Matrix>()));
            
            Id2DataPair outputMapper(PROPERTY, outData);
            provider.sendOutputData(outputMapper);
        }
        
        const std::vector<const Input*> MatrixPropertyBase::setupInputs()
        {
            std::vector<const Input*> inputs;
            Input* input = new Input(INPUT, Variant::MATRIX);
            input->setTitle(L_("Matrix"));
            inputs.push_back(input);
            return inputs;            
        }
        
        const std::vector<const Output*> MatrixPropertyBase::setupOutputs()
        {
            std::vector<const Output*> outputs;
            outputs.push_back(createOutput());
            return outputs;
        }
    } 
}
