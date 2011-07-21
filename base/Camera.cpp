#include "Camera.h"

#include "Config.h"

#include <stream/OperatorException.h>
#include <stream/DataContainer.h>
#include <stream/DataProvider.h>
#include <stream/Id2DataPair.h>
#include <stream/Network.h>
#include <stream/OperatorNode.h>
#include <stream/InputNode.h>

#include "ConstImage.h"
#include "AdjustRgbChannels.h"
#include "PeriodicDelay.h"
#include "Trigger.h"
#include "Clip.h"
#include "ConvertPixelType.h"
#include "CameraBuffer.h"
#include "Queue.h"

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
        delete m_network;
    }
    
    void Camera::initialize()
    {
        m_network = new Network();
        
        m_input = m_network->addOperator(new ConstImage);
        m_adjustRgbChannels = m_network->addOperator(new AdjustRgbChannels);
        m_clip = m_network->addOperator(new Clip);
        m_buffer = m_network->addOperator(new camera::CameraBuffer);
        m_period = m_network->addOperator(new PeriodicDelay);
        m_trigger = m_network->addOperator(new Trigger);
        m_pixelType = m_network->addOperator(new ConvertPixelType);
        m_imageQueue = m_network->addOperator(new Queue);
        m_indexQueue = m_network->addOperator(new Queue);
        
        m_adjustRgbChannels->getInputNode(AdjustRgbChannels::INPUT)->connect(m_input->getOutputNode(ConstImage::OUTPUT));
        m_clip->getInputNode(Clip::INPUT)->connect(m_adjustRgbChannels->getOutputNode(AdjustRgbChannels::OUTPUT));
        m_trigger->getInputNode(Trigger::INPUT)->connect(m_clip->getOutputNode(Clip::OUTPUT));
        m_period->getInputNode(PeriodicDelay::INPUT)->connect(m_trigger->getOutputNode(Trigger::OUTPUT));
        m_buffer->getInputNode(camera::CameraBuffer::INPUT)->connect(m_period->getOutputNode(PeriodicDelay::OUTPUT));
        m_pixelType->getInputNode(ConvertPixelType::SOURCE)->connect(m_buffer->getOutputNode(camera::CameraBuffer::OUTPUT));
        m_pixelType->getInputNode(ConvertPixelType::DESTINATION)->connect(m_buffer->getOutputNode(camera::CameraBuffer::BUFFER));
        m_imageQueue->getInputNode(Queue::INPUT)->connect(m_pixelType->getOutputNode(ConvertPixelType::OUTPUT));
        m_indexQueue->getInputNode(Queue::INPUT)->connect(m_buffer->getOutputNode(camera::CameraBuffer::ID));
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
        image->setAccessMode(stream::Parameter::ACTIVATED_WRITE);
        parameters.push_back(image);
                                    
        return parameters;
    }
} 
