#include "TestOperator.h"

#include <stream/Id2DataPair.h>
#include <stream/DataProvider.h>

namespace stream
{
    const std::string TestOperator::NAME("Test Operator");
    const Version TestOperator::VERSION(0, 1);
    
    TestOperator::TestOperator(const unsigned int id)
      : Operator(id, NAME, VERSION, setupInputs(), setupOutputs(), setupParameters()),
        m_sleepTime(1000)
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
        usleep(m_sleepTime * 1000);
        
        provider.sendOutputData(input1 && input2);
    }

    
    const std::vector< Descriptor > TestOperator::setupInputs()
    {
        std::vector<Descriptor> inputs;
        inputs.push_back(Descriptor("Input 1", INPUT_1, DataType()));
        inputs.push_back(Descriptor("Input 2", INPUT_2, DataType()));
        
        return inputs;
    }
    
    const std::vector< Descriptor > TestOperator::setupOutputs()
    {
        std::vector<Descriptor> outputs;
        outputs.push_back(Descriptor("Output 1", OUTPUT_1, DataType()));
        outputs.push_back(Descriptor("Output 2", OUTPUT_2, DataType()));
        
        return outputs;
    }
    
    const std::vector< Parameter > TestOperator::setupParameters()
    {
        std::vector<Parameter> parameters;
        parameters.push_back(Parameter("Sleep Time (ms)", SLEEP_TIME,
                                       DataType(), Parameter::READ_WRITE, Parameter::READ_WRITE));
                                       
        return parameters;
    }

    
}
