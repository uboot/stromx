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

#include <boost/chrono.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/thread.hpp>
#include "stromx/runtime/DataContainer.h"
#include "stromx/runtime/DataProvider.h"
#include "stromx/runtime/Id2DataPair.h"
#include "stromx/runtime/OperatorException.h"
#include "stromx/runtime/PeriodicDelay.h"

namespace stromx
{
    using namespace runtime;
    
    namespace runtime
    {
        /** \cond */
        namespace impl
        {
            struct BoostSystemTime
            {
                boost::chrono::steady_clock::time_point m_time;
            };
        }
        /** \endcond */
        
        const std::string PeriodicDelay::TYPE("PeriodicDelay");
        
        const std::string PeriodicDelay::PACKAGE(STROMX_RUNTIME_PACKAGE_NAME);
        const Version PeriodicDelay::VERSION(0, 1, 0);
        
        PeriodicDelay::PeriodicDelay()
          : OperatorKernel(TYPE, PACKAGE, VERSION, setupInputs(), setupOutputs(), setupParameters()),
            m_period(1000),
            m_nextTrigger(new impl::BoostSystemTime)
        {
        }
        
        PeriodicDelay::~PeriodicDelay()
        {
            delete m_nextTrigger;
        }
        
        void PeriodicDelay::activate()
        {
            m_nextTrigger->m_time = boost::chrono::steady_clock::now() + 
                                    boost::chrono::milliseconds(m_period);
        }


        void PeriodicDelay::setParameter(unsigned int id, const Data& value)
        {
            try
            {
                switch(id)
                {
                case PERIOD:
                    m_period = data_cast<UInt32>(value);
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

        const DataRef PeriodicDelay::getParameter(const unsigned int id) const
        {
            switch(id)
            {
            case PERIOD:
                return m_period;
            default:
                throw WrongParameterId(id, *this);
            }
        }  
        
        void PeriodicDelay::execute(DataProvider& provider)
        {
            using namespace boost::chrono;
            
            Id2DataPair inputDataMapper(INPUT);
            provider.receiveInputData(inputDataMapper);
            
            if(m_period)
            { 
                try
                {
                    provider.unlockParameters();
                    boost::this_thread::sleep_until(m_nextTrigger->m_time);
                    provider.lockParameters();
                }
                catch(boost::thread_interrupted&)
                {
                    throw Interrupt();
                }
            }
            
            // check m_period again because it might have changed while sleeping
            if(m_period)
            {
                milliseconds passedMs = duration_cast<milliseconds>(
                    steady_clock::now() - m_nextTrigger->m_time);
                unsigned int numPeriods = passedMs.count() / m_period + 1;
                m_nextTrigger->m_time += boost::chrono::milliseconds(m_period * numPeriods);
            }
            
            Id2DataPair outputDataMapper(OUTPUT, inputDataMapper.data());
            provider.sendOutputData( outputDataMapper);
        }
        
        const std::vector<const runtime::Description*> PeriodicDelay::setupInputs()
        {
            std::vector<const Description*> inputs;
            
            Description* input = new Description(OUTPUT, Variant::DATA);
            input->setTitle("Input");
            inputs.push_back(input);
            
            return inputs;
        }
        
        const std::vector<const Description*> PeriodicDelay::setupOutputs()
        {
            std::vector<const Description*> outputs;
            
            Description* output = new Description(OUTPUT, Variant::DATA);
            output->setTitle("Output");
            outputs.push_back(output);
            
            return outputs;
        }
        
        const std::vector<const Parameter*> PeriodicDelay::setupParameters()
        {
            std::vector<const runtime::Parameter*> parameters;
            
            Parameter* period = new Parameter(PERIOD, Variant::UINT_32);
            period->setTitle("Period (milliseconds)");
            period->setAccessMode(runtime::Parameter::ACTIVATED_WRITE);
            parameters.push_back(period);
                                        
            return parameters;
        }
    } 
}
