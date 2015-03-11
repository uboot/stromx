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

#include "stromx/runtime/DataProvider.h"
#include "stromx/runtime/Fork.h"
#include "stromx/runtime/Id2DataComposite.h"
#include "stromx/runtime/Id2DataPair.h"
#include "stromx/runtime/NumericParameter.h"
#include "stromx/runtime/OperatorException.h"
#include <boost/assert.hpp>
#include <boost/lexical_cast.hpp>

namespace stromx
{
    namespace runtime
    {
        const std::string Fork::TYPE("Fork");
        const std::string Fork::PACKAGE(STROMX_RUNTIME_PACKAGE_NAME);
        const Version Fork::VERSION(0, 1, 0);
        const unsigned int Fork::MIN_OUTPUTS = 2;
        const unsigned int Fork::MAX_OUTPUTS = 4;
        
        Fork::Fork()
          : OperatorKernel(TYPE, PACKAGE, VERSION, setupParameters()),
            m_numOutputs(2)
        {
        }
        
        void Fork::setParameter(unsigned int id, const runtime::Data& value)
        {
            UInt32 uintValue;
            
            switch(id)
            {
            case NUM_OUTPUTS:
                uintValue = data_cast<UInt32>(value);
                if(uintValue < MIN_OUTPUTS)
                    throw WrongParameterValue(parameter(NUM_OUTPUTS), *this, "Too little outputs.");
                if(uintValue > MAX_OUTPUTS)
                    throw WrongParameterValue(parameter(NUM_OUTPUTS), *this, "Too many outputs.");
                m_numOutputs = uintValue;
                break;
            default:
                throw WrongParameterId(id, *this);
            }
        }

        const DataRef Fork::getParameter(const unsigned int id) const
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
            OperatorKernel::initialize(setupInputs(),
                                       setupOutputs(),
                                       std::vector<const Parameter*>());
        }
        
        void Fork::execute(DataProvider& provider)
        {
            Id2DataPair input(INPUT);
            
            provider.receiveInputData(input);
            
            unsigned int numOutputs = (unsigned int)(m_numOutputs);
            BOOST_ASSERT(MIN_OUTPUTS >= 2);
            
            std::vector<Id2DataPair*> outputPairs;
            for(unsigned int i = 0; i < numOutputs; ++i)
                outputPairs.push_back(new Id2DataPair(i, input.data()));
            
            std::vector<Id2DataComposite*> outputComposites;
            outputComposites.push_back(new Id2DataComposite(*outputPairs[0],
                                                            *outputPairs[1],
                                                            Id2DataComposite::OR));
            
            for(unsigned int i = 1; i < numOutputs - 1; ++i)
            {
                outputComposites.push_back(new Id2DataComposite(*outputComposites[i - 1],
                                                                *outputPairs[i + 1],
                                                                Id2DataComposite::OR));
            }
            
            try
            {
                provider.sendOutputData(*outputComposites.back());
                
                for(std::vector<Id2DataPair*>::const_iterator iter = outputPairs.begin();
                    iter != outputPairs.end();
                    ++iter)
                {
                    delete *iter;
                }
                
                for(std::vector<Id2DataComposite*>::const_iterator iter = outputComposites.begin();
                    iter != outputComposites.end();
                    ++iter)
                {
                    delete *iter;
                }
            }
            catch(...)
            {
                for(std::vector<Id2DataPair*>::const_iterator iter = outputPairs.begin();
                    iter != outputPairs.end();
                    ++iter)
                {
                    delete *iter;
                }
                
                for(std::vector<Id2DataComposite*>::const_iterator iter = outputComposites.begin();
                    iter != outputComposites.end();
                    ++iter)
                {
                    delete *iter;
                }
                
                throw;
            }
        }
        
        const std::vector<const Description*> Fork::setupInputs()
        {
            std::vector<const Description*> inputs;
            Description* input = new Description(INPUT, Variant::DATA);
            input->setTitle("Input");
            inputs.push_back(input);
            
            return inputs;
        }
        
        const std::vector<const Description*> Fork::setupOutputs() const
        {
            std::vector<const Description*> outputs;
            
            for(unsigned int i = 0; i < (unsigned int)(m_numOutputs); ++i)
            {
                Description* output = new Description(i, Variant::DATA);
                output->setTitle("Output " + boost::lexical_cast<std::string>(i));
                outputs.push_back(output);
            }
            
            return outputs;
        }
        
        const std::vector<const Parameter*> Fork::setupParameters()
        {
            std::vector<const runtime::Parameter*> parameters;
            
            NumericParameter<UInt32>* numOutputs = new NumericParameter<UInt32>(NUM_OUTPUTS);
            numOutputs->setTitle("Number of outputs");
            numOutputs->setAccessMode(runtime::Parameter::NONE_WRITE);
            numOutputs->setMin(UInt32(MIN_OUTPUTS));
            numOutputs->setMax(UInt32(MAX_OUTPUTS));
            parameters.push_back(numOutputs);
            
            return parameters;
        }
    }
}
