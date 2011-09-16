#include "Add.h"

#include <strom/Primitive.h>
#include <strom/OperatorException.h>
#include <strom/DataContainer.h>
#include <strom/DataProvider.h>
#include <strom/Id2DataPair.h>
#include <strom/Id2DataComposite.h>
#include <strom/ReadAccess.h>

using namespace strom;

namespace math
{
    const std::string Add::TYPE("Add");
    const std::string Add::PACKAGE("Math");
    const Version Add::VERSION(1, 0);
    
    Add::Add()
      : OperatorKernel(TYPE, PACKAGE, VERSION, setupInputs(), setupOutputs(), setupParameters())
    {
    }

    void Add::setParameter(unsigned int id, const Data& value)
    {
        try
        {
            switch(id)
            {
            case OFFSET:
                m_offset = dynamic_cast<const UInt32&>(value);
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

    const Data& Add::getParameter(unsigned int id)
    {
        switch(id)
        {
        case OFFSET:
            return m_offset;
        default:
            throw WrongParameterId(id, *this);
        }
    }  
    
    void Add::execute(DataProvider& provider)
    {
        Id2DataPair inputMapper(INPUT);
        provider.receiveInputData(inputMapper);
        
        ReadAccess<UInt32> input(inputMapper.data());
        
        Data* result = new UInt32((unsigned int)(input()) + (unsigned int)(m_offset));
        
        DataContainer outContainer(result);
       
        Id2DataPair output(OUTPUT, outContainer);
        provider.sendOutputData(output);
    }
    
    const std::vector<const strom::Description*> Add::setupInputs()
    {
        std::vector<const Description*> inputs;
        
        Description* input = new Description(INPUT, DataVariant::UINT_32);
        input->setName("Input");
        inputs.push_back(input);
        
        return inputs;
    }
    
    const std::vector<const Description*> Add::setupOutputs()
    {
        std::vector<const Description*> outputs;
        
        Description* output = new Description(OUTPUT, DataVariant::UINT_32);
        output->setName("Output");
        outputs.push_back(output);
        
        return outputs;
    }
    
    const std::vector<const Parameter*> Add::setupParameters()
    {
        std::vector<const strom::Parameter*> parameters;
        
        Parameter* offset = new Parameter(OFFSET, DataVariant::UINT_32);
        offset->setName("Offset");
        offset->setAccessMode(strom::Parameter::ACTIVATED_WRITE);
        parameters.push_back(offset);
                                    
        return parameters;
    }
} 
