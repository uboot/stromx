#include "Camera.h"

#include "Config.h"

#include <stream/OperatorException.h>
#include <stream/DataContainer.h>
#include <stream/DataProvider.h>

#include <stream/Id2DataPair.h>

using namespace stream;

namespace base
{
    const std::string Camera::NAME("Camera");
    
    const std::string Camera::PACKAGE(PACKAGE_NAME);
    const Version Camera::VERSION(BASE_VERSION_MAJOR, BASE_VERSION_MINOR);
    
    Camera::Camera()
      : Operator(NAME, PACKAGE, VERSION, setupInputs(), setupOutputs(), setupParameters())
    {
    }
    
    Camera::~Camera()
    {

    }

    void Camera::setParameter(unsigned int id, const Data& value)
    {
        try
        {
            switch(id)
            {
            default:
                throw ParameterIdException(id, *this);
            }
        }
        catch(std::bad_cast&)
        {
            throw ParameterTypeException(*parameters()[id], *this);
        }
    }

    const Data& Camera::getParameter(unsigned int id)
    {
        switch(id)
        {
        default:
            throw ParameterIdException(id, *this);
        }
    }  
    
    void Camera::execute(DataProvider& provider)
    {
        
    }
    
    void Camera::activate()
    {
    }

    void Camera::deactivate()
    {
    }
    
    const std::vector< Description* > Camera::setupInputs()
    {
        std::vector<Description*> inputs;
        
        return inputs;
    }
    
    const std::vector< stream::Description* > Camera::setupOutputs()
    {
        std::vector<Description*> outputs;
        
        Description* output = new Description(OUTPUT, DataType::IMAGE);
        output->setName("Output");
        outputs.push_back(output);
        
        return outputs;
    }
    
    const std::vector< stream::Parameter* > Camera::setupParameters()
    {
        std::vector<stream::Parameter*> parameters;
        
        Parameter* image = new Parameter(IMAGE, DataType::IMAGE);
        image->setInactiveAccessMode(stream::Parameter::READ_WRITE);
        image->setActiveAccessMode(stream::Parameter::READ_WRITE);
        parameters.push_back(image);
                                    
        return parameters;
    }
} 
