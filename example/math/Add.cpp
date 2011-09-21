#include "Add.h"

#include <strom/Primitive.h>
#include <strom/OperatorException.h>
#include <strom/DataContainer.h>
#include <strom/DataProvider.h>
#include <strom/Id2DataPair.h>
#include <strom/Id2DataComposite.h>
#include <strom/ReadAccess.h>

namespace math
{
    const std::string Add::TYPE("Add");
    const std::string Add::PACKAGE("Math");
    const strom::Version Add::VERSION(1, 0);
    
    Add::Add()
      : OperatorKernel(TYPE, PACKAGE, VERSION, setupInputs(), setupOutputs(), setupParameters())
    {
    }

    void Add::setParameter(unsigned int id, const strom::Data& value)
    {
        try
        {
            switch(id)
            {
            case OFFSET:
                m_offset = dynamic_cast<const strom::UInt32&>(value);
                break;
            default:
                throw strom::WrongParameterId(id, *this);
            }
        }
        catch(std::bad_cast&)
        {
            throw strom::WrongParameterId(id, *this);
        }
    }

    const strom::Data& Add::getParameter(unsigned int id)
    {
        switch(id)
        {
        case OFFSET:
            return m_offset;
        default:
            throw strom::WrongParameterId(id, *this);
        }
    }  
    
    void Add::execute(strom::DataProvider& provider)
    {
        strom::Id2DataPair inputMapper(INPUT);
        provider.receiveInputData(inputMapper);
        
        strom::ReadAccess<strom::UInt32> input(inputMapper.data());
        
        strom::Data* result = new strom::UInt32((unsigned int)(input()) + (unsigned int)(m_offset));
        
        strom::DataContainer outContainer(result);
       
        strom::Id2DataPair output(OUTPUT, outContainer);
        provider.sendOutputData(output);
    }
    
    const std::vector<const strom::Description*> Add::setupInputs()
    {
        std::vector<const strom::Description*> inputs;
        
        strom::Description* input = new strom::Description(INPUT, strom::DataVariant::UINT_32);
        input->setName("Input");
        inputs.push_back(input);
        
        return inputs;
    }
    
    const std::vector<const strom::Description*> Add::setupOutputs()
    {
        std::vector<const strom::Description*> outputs;
        
        strom::Description* output = new strom::Description(OUTPUT, strom::DataVariant::UINT_32);
        output->setName("Output");
        outputs.push_back(output);
        
        return outputs;
    }
    
    const std::vector<const strom::Parameter*> Add::setupParameters()
    {
        std::vector<const strom::Parameter*> parameters;
        
        strom::Parameter* offset = new strom::Parameter(OFFSET, strom::DataVariant::UINT_32);
        offset->setName("Offset");
        offset->setAccessMode(strom::Parameter::ACTIVATED_WRITE);
        parameters.push_back(offset);
                                    
        return parameters;
    }
} 
