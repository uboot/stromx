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

#include "stromx/runtime/Repeat.h"

#include "stromx/runtime/DataProvider.h"
#include "stromx/runtime/Id2DataPair.h"
#include "stromx/runtime/Id2DataComposite.h"
#include "stromx/runtime/Locale.h"
#include "stromx/runtime/NumericParameter.h"
#include "stromx/runtime/OperatorException.h"
#include "stromx/runtime/ReadAccess.h"

#include <boost/assert.hpp>

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
        const std::string Repeat::TYPE("Repeat");
        
        const std::string Repeat::PACKAGE(STROMX_RUNTIME_PACKAGE_NAME);
        const Version Repeat::VERSION(0, 1, 0);
        
        Repeat::Repeat()
          : OperatorKernel(TYPE, PACKAGE, VERSION, setupInputs(), setupOutputs(), setupParameters()),
            m_currentIteration(0)
        {
        }
        
        void Repeat::setParameter(unsigned int id, const Data& value)
        {
            try
            {
                switch(id)
                {
                case NUM_ITERATIONS:
                    m_numIterations = data_cast<UInt32>(value);
                    break;
                default:
                    throw WrongParameterId(id, *this);
                }
            }
            catch(std::bad_cast&)
            {
                throw WrongParameterType(parameter(id), *this);
            }
        }

        const DataRef Repeat::getParameter(const unsigned int id) const
        {
            switch(id)
            {
            case NUM_ITERATIONS:
                return m_numIterations;
            default:
                throw WrongParameterId(id, *this);
            }
        } 
        
        void Repeat::activate()
        {
            m_currentIteration = 0;
        }
        
        void Repeat::deactivate()
        {
            m_currentData = DataContainer();
        }
        
        void Repeat::execute(DataProvider& provider)
        {
            if (m_currentData.empty())
            {
                Id2DataPair inputMapper(INPUT);
                provider.receiveInputData(inputMapper);
                m_currentData = inputMapper.data();
            }
                
            if (m_currentIteration < m_numIterations)
            {
                BOOST_ASSERT(! m_currentData.empty());
                
                Id2DataPair outputMapper(OUTPUT, m_currentData);
                provider.sendOutputData(outputMapper);
                ++m_currentIteration;
            }
            
            if (m_currentIteration >= m_numIterations)
            {
                m_currentData = DataContainer();
            }
        }
        
        const std::vector<const runtime::Description*> Repeat::setupInputs()
        {
            std::vector<const Description*> inputs;
            
            Description* input = new Description(INPUT, Variant::DATA);
            input->setTitle(L_("Input"));
            input->setOperatorThread(INPUT_THREAD);
            inputs.push_back(input);
            
            return inputs;
        }
        
        const std::vector<const Description*> Repeat::setupOutputs()
        {
            std::vector<const Description*> outputs;
            
            Description* output = new Description(OUTPUT, Variant::DATA);
            output->setTitle(L_("Output"));
            output->setOperatorThread(OUTPUT_THREAD);
            outputs.push_back(output);
            
            return outputs;
        }
        
        const std::vector< const Parameter* > Repeat::setupParameters()
        {
            std::vector<const Parameter*> parameters;
            
            Parameter* numIterations = new NumericParameter<UInt32>(NUM_ITERATIONS);
            numIterations->setTitle(L_("Number of iterations"));
            numIterations->setAccessMode(Parameter::ACTIVATED_WRITE);
            parameters.push_back(numIterations);
            
            return parameters;
        }
    } 
}
