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
    const std::string TestOperator::NAME("test");
    const std::string TestOperator::PACKAGE("TestPackage");
    const Version TestOperator::VERSION(0, 1);
    
    TestOperator::TestOperator()
      : Operator(NAME, PACKAGE, VERSION, setupInputs(), setupOutputs(), setupParameters()),
        m_sleepTime(100),
        m_numExecutes(0)
    {
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
            throw ParameterTypeException(*parameters()[id], *this);
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
    
    const std::vector<Description*> TestOperator::setupInputs()
    {
        std::vector<Description*> inputs;
        inputs.push_back(new Description(INPUT_1, DataType::NONE));
        inputs.push_back(new Description(INPUT_2, DataType::NONE));
        
        return inputs;
    }
    
    const std::vector<Description*> TestOperator::setupOutputs()
    {
        std::vector<Description*> outputs;
        outputs.push_back(new Description(OUTPUT_1, DataType::NONE));
        outputs.push_back(new Description(OUTPUT_2, DataType::NONE));
        
        return outputs;
    }
    
    const std::vector<Parameter*> TestOperator::setupParameters()
    {
        std::vector<Parameter*> parameters;
        Parameter* param = new Parameter(SLEEP_TIME, DataType::UINT_32);
        param->setInactiveAccessMode(Parameter::READ);
        param->setActiveAccessMode(Parameter::WRITE);
        parameters.push_back(param);
                                       
        return parameters;
    }
}
