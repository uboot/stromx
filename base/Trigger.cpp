#include "Trigger.h"

#include "Config.h"

#include <stream/DataContainer.h>
#include <stream/DataProvider.h>
#include <stream/Id2DataPair.h>
#include <stream/OperatorException.h>

using namespace stream;

namespace base
{
    const std::string Trigger::NAME("Trigger");
    
    const std::string Trigger::PACKAGE(PACKAGE_NAME);
    const Version Trigger::VERSION(BASE_VERSION_MAJOR, BASE_VERSION_MINOR);
    
    Trigger::Trigger()
      : Operator(NAME, PACKAGE, VERSION, setupInputs(), setupOutputs(), setupParameters()),
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
                throw ParameterIdException(id, *this);
            }
        }
        catch(std::bad_cast&)
        {
            throw ParameterTypeException(*parameters()[id], *this);
        }
    }

    const Data& Trigger::getParameter(unsigned int id)
    {
        switch(id)
        {
        case TRIGGER:
            throw ParameterAccessModeException(*parameters()[id], *this);
        case ACTIVE:
            return m_active;
        default:
            throw ParameterIdException(id, *this);
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
                throw InterruptException();
            }
        }
        
        Id2DataPair outputDataMapper(OUTPUT, inputDataMapper.data());
        provider.sendOutputData( outputDataMapper);
    }
    
    const std::vector< Description* > Trigger::setupInputs()
    {
        std::vector<Description*> inputs;
        
        Description* input = new Description(OUTPUT, DataType::DATA);
        input->setName("Input");
        inputs.push_back(input);
        
        return inputs;
    }
    
    const std::vector< stream::Description* > Trigger::setupOutputs()
    {
        std::vector<Description*> outputs;
        
        Description* output = new Description(OUTPUT, DataType::DATA);
        output->setName("Output");
        outputs.push_back(output);
        
        return outputs;
    }
    
    const std::vector< stream::Parameter* > Trigger::setupParameters()
    {
        std::vector<stream::Parameter*> parameters;
        
        Parameter* trigger = new Parameter(TRIGGER, DataType::TRIGGER);
        trigger->setName("Trigger");
        trigger->setActiveAccessMode(Parameter::WRITE);
        trigger->setInactiveAccessMode(Parameter::WRITE);
        parameters.push_back(trigger);
        
        Parameter* active = new Parameter(ACTIVE, DataType::BOOL);
        active->setName("Active");
        active->setActiveAccessMode(Parameter::WRITE);
        active->setInactiveAccessMode(Parameter::WRITE);
        parameters.push_back(active);
                                    
        return parameters;
    }
} 
