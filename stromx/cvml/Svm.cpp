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

#include "stromx/cvml/Svm.h"

namespace stromx
{
    namespace cvml
    {        
        const std::string Svm::TYPE("Svm");
        
        const std::string Svm::PACKAGE(STROMX_CVML_PACKAGE_NAME);
        const runtime::Version Svm::VERSION(0, 1, 0);
        
        Svm::Svm()
          : OperatorKernel(TYPE, PACKAGE, VERSION, setupInputs(), setupOutputs(), setupParameters())
        {
        }
        
        
        const runtime::DataRef Svm::getParameter(const unsigned int id)
        {
        }
        
        void Svm::setParameter(const unsigned int id, const runtime::Data& value)
        {
        }
        
        void Svm::execute(runtime::DataProvider& provider)
        {
        }
        
        
        const std::vector<const runtime::Description*> Svm::setupInputs()
        {
            std::vector<const runtime::Description*> inputs;
            
            return inputs;
        }
        
        const std::vector<const runtime::Description*> Svm::setupOutputs()
        {
            std::vector<const runtime::Description*> outputs;
            
            return outputs;
        }
        
        const std::vector<const runtime::Parameter*> Svm::setupParameters()
        {
            std::vector<const runtime::Parameter*> parameters;
                                        
            return parameters;
        }
    } 
}
