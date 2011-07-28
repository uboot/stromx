#include "Queue.h"

#include "Config.h"

#include <stream/NumericParameter.h>
#include <stream/Primitive.h>
#include <stream/OperatorException.h>
#include <stream/DataContainer.h>
#include <stream/DataProvider.h>
#include <stream/Id2DataPair.h>
#include <stream/Try.h>

using namespace stream;

namespace base
{
    const std::string Queue::NAME("Queue");
    
    const std::string Queue::PACKAGE(PACKAGE_NAME);
    const Version Queue::VERSION(BASE_VERSION_MAJOR, BASE_VERSION_MINOR);
    
    Queue::Queue()
      : Operator(NAME, PACKAGE, VERSION, setupInputs(), setupOutputs(), setupParameters()),
        m_size(1)
    {
    }

    void Queue::setParameter(unsigned int id, const Data& value)
    {
        try
        {
            switch(id)
            {
            case SIZE:
                if(m_size == 0)
                    throw ParameterValueException(parameter(SIZE), *this);
                
                m_size = dynamic_cast<const UInt32&>(value);
                break;
            default:
                throw ParameterIdException(id, *this);
            }
        }
        catch(std::bad_cast&)
        {
            throw ParameterValueException(parameter(id), *this);
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
    
    void Queue::deactivate()
    {
        m_deque.clear();
    }
    
    void Queue::execute(DataProvider& provider)
    {
        // if the queue is not full
        if(m_deque.size() < m_size)
        {
            // try to get data from the input
            Id2DataPair inputDataMapper(INPUT);
            provider.receiveInputData(Try(inputDataMapper));
            
            // if there was data push it on the queue
            if(! inputDataMapper.data().empty())
                m_deque.push_back(inputDataMapper.data());
        }
            
        // if the queue is not empty
        if(m_deque.size() > 0)
        {
            // try to push data to the output
            Id2DataPair outputDataMapper(OUTPUT, m_deque.front());
            provider.sendOutputData(Try(outputDataMapper));
            
            // if this was successful delete the data from the queue
            if(outputDataMapper.data().empty())
                m_deque.pop_front();
        }
    }
    
    const std::vector<const stream::Description*> Queue::setupInputs()
    {
        std::vector<const Description*> inputs;
        
        Description* input = new Description(INPUT, DataType::DATA);
        input->setName("Input");
        inputs.push_back(input);
        
        return inputs;
    }
    
    const std::vector<const Description*> Queue::setupOutputs()
    {
        std::vector<const Description*> outputs;
        
        Description* output = new Description(OUTPUT, DataType::DATA);
        output->setName("Output");
        outputs.push_back(output);
        
        return outputs;
    }
    
    const std::vector<const Parameter*> Queue::setupParameters()
    {
        std::vector<const stream::Parameter*> parameters;
        
        NumericParameter<UInt32>* size = new NumericParameter<UInt32>(SIZE, DataType::UINT_32);
        size->setName("Size");
        size->setMin(UInt32(1));
        size->setAccessMode(stream::Parameter::INITIALIZED_WRITE);
        parameters.push_back(size);
                                    
        return parameters;
    }
} 
