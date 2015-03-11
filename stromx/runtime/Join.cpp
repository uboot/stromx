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
#include "stromx/runtime/Id2DataComposite.h"
#include "stromx/runtime/Id2DataPair.h"
#include "stromx/runtime/Join.h"
#include "stromx/runtime/NumericParameter.h"
#include "stromx/runtime/OperatorException.h"
#include <boost/assert.hpp>
#include <boost/lexical_cast.hpp>

namespace stromx
{
    namespace runtime
    {
        const std::string Join::TYPE("Join");
        const std::string Join::PACKAGE(STROMX_RUNTIME_PACKAGE_NAME);
        const Version Join::VERSION(0, 1, 0);
        const unsigned int Join::MIN_INPUTS = 2;
        const unsigned int Join::MAX_INPUTS = 4;
        
        Join::Join()
          : OperatorKernel(TYPE, PACKAGE, VERSION, setupParameters()),
            m_numInputs(2)
        {
        }
        
        void Join::setParameter(unsigned int id, const runtime::Data& value)
        {
            UInt32 uintValue;
            
            switch(id)
            {
            case NUM_INPUTS:
                uintValue = data_cast<UInt32>(value);
                if(uintValue < MIN_INPUTS)
                    throw WrongParameterValue(parameter(NUM_INPUTS), *this, "Too little inputs.");
                if(uintValue > MAX_INPUTS)
                    throw WrongParameterValue(parameter(NUM_INPUTS), *this, "Too many inputs.");
                m_numInputs = uintValue;
                break;
            default:
                throw WrongParameterId(id, *this);
            }
        }

        const DataRef Join::getParameter(const unsigned int id) const
        {
            switch(id)
            {
            case NUM_INPUTS:
                return m_numInputs;
            default:
                throw WrongParameterId(id, *this);
            }
        }
        
        void Join::initialize()
        {
            OperatorKernel::initialize(setupInputs(),
                                       setupOutputs(),
                                       std::vector<const Parameter*>());
        }
        
        void Join::execute(DataProvider& provider)
        {
            unsigned int numInputs = (unsigned int)(m_numInputs);
            BOOST_ASSERT(MIN_INPUTS >= 2);
            
            std::vector<Id2DataPair*> inputPairs;
            for(unsigned int i = 0; i < numInputs; ++i)
                inputPairs.push_back(new Id2DataPair(i));
            
            std::vector<Id2DataComposite*> inputComposites;
            inputComposites.push_back(new Id2DataComposite(*inputPairs[0],
                                                           *inputPairs[1],
                                                           Id2DataComposite::OR));
            for(unsigned int i = 1; i < numInputs - 1; ++i)
            {
                inputComposites.push_back(new Id2DataComposite(*inputComposites[i - 1],
                                                               *inputPairs[i + 1],
                                                               Id2DataComposite::OR));
                
                
            }
            
            try
            {
                provider.receiveInputData(*inputComposites.back());
                
                for(std::vector<Id2DataPair*>::const_iterator iter = inputPairs.begin();
                    iter != inputPairs.end();
                    ++iter)
                {
                    if(! (*iter)->data().empty())
                    {
                        Id2DataPair outputMapper(OUTPUT, (*iter)->data());
                        provider.sendOutputData(outputMapper);
                        break;
                    }
                }
                
                for(std::vector<Id2DataPair*>::const_iterator iter = inputPairs.begin();
                    iter != inputPairs.end();
                    ++iter)
                {
                    delete *iter;
                }
                
                for(std::vector<Id2DataComposite*>::const_iterator iter = inputComposites.begin();
                    iter != inputComposites.end();
                    ++iter)
                {
                    delete *iter;
                }
            }
            catch(...)
            {
                for(std::vector<Id2DataPair*>::const_iterator iter = inputPairs.begin();
                    iter != inputPairs.end();
                    ++iter)
                {
                    delete *iter;
                }
                
                for(std::vector<Id2DataComposite*>::const_iterator iter = inputComposites.begin();
                    iter != inputComposites.end();
                    ++iter)
                {
                    delete *iter;
                }
                
                throw;
            }
        }
        
        const std::vector<const Description*> Join::setupOutputs()
        {
            std::vector<const Description*> outputs;
            Description* output = new Description(OUTPUT, Variant::DATA);
            output->setTitle("Output");
            outputs.push_back(output);
            
            return outputs;
        }
        
        const std::vector<const Description*> Join::setupInputs() const
        {
            std::vector<const Description*> inputs;
            
            for(unsigned int i = 0; i < (unsigned int)(m_numInputs); ++i)
            {
                Description* input = new Description(i, Variant::DATA);
                input->setTitle("Input " + boost::lexical_cast<std::string>(i));
                inputs.push_back(input);
            }
            
            return inputs;
        }
        
        const std::vector<const Parameter*> Join::setupParameters()
        {
            std::vector<const runtime::Parameter*> parameters;
            
            NumericParameter<UInt32>* numInputs = new NumericParameter<UInt32>(NUM_INPUTS);
            numInputs->setTitle("Number of inputs");
            numInputs->setAccessMode(runtime::Parameter::NONE_WRITE);
            numInputs->setMin(UInt32(MIN_INPUTS));
            numInputs->setMax(UInt32(MAX_INPUTS));
            parameters.push_back(numInputs);
            
            return parameters;
        }
    }
}
