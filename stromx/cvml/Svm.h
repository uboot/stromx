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

#ifndef STROMX_CVML_SVM_H
#define STROMX_CVML_SVM_H

#include "stromx/cvml/Cvml.h"
#include "stromx/runtime/OperatorKernel.h"

namespace stromx
{
    namespace cvml
    {
        class STROMX_CVML_API Svm : public runtime::OperatorKernel
        {
        public:            
            Svm();
            virtual ~Svm() {}
            
            virtual OperatorKernel* clone() const { return new Svm; }
            virtual const runtime::DataRef getParameter(const unsigned int id);
            virtual void setParameter(const unsigned int id, const runtime::Data& value);
            virtual void execute(runtime::DataProvider& provider);
            
        private:
            static const std::string TYPE;
            static const std::string PACKAGE;
            static const runtime::Version VERSION; 
            
            static const std::vector<const runtime::Description*> setupInputs();
            static const std::vector<const runtime::Description*> setupOutputs();
            static const std::vector<const runtime::Parameter*> setupParameters();
        };       
    }
}

#endif // STROMX_CVML_SVM_H
