#include "Add.h"

#include <stromx/core/Primitive.h>
#include <stromx/core/OperatorException.h>
#include <stromx/core/DataProvider.h>
#include <stromx/core/Id2DataPair.h>
#include <stromx/core/Id2DataComposite.h>
#include <stromx/core/ReadAccess.h>

using namespace stromx;

namespace math
{
    const std::string Add::TYPE("Add");
    const std::string Add::PACKAGE("Math");
    const core::Version Add::VERSION(1, 0);
    
    const std::vector<const core::Description*> Add::setupInputs()
    {
        std::vector<const core::Description*> inputs;
        
        core::Description* input = new core::Description(INPUT, core::DataVariant::UINT_32);
        input->setName("Input");
        inputs.push_back(input);
        
        return inputs;
    }
    
    const std::vector<const core::Description*> Add::setupOutputs()
    {
        std::vector<const core::Description*> outputs;
        
        core::Description* output = new core::Description(OUTPUT, core::DataVariant::UINT_32);
        output->setName("Output");
        outputs.push_back(output);
        
        return outputs;
    }
    
    const std::vector<const core::Parameter*> Add::setupParameters()
    {
        std::vector<const core::Parameter*> parameters;
        
        core::Parameter* offset = new core::Parameter(OFFSET, core::DataVariant::UINT_32);
        offset->setName("Offset");
        offset->setAccessMode(core::Parameter::ACTIVATED_WRITE);
        parameters.push_back(offset);
                                    
        return parameters;
    }
    
    Add::Add()
      : OperatorKernel(TYPE, PACKAGE, VERSION, setupInputs(), setupOutputs(), setupParameters())
    {
    }
    
    void Add::setParameter(unsigned int id, const core::Data& value)
    {
        try
        {
            switch(id)
            {
            case OFFSET:
                m_offset = core::data_cast<const core::UInt32&>(value);
                break;
            default:
                throw core::WrongParameterId(id, *this);
            }
        }
        catch(core::BadCast&)
        {
            throw core::WrongParameterId(id, *this);
        }
    }

    const core::Data& Add::getParameter(const unsigned int id) const
    {
        switch(id)
        {
        case OFFSET:
            return m_offset;
        default:
            throw core::WrongParameterId(id, *this);
        }
    }  
    
    void Add::execute(core::DataProvider& provider)
    {
        core::Id2DataPair inputMapper(INPUT);
        provider.receiveInputData(inputMapper);
        
        core::ReadAccess<core::UInt32> input(inputMapper.data());
        
        core::Data* result = new core::UInt32((unsigned int)(input()) + (unsigned int)(m_offset));
        
        core::DataContainer outContainer(result);
       
        core::Id2DataPair output(OUTPUT, outContainer);
        provider.sendOutputData(output);
    }
} 
