#include "AddAndMultiply.h"

#include <stream/Primitive.h>
#include <stream/OperatorException.h>
#include <stream/DataContainer.h>
#include <stream/DataProvider.h>
#include <stream/Id2DataPair.h>
#include <stream/Id2DataComposite.h>
#include <stream/ReadAccess.h>

using namespace stream;

namespace math
{
    const std::string AddAndMultiply::TYPE("AddAndMultiply");
    const std::string AddAndMultiply::PACKAGE("Math");
    const Version AddAndMultiply::VERSION(1, 0);
    
    AddAndMultiply::AddAndMultiply()
      : OperatorKernel(TYPE, PACKAGE, VERSION, setupInputs(), setupOutputs(), setupParameters()),
        m_coefficient(1.0)
    {
    }

    void AddAndMultiply::setParameter(unsigned int id, const Data& value)
    {
        try
        {
            switch(id)
            {
            case COEFFICIENT:
                m_coefficient = dynamic_cast<const Double&>(value);
                break;
            default:
                throw WrongParameterId(id, *this);
            }
        }
        catch(std::bad_cast&)
        {
            throw WrongParameterId(id, *this);
        }
    }

    const Data& AddAndMultiply::getParameter(unsigned int id)
    {
        switch(id)
        {
        case COEFFICIENT:
            return m_coefficient;
        default:
            throw WrongParameterId(id, *this);
        }
    }  
    
    const double AddAndMultiply::extractDouble(const DataContainer& dataContainer)
    {
        ReadAccess access(dataContainer);
        const Data & data = access();
        const UInt32 & streamDouble = dynamic_cast<const UInt32 &>(data);
        
        return double(streamDouble);
    }
    
    void AddAndMultiply::execute(DataProvider& provider)
    {
        Id2DataPair input1(INPUT_1);
        Id2DataPair input2(INPUT_2);
        provider.receiveInputData(input1 && input2);
        
        double sum = (extractDouble(input1.data()) + extractDouble(input2.data()));
        double result = sum * double(m_coefficient);
        
        DataContainer outContainer(new Double(result));
       
        Id2DataPair output(OUTPUT, outContainer);
        provider.sendOutputData(output);
    }
    
    const std::vector<const stream::Description*> AddAndMultiply::setupInputs()
    {
        std::vector<const Description*> inputs;
        
        Description* input1 = new Description(INPUT_1, DataVariant::UINT_32);
        input1->setName("Input 1");
        inputs.push_back(input1);
        
        Description* input2 = new Description(INPUT_2, DataVariant::UINT_32);
        input2->setName("Input 1");
        inputs.push_back(input2);
        
        return inputs;
    }
    
    const std::vector<const Description*> AddAndMultiply::setupOutputs()
    {
        std::vector<const Description*> outputs;
        
        Description* output = new Description(OUTPUT, DataVariant::DOUBLE);
        output->setName("Output");
        outputs.push_back(output);
        
        return outputs;
    }
    
    const std::vector<const Parameter*> AddAndMultiply::setupParameters()
    {
        std::vector<const stream::Parameter*> parameters;
        
        Parameter* coefficient = new Parameter(COEFFICIENT, DataVariant::DOUBLE);
        coefficient->setName("Coefficient");
        coefficient->setAccessMode(stream::Parameter::ACTIVATED_WRITE);
        parameters.push_back(coefficient);
                                    
        return parameters;
    }
} 
