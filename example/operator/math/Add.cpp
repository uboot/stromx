#include "Add.h"

#include <stromx/runtime/Primitive.h>
#include <stromx/runtime/OperatorException.h>
#include <stromx/runtime/DataProvider.h>
#include <stromx/runtime/Id2DataPair.h>
#include <stromx/runtime/Id2DataComposite.h>
#include <stromx/runtime/ReadAccess.h>

using namespace stromx;

namespace math
{
    const std::string Add::TYPE("Add");
    const std::string Add::PACKAGE("Math");
    const runtime::Version Add::VERSION(1, 0, 0);
    
    const std::vector<const runtime::Description*> Add::setupInputs()
    {
        std::vector<const runtime::Description*> inputs;
        
        runtime::Description* input = new runtime::Description(INPUT, runtime::Variant::UINT_32);
        input->setTitle("Input");
        inputs.push_back(input);
        
        return inputs;
    }
    
    const std::vector<const runtime::Description*> Add::setupOutputs()
    {
        std::vector<const runtime::Description*> outputs;
        
        runtime::Description* output = new runtime::Description(OUTPUT, runtime::Variant::UINT_32);
        output->setTitle("Output");
        outputs.push_back(output);
        
        return outputs;
    }
    
    const std::vector<const runtime::Parameter*> Add::setupParameters()
    {
        std::vector<const runtime::Parameter*> parameters;
        
        runtime::Parameter* offset = new runtime::Parameter(OFFSET, runtime::Variant::UINT_32);
        offset->setTitle("Offset");
        offset->setAccessMode(runtime::Parameter::ACTIVATED_WRITE);
        parameters.push_back(offset);
                                    
        return parameters;
    }
    
    Add::Add()
      : OperatorKernel(TYPE, PACKAGE, VERSION, setupInputs(), setupOutputs(), setupParameters())
    {
    }
    
    void Add::setParameter(unsigned int id, const runtime::Data& value)
    {
        try
        {
            switch(id)
            {
            case OFFSET:
                m_offset = runtime::data_cast<runtime::UInt32>(value);
                break;
            default:
                throw runtime::WrongParameterId(id, *this);
            }
        }
        catch(runtime::BadCast&)
        {
            throw runtime::WrongParameterType(parameter(id), *this);
        }
    }

    const runtime::DataRef Add::getParameter(const unsigned int id) const
    {
        switch(id)
        {
        case OFFSET:
            return m_offset;
        default:
            throw runtime::WrongParameterId(id, *this);
        }
    }  
    
    void Add::execute(runtime::DataProvider& provider)
    {
        runtime::Id2DataPair inputMapper(INPUT);
        provider.receiveInputData(inputMapper);
        
        runtime::ReadAccess<runtime::UInt32> input(inputMapper.data());
        
        runtime::Data* result = new runtime::UInt32((unsigned int)(input()) + (unsigned int)(m_offset));
        
        runtime::DataContainer outContainer(result);
       
        runtime::Id2DataPair output(OUTPUT, outContainer);
        provider.sendOutputData(output);
    }
} 
