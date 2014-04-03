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

#include "stromx/raspi/ReadGpio.h"

#include "stromx/runtime/OperatorException.h"

namespace stromx
{
    using namespace runtime;
    
    namespace raspi
    {
        const std::string ReadGpio::TYPE("ReadGpio");
        const std::string ReadGpio::PACKAGE(STROMX_RASPI_PACKAGE_NAME);
        const Version ReadGpio::VERSION(0, 1, 0);
        
        ReadGpio::ReadGpio()
        : OperatorKernel(TYPE, PACKAGE, VERSION, setupInputs(), setupOutputs(), setupParameters())
        {
        }

        void ReadGpio::setParameter(unsigned int id, const Data&)
        {
            throw WrongParameterId(id, *this);
        }

        const DataRef ReadGpio::getParameter(const unsigned int id) const
        {
            throw WrongParameterId(id, *this);
        } 
        
        void ReadGpio::execute(DataProvider& provider)
        {
        }
        
        const std::vector<const Description*> ReadGpio::setupInputs()
        {
            std::vector<const Description*> inputs;

            return inputs;
        }
        
        const std::vector<const Description*> ReadGpio::setupOutputs()
        {
            std::vector<const Description*> outputs;;
            
            return outputs;
        }
        
        const std::vector<const Parameter*> ReadGpio::setupParameters()
        {
            std::vector<const Parameter*> parameters;
            
            return parameters;
        }
    } 
}
