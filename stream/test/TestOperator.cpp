#include "TestOperator.h"

#include <stream/Id2DataPair.h>
#include <stream/Id2DataComposite.h>
#include <stream/DataProvider.h>
#include <stream/Primitive.h>
#include <stream/OperatorException.h>

#include <typeinfo>
#include <boost/thread/thread.hpp>


namespace stream
{
    const std::string TestOperator::TYPE("Test");
    const std::string TestOperator::PACKAGE("TestPackage");
    const Version TestOperator::VERSION(0, 1);
    
    TestOperator::TestOperator()
      : OperatorKernel(TYPE, PACKAGE, VERSION, setupInitParameters()),
        m_sleepTime(100),
        m_bufferSize(1000),
        m_numExecutes(0)
    {
    }
    
    void TestOperator::initialize()
    {
        OperatorKernel::initialize(setupInputs(), setupOutputs(), setupParameters());
    }

    void TestOperator::setParameter(unsigned int id, const stream::Data& value)
    {
        try
        {
            const UInt32& v = dynamic_cast<const UInt32&>(value);
            m_sleepTime = v;
        }
        catch(std::bad_cast&)
        {
            throw WrongParameterType(*parameters()[id], *this);
        }
    }

    const Data& TestOperator::getParameter(unsigned int id)
    {
        return m_sleepTime;
    }  
    
    void TestOperator::execute(DataProvider& provider)
    {
        Id2DataPair input1(INPUT_1);
        Id2DataPair input2(INPUT_2);
        
        provider.receiveInputData(input1 && input2);
        
        // execute...
        m_numExecutes++;
        boost::this_thread::sleep(boost::posix_time::microsec(m_sleepTime));
        
        Id2DataPair output1(OUTPUT_1, input1.data());
        Id2DataPair output2(OUTPUT_2, input2.data());
        provider.sendOutputData(output1 && output2);
    }
    
    const std::vector<const Description*> TestOperator::setupInputs()
    {
        std::vector<const Description*> inputs;
        inputs.push_back(new Description(INPUT_1, DataType::NONE));
        inputs.push_back(new Description(INPUT_2, DataType::NONE));
        
        return inputs;
    }
    
    const std::vector<const Description*> TestOperator::setupOutputs()
    {
        std::vector<const Description*> outputs;
        outputs.push_back(new Description(OUTPUT_1, DataType::NONE));
        outputs.push_back(new Description(OUTPUT_2, DataType::NONE));
        
        return outputs;
    }
    
    const std::vector<const Parameter*> TestOperator::setupParameters()
    {
        std::vector<const Parameter*> parameters;
        Parameter* param = new Parameter(SLEEP_TIME, DataType::UINT_32);
        param->setAccessMode(Parameter::INITIALIZED_WRITE);
        parameters.push_back(param);
        
        return parameters;
    }
    
    const std::vector<const Parameter*> TestOperator::setupInitParameters()
    {
        std::vector<const Parameter*> parameters;
        Parameter* param = new Parameter(BUFFER_SIZE, DataType::UINT_32);
        param->setAccessMode(Parameter::NONE_WRITE);
        parameters.push_back(param);
                                       
        return parameters;
    }
}
