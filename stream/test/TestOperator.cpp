#include "TestOperator.h"

#include <Id2DataPair.h>
#include <Id2DataComposite.h>
#include <DataProvider.h>

namespace stream
{
    const std::string TestOperator::NAME("Test Operator");
    const std::string TestOperator::PACKAGE("Test Package");
    const Version TestOperator::VERSION(0, 1);
    
    TestOperator::TestOperator()
      : Operator(NAME, PACKAGE, VERSION, setupInputs(), setupOutputs(), setupParameters()),
        m_sleepTime(1000),
        m_numExecutes(0)
    {
    }

    void TestOperator::setParameter(unsigned int id, const stream::Data& value)
    {
    }

    void TestOperator::getParameter(unsigned int id, Data& value)
    {
    }  
    
    void TestOperator::execute(DataProvider& provider)
    {
        Id2DataPair input1(INPUT_1);
        Id2DataPair input2(INPUT_2);
        
        provider.receiveInputData(input1 && input2);
        
        // execute...
        m_numExecutes++;
        usleep(m_sleepTime * 1000);
        
        Id2DataPair output1(OUTPUT_1, input1.data());
        Id2DataPair output2(OUTPUT_2, input2.data());
        provider.sendOutputData(output1 && output2);
    }

    
    const std::vector< Description > TestOperator::setupInputs()
    {
        std::vector<Description> inputs;
        inputs.push_back(Description(INPUT_1));
        inputs.push_back(Description(INPUT_2));
        
        return inputs;
    }
    
    const std::vector< Description > TestOperator::setupOutputs()
    {
        std::vector<Description> outputs;
        outputs.push_back(Description(OUTPUT_1));
        outputs.push_back(Description(OUTPUT_2));
        
        return outputs;
    }
    
    const std::vector< Parameter > TestOperator::setupParameters()
    {
        std::vector<Parameter> parameters;
        Parameter param(SLEEP_TIME);
        param.setType(DataType::UINT_32);
        param.setInactiveAccessMode(Parameter::READ_WRITE);
        param.setActiveAccessMode(Parameter::READ_WRITE);
        parameters.push_back(param);
                                       
        return parameters;
    }

    
}
