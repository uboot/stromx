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

#include <boost/thread/thread.hpp>
#include <typeinfo>
#include "stromx/runtime/DataProvider.h"
#include "stromx/runtime/Id2DataComposite.h"
#include "stromx/runtime/Id2DataPair.h"
#include "stromx/runtime/OperatorException.h"
#include "stromx/runtime/ParameterGroup.h"
#include "stromx/runtime/Primitive.h"
#include "stromx/runtime/test/TestOperator.h"

namespace stromx
{
    namespace runtime
    {
        const std::string TestOperator::TYPE("TestOperator");
        const std::string TestOperator::PACKAGE("TestPackage");
        const Version TestOperator::VERSION(0, 1, 0);
        
        TestOperator::TestOperator()
          : OperatorKernel(TYPE, PACKAGE, VERSION, setupInitInputs(), setupInitOutputs(), setupInitParameters()),
            m_sleepTime(300),
            m_bufferSize(1000),
            m_throwException(false),
            m_numExecutes(0)
        {
        }
        
        void TestOperator::initialize()
        {
            OperatorKernel::initialize(setupInputs(), setupOutputs(), setupParameters());
        }

        void TestOperator::setParameter(unsigned int id, const runtime::Data& value)
        {
            try
            {
                switch(id)
                {
                case BUFFER_SIZE:
                    m_bufferSize = data_cast<UInt32>(value);
                    break;
                case SLEEP_TIME:
                    m_sleepTime = data_cast<UInt32>(value);
                    break;
                case THROW_EXCEPTION:
                    m_throwException = data_cast<Bool>(value);
                    break;
                case TEST_DATA:
                    m_testData = data_cast<TestData>(value);
                    break;
                default:
                    throw WrongParameterId(id, *this);
                }
            }
            catch(std::bad_cast&)
            {
                throw WrongParameterType(*parameters()[id], *this);
            }
        }

        const DataRef TestOperator::getParameter(const unsigned int id) const
        {
            switch(id)
            {
            case BUFFER_SIZE:
                return m_bufferSize;
            case SLEEP_TIME:
                return m_sleepTime;
            case THROW_EXCEPTION:
                return m_throwException;
            case TEST_DATA:
                return m_testData;
            default:
                throw WrongParameterId(id, *this);
            }
        }  
        
        void TestOperator::execute(DataProvider& provider)
        {
            if(m_throwException)
            {
                throw InternalError("Funny exception.");
            }
                
            Id2DataPair input1(INPUT_1);
            Id2DataPair input2(INPUT_2);
            
            provider.receiveInputData(input1 && input2);
            
             // execute...
            m_numExecutes++;
            boost::this_thread::sleep_for(boost::chrono::milliseconds(m_sleepTime));
            
            Id2DataPair output1(OUTPUT_1, input1.data());
            Id2DataPair output2(OUTPUT_2, input2.data());
            provider.sendOutputData(output1 && output2);
        }
        
        const std::vector< const Description* > TestOperator::setupInitInputs()
        {
            std::vector<const Description*> inputs;
            inputs.push_back(new Description(INPUT_1, Variant::NONE));
            
            return inputs;
        }
                
        const std::vector<const Description*> TestOperator::setupInputs()
        {
            std::vector<const Description*> inputs;
            inputs.push_back(new Description(INPUT_2, Variant::NONE));
            
            return inputs;
        }
        
        const std::vector<const Description*> TestOperator::setupInitOutputs()
        {
            std::vector<const Description*> outputs;
            outputs.push_back(new Description(OUTPUT_1, TestData::TEST_DATA));
            
            return outputs;
        }
        
        const std::vector<const Description*> TestOperator::setupOutputs()
        {
            std::vector<const Description*> outputs;
            outputs.push_back(new Description(OUTPUT_2, TestData::TEST_DATA));
            
            return outputs;
        }
        
        const std::vector<const Parameter*> TestOperator::setupParameters()
        {
            std::vector<const Parameter*> parameters;
            Parameter* param = new Parameter(SLEEP_TIME, Variant::UINT_32);
            param->setAccessMode(Parameter::INITIALIZED_WRITE);
            parameters.push_back(param);
            
            param = new Parameter(THROW_EXCEPTION, Variant::BOOL);
            param->setAccessMode(Parameter::ACTIVATED_WRITE);
            parameters.push_back(param);
            
            ParameterGroup* group = new ParameterGroup(DATA_GROUP);
            parameters.push_back(group);
            
            param = new Parameter(TEST_DATA, TestData::TEST_DATA, group);
            param->setAccessMode(Parameter::ACTIVATED_WRITE);
            parameters.push_back(param);
            
            return parameters;
        }
        
        const std::vector<const Parameter*> TestOperator::setupInitParameters()
        {
            std::vector<const Parameter*> parameters;
            Parameter* param = new Parameter(BUFFER_SIZE, Variant::UINT_32);
            param->setAccessMode(Parameter::NONE_WRITE);
            parameters.push_back(param);
                                        
            return parameters;
        }
    }
}
