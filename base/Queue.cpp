#include "Queue.h"

#include "Config.h"

#include <stream/NumericParameter.h>
#include <stream/Primitive.h>
#include <stream/OperatorException.h>
#include <stream/DataContainer.h>
#include <stream/DataProvider.h>

#include <stream/Id2DataPair.h>

using namespace stream;

namespace base
{
    const std::string Queue::NAME("Queue");
    
    const std::string Queue::PACKAGE(PACKAGE_NAME);
    const Version Queue::VERSION(BASE_VERSION_MAJOR, BASE_VERSION_MINOR);
    
    Queue::Queue()
      : Operator(NAME, PACKAGE, VERSION, setupInputs(), setupOutputs(), setupParameters())
    {
    }

    void Queue::setParameter(unsigned int id, const Data& value)
    {
        try
        {
            switch(id)
            {
            case SIZE:
                m_size = dynamic_cast<const UInt32&>(value);
                break;
            default:
                throw ParameterIdException(id, *this);
            }
        }
        catch(std::bad_cast&)
        {
            throw ParameterTypeException(*parameters()[id], *this);
        }
    }

    const Data& Queue::getParameter(unsigned int id)
    {
        switch(id)
        {
        case SIZE:
            return m_size;
        default:
            throw ParameterIdException(id, *this);
        }
    }  
    
    void Queue::execute(DataProvider& provider)
    {
        Id2DataPair inputDataMapper(INPUT);
        provider.receiveInputData(inputDataMapper);
        
        
        
        Id2DataPair outputDataMapper(OUTPUT, inputDataMapper.data());
        provider.sendOutputData( outputDataMapper);
    }
    
    const std::vector< Description* > Queue::setupInputs()
    {
        std::vector<Description*> inputs;
        
        Description* input = new Description(INPUT, DataType::DATA);
        input->setName("Input");
        inputs.push_back(input);
        
        return inputs;
    }
    
    const std::vector< stream::Description* > Queue::setupOutputs()
    {
        std::vector<Description*> outputs;
        
        Description* output = new Description(OUTPUT, DataType::DATA);
        output->setName("Output");
        outputs.push_back(output);
        
        return outputs;
    }
    
    const std::vector< stream::Parameter* > Queue::setupParameters()
    {
        std::vector<stream::Parameter*> parameters;
        
        NumericParameter<UInt32>* size = new NumericParameter<UInt32>(SIZE, DataType::UINT_32);
        size->setName("Size");
        size->setInactiveAccessMode(stream::Parameter::READ_WRITE);
        size->setActiveAccessMode(stream::Parameter::READ);
        parameters.push_back(size);
                                    
        return parameters;
    }
} 
