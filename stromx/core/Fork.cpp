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

#include "Fork.h"
#include "DataProvider.h"
#include "Id2DataPair.h"
#include "Id2DataComposite.h"
#include "NumericParameter.h"
#include "OperatorException.h"
#include <boost/assert.hpp>
#include <boost/lexical_cast.hpp>

namespace stromx
{
    namespace core
    {
        const std::string Fork::TYPE("Fork");
        const std::string Fork::PACKAGE(STROMX_CORE_PACKAGE_NAME);
        const Version Fork::VERSION(STROMX_VERSION_MAJOR, STROMX_VERSION_MINOR, STROMX_VERSION_PATCH);
        const unsigned int Fork::MIN_OUTPUTS = 2;
        const unsigned int Fork::MAX_OUTPUTS = 4;
        
        Fork::Fork()
          : OperatorKernel(TYPE, PACKAGE, VERSION, setupParameters())
        {
        }
        
        void Fork::setParameter(unsigned int id, const core::Data& value)
        {
            UInt32 uintValue;
            
            switch(id)
            {
            case NUM_OUTPUTS:
                uintValue = dynamic_cast<const UInt32 &>(value);
                if(uintValue > MAX_OUTPUTS)
                    throw WrongParameterValue(parameter(NUM_OUTPUTS), *this, "Too many outputs.");
                m_numOutputs = uintValue;
                break;
            default:
                throw WrongParameterId(id, *this);
            }
        }

        const Data& Fork::getParameter(const unsigned int id) const
        {
            switch(id)
            {
            case NUM_OUTPUTS:
                return m_numOutputs;
            default:
                throw WrongParameterId(id, *this);
            }
        }
        
        void Fork::initialize()
        {
            stromx::core::OperatorKernel::initialize(setupInputs(),
                                                     setupOutputs(),
                                                     std::vector<const Parameter*>());
        }
        
        void Fork::execute(DataProvider& provider)
        {
            Id2DataPair input(INPUT);
            
            provider.receiveInputData(input);
            
            unsigned int numOutputs = (unsigned int)(m_numOutputs);
            BOOST_ASSERT(MIN_OUTPUTS >= 2);
            
            std::vector<Id2DataPair> outputPairs;
            for(unsigned int i = 0; i < numOutputs; ++i)
                outputPairs[i] = Id2DataPair(i, input.data());
            
//             Id2DataComposite output = Id2DataPair(0, input.data()) && Id2DataPair(1, input.data());
//             for(unsigned int i = 2; i < numOutputs; ++i)
//                 output = output || Id2DataPair(i, input.data());
        }
        
        const std::vector<const Description*> Fork::setupInputs()
        {
            std::vector<const Description*> inputs;
            Description* input = new Description(INPUT, DataVariant::DATA);
            input->setDoc("Input");
            inputs.push_back(input);
            
            return inputs;
        }
        
        const std::vector<const Description*> Fork::setupOutputs() const
        {
            std::vector<const Description*> outputs;
            
            for(unsigned int i = 0; i < (unsigned int)(m_numOutputs); ++i)
            {
                Description* output = new Description(i, DataVariant::DATA);
                output->setDoc("Input " + boost::lexical_cast<std::string>(i));
                outputs.push_back(output);
            }
            
            return outputs;
        }
        
        const std::vector<const Parameter*> Fork::setupParameters()
        {
            std::vector<const core::Parameter*> parameters;
            
            NumericParameter<UInt32>* numOutputs = new NumericParameter<UInt32>(NUM_OUTPUTS, DataVariant::UINT_32);
            numOutputs->setDoc("Number of outputs");
            numOutputs->setAccessMode(core::Parameter::NONE_WRITE);
            numOutputs->setMin(UInt32(MIN_OUTPUTS));
            numOutputs->setMax(UInt32(MAX_OUTPUTS));
            parameters.push_back(numOutputs);
            
            return parameters;
        }
    }
}
