#include "PeriodicDelay.h"

#include "Config.h"

#include <stromx/core/DataContainer.h>
#include <stromx/core/DataProvider.h>
#include <stromx/core/Id2DataPair.h>
#include <stromx/core/OperatorException.h>

#include <boost/thread/mutex.hpp>

using namespace core;

namespace base
{
    const std::string PeriodicDelay::TYPE("PeriodicDelay");
    
    const std::string PeriodicDelay::PACKAGE(PACKAGE_NAME);
    const Version PeriodicDelay::VERSION(BASE_VERSION_MAJOR, BASE_VERSION_MINOR);
    
    PeriodicDelay::PeriodicDelay()
      : OperatorKernel(TYPE, PACKAGE, VERSION, setupInputs(), setupOutputs(), setupParameters())
    {
    }
    
    void PeriodicDelay::activate()
    {
        m_nextTrigger = boost::get_system_time() + boost::posix_time::millisec(m_period);
    }


    void PeriodicDelay::setParameter(unsigned int id, const Data& value)
    {
        try
        {
            switch(id)
            {
            case PERIOD:
            {
                m_period = dynamic_cast<const UInt32 &>(value);
                break;
            }
            default:
                throw WrongParameterId(id, *this);
            }
        }
        catch(std::bad_cast&)
        {
            throw WrongParameterType(parameter(id), *this);
        }
    }

    const Data& PeriodicDelay::getParameter(const unsigned int id) const
    {
        switch(id)
        {
        case PERIOD:
            return m_period;
        default:
            throw WrongParameterId(id, *this);
        }
    }  
    
    void PeriodicDelay::execute(DataProvider& provider)
    {
        Id2DataPair inputDataMapper(INPUT);
        provider.receiveInputData(inputDataMapper);
        
        if(m_period)
        { 
            try
            {
                boost::this_thread::sleep(m_nextTrigger);
            }
            catch(boost::thread_interrupted&)
            {
                throw Interrupt();
            }
        
            unsigned int passedMs = (boost::get_system_time() - m_nextTrigger).total_milliseconds();
            unsigned int numPeriods = passedMs / m_period + 1;
            m_nextTrigger += boost::posix_time::millisec(m_period * numPeriods);
        }
        
        Id2DataPair outputDataMapper(OUTPUT, inputDataMapper.data());
        provider.sendOutputData( outputDataMapper);
    }
    
    const std::vector<const core::Description*> PeriodicDelay::setupInputs()
    {
        std::vector<const Description*> inputs;
        
        Description* input = new Description(OUTPUT, DataVariant::DATA);
        input->setName("Input");
        inputs.push_back(input);
        
        return inputs;
    }
    
    const std::vector<const Description*> PeriodicDelay::setupOutputs()
    {
        std::vector<const Description*> outputs;
        
        Description* output = new Description(OUTPUT, DataVariant::DATA);
        output->setName("Output");
        outputs.push_back(output);
        
        return outputs;
    }
    
    const std::vector<const Parameter*> PeriodicDelay::setupParameters()
    {
        std::vector<const core::Parameter*> parameters;
        
        Parameter* period = new Parameter(PERIOD, DataVariant::UINT_32);
        period->setName("Period (milliseconds)");
        period->setAccessMode(core::Parameter::ACTIVATED_WRITE);
        parameters.push_back(period);
                                    
        return parameters;
    }
} 
