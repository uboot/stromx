#include "Trigger.h"

#include "Config.h"

#include <stream/DataContainer.h>
#include <stream/DataProvider.h>
#include <stream/Id2DataPair.h>
#include <stream/OperatorException.h>

using namespace stream;

namespace base
{
    const std::string Trigger::TYPE("Trigger");
    
    const std::string Trigger::PACKAGE(PACKAGE_NAME);
    const Version Trigger::VERSION(BASE_VERSION_MAJOR, BASE_VERSION_MINOR);
    
    Trigger::Trigger()
      : OperatorKernel(TYPE, PACKAGE, VERSION, setupInputs(), setupOutputs(), setupParameters()),
        m_active(true)
    {
    }

    void Trigger::setParameter(unsigned int id, const Data& value)
    {
        try
        {
            switch(id)
            {
            case TRIGGER:
            {
                // trigger
                m_cond.notify_all();
                break;
            }
            case ACTIVE:
                m_active = dynamic_cast<const Bool&>(value);
                break;
            default:
                throw WrongParameterId(id, *this);
            }
        }
        catch(std::bad_cast&)
        {
            throw WrongParameterType(parameter(id), *this);
        }
    }

    const Data& Trigger::getParameter(unsigned int id)
    {
        switch(id)
        {
        case TRIGGER:
            throw ParameterAccessViolation(parameter(id), *this);
        case ACTIVE:
            return m_active;
        default:
            throw WrongParameterId(id, *this);
        }
    }  
    
    void Trigger::execute(DataProvider& provider)
    {
        Id2DataPair inputDataMapper(INPUT);
        provider.receiveInputData(inputDataMapper);
        
        if(m_active)
        {
            try
            {
                // wait for trigger
                unique_lock_t lock(m_mutex);
                m_cond.wait(lock);
            }
            catch(boost::thread_interrupted&)
            {
                throw Interrupt();
            }
        }
        
        Id2DataPair outputDataMapper(OUTPUT, inputDataMapper.data());
        provider.sendOutputData( outputDataMapper);
    }
    
    const std::vector<const stream::Description*> Trigger::setupInputs()
    {
        std::vector<const Description*> inputs;
        
        Description* input = new Description(OUTPUT, DataVariant::DATA);
        input->setName("Input");
        inputs.push_back(input);
        
        return inputs;
    }
    
    const std::vector<const Description*> Trigger::setupOutputs()
    {
        std::vector<const Description*> outputs;
        
        Description* output = new Description(OUTPUT, DataVariant::DATA);
        output->setName("Output");
        outputs.push_back(output);
        
        return outputs;
    }
    
    const std::vector<const Parameter*> Trigger::setupParameters()
    {
        std::vector<const stream::Parameter*> parameters;
        
        Parameter* trigger = new Parameter(TRIGGER, DataVariant::TRIGGER);
        trigger->setName("Trigger");
        trigger->setAccessMode(stream::Parameter::ACTIVATED_WRITE);
        parameters.push_back(trigger);
        
        Parameter* active = new Parameter(ACTIVE, DataVariant::BOOL);
        active->setName("Active");
        active->setAccessMode(stream::Parameter::ACTIVATED_WRITE);
        parameters.push_back(active);
                                    
        return parameters;
    }
} 
