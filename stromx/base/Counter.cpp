/* 
*  Copyright 2011 Matthias Fuchs
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

#include <typeinfo>
#include "Counter.h"
#include <stromx/core/DataProvider.h>
#include <stromx/core/Id2DataPair.h>
#include <stromx/core/OperatorException.h>

namespace stromx
{
    using namespace core;

    namespace base
    {
        const std::string Counter::TYPE("Counter");
        const std::string Counter::PACKAGE(STROMX_BASE_PACKAGE_NAME);
        const Version Counter::VERSION(BASE_VERSION_MAJOR, BASE_VERSION_MINOR, BASE_VERSION_PATCH);
        
        Counter::Counter()
        : OperatorKernel(TYPE, PACKAGE, VERSION, setupInputs(), setupOutputs(), setupParameters()),
            m_counter(0)
        {
        }

        void Counter::setParameter(unsigned int id, const Data& value)
        {
            throw WrongParameterId(id, *this);
        }

        const Data& Counter::getParameter(const unsigned int id) const
        {
            throw WrongParameterId(id, *this);
        } 
        
        void Counter::activate()
        {
            m_counter = 0;
        }
        
        void Counter::execute(DataProvider& provider)
        {
            Data* outData = new UInt32(m_counter);
            
            m_counter++;
            
            Id2DataPair outputDataMapper(OUTPUT, DataContainer(outData));
            provider.sendOutputData(outputDataMapper);
        }
        
        const std::vector<const Description*> Counter::setupInputs()
        {
            std::vector<const Description*> inputs;

            return inputs;
        }
        
        const std::vector<const Description*> Counter::setupOutputs()
        {
            std::vector<const Description*> outputs;
            
            Description* output = new Description(OUTPUT, DataVariant::UINT_32);
            output->setDoc("Output");
            outputs.push_back(output);
            
            return outputs;
        }
        
        const std::vector<const Parameter*> Counter::setupParameters()
        {
            std::vector<const Parameter*> parameters;
            
            return parameters;
        }
    } 
}
