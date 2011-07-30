#include "Camera.h"

#include "Config.h"

#include <stream/OperatorException.h>
#include <stream/DataContainer.h>
#include <stream/DataProvider.h>
#include <stream/Id2DataPair.h>
#include <stream/Id2DataComposite.h>
#include <stream/Network.h>
#include <stream/Stream.h>
#include <stream/OperatorNode.h>
#include <stream/InputNode.h>
#include <stream/EnumParameter.h>
#include <stream/NumericParameter.h>
#include <stream/SynchronizedOperatorKernel.h>
#include <stream/OperatorException.h>
#include <stream/Trigger.h>

#include "ConstImage.h"
#include "AdjustRgbChannels.h"
#include "PeriodicDelay.h"
#include "Trigger.h"
#include "Clip.h"
#include "ConvertPixelType.h"
#include "CameraBuffer.h"
#include "Queue.h"
#include <stream/ThreadImpl.h>

using namespace stream;

namespace base
{
    const std::string Camera::NAME("Camera");
    
    const std::string Camera::PACKAGE(PACKAGE_NAME);
    const Version Camera::VERSION(BASE_VERSION_MAJOR, BASE_VERSION_MINOR);
    
    Camera::Camera()
      : OperatorKernel(NAME, PACKAGE, VERSION, setupInputs(), setupOutputs(), setupParameters())
    {
        // TODO: initialize all members
    }
    
    Camera::~Camera()
    {
        delete m_stream;
    }
    
    void Camera::initialize()
    {
        Network* network = new Network();
        
        m_input = network->addOperator(new ConstImage);
        m_adjustRgbChannels = network->addOperator(new AdjustRgbChannels);
        m_clip = network->addOperator(new Clip);
        m_buffer = network->addOperator(new camera::CameraBuffer);
        m_period = network->addOperator(new PeriodicDelay);
        m_trigger = network->addOperator(new Trigger);
        m_pixelType = network->addOperator(new ConvertPixelType);
        m_imageQueue = network->addOperator(new Queue);
        m_indexQueue = network->addOperator(new Queue);
        
        m_adjustRgbChannels->getInputNode(AdjustRgbChannels::INPUT)->connect(m_input->getOutputNode(ConstImage::OUTPUT));
        m_clip->getInputNode(Clip::INPUT)->connect(m_adjustRgbChannels->getOutputNode(AdjustRgbChannels::OUTPUT));
        m_trigger->getInputNode(Trigger::INPUT)->connect(m_clip->getOutputNode(Clip::OUTPUT));
        m_period->getInputNode(PeriodicDelay::INPUT)->connect(m_trigger->getOutputNode(Trigger::OUTPUT));
        m_buffer->getInputNode(camera::CameraBuffer::INPUT)->connect(m_period->getOutputNode(PeriodicDelay::OUTPUT));
        m_pixelType->getInputNode(ConvertPixelType::SOURCE)->connect(m_buffer->getOutputNode(camera::CameraBuffer::OUTPUT));
        m_pixelType->getInputNode(ConvertPixelType::DESTINATION)->connect(m_buffer->getOutputNode(camera::CameraBuffer::BUFFER));
        m_imageQueue->getInputNode(Queue::INPUT)->connect(m_pixelType->getOutputNode(ConvertPixelType::OUTPUT));
        m_indexQueue->getInputNode(Queue::INPUT)->connect(m_buffer->getOutputNode(camera::CameraBuffer::INDEX));
        
        ThreadImpl* frameThread = new ThreadImpl();
        frameThread->addNode(m_adjustRgbChannels->getInputNode(AdjustRgbChannels::INPUT));
        frameThread->addNode(m_clip->getInputNode(Clip::INPUT));
        frameThread->addNode(m_trigger->getInputNode(Trigger::INPUT));
        frameThread->addNode(m_period->getInputNode(PeriodicDelay::INPUT));
        frameThread->addNode(m_buffer->getInputNode(camera::CameraBuffer::INPUT));
        
        ThreadImpl* mainThread = new ThreadImpl();
        mainThread->addNode(m_pixelType->getInputNode(ConvertPixelType::SOURCE));
        mainThread->addNode(m_pixelType->getInputNode(ConvertPixelType::DESTINATION));
        mainThread->addNode(m_imageQueue->getInputNode(Queue::INPUT));
        mainThread->addNode(m_indexQueue->getInputNode(Queue::INPUT));
        
        m_stream = new Stream(network);
        m_stream->addThread(frameThread);
        m_stream->addThread(mainThread);
        
        // start with software trigger
        m_trigger->op()->setParameter(Trigger::ACTIVE, Bool(true));
        m_period->op()->setParameter(PeriodicDelay::PERIOD, UInt32(0));
    }

    void Camera::setParameter(unsigned int id, const Data& value)
    {
        try
        {
            switch(id)
            {       
            case TRIGGER:
                m_trigger->op()->setParameter(Trigger::TRIGGER, stream::Trigger());
                break;
            case IMAGE:
                m_input->op()->setParameter(ConstImage::IMAGE, value);
                break;
            case NUM_BUFFERS:
                m_buffer->op()->setParameter(camera::CameraBuffer::NUM_BUFFERS, value);
                m_imageQueue->op()->setParameter(Queue::SIZE, value);
                m_indexQueue->op()->setParameter(Queue::SIZE, value);
                break;
            case TRIGGER_MODE:
                int triggerMode;
                try
                {
                    triggerMode = dynamic_cast<const Enum&>(value);
                }
                catch(std::bad_cast&)
                {
                    throw ParameterTypeException(parameter(TRIGGER_MODE), *this);
                }
                
                switch(triggerMode)
                {
                case SOFTWARE:
                    m_trigger->op()->setParameter(Trigger::ACTIVE, Bool(true));
                    break;
                case INTERNAL:
                    m_trigger->op()->setParameter(Trigger::ACTIVE, Bool(false));
                    break;
                default:
                    throw ParameterValueException(parameter(TRIGGER_MODE), *this);
                }
                break;
            case FRAME_PERIOD:
                m_period->op()->setParameter(PeriodicDelay::PERIOD, value);
                break;
            case BUFFER_SIZE:
                m_buffer->op()->setParameter(camera::CameraBuffer::BUFFER_SIZE, value);
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

    const Data& Camera::getParameter(unsigned int id)
    {
        switch(id)
        {
        case TRIGGER:
            return stream::Trigger();
        case TRIGGER_MODE:
        {
            const Data& value = m_trigger->op()->getParameter(Trigger::ACTIVE);
            const Bool& triggerActive = dynamic_cast<const Bool&>(value);
            
            if(triggerActive)
                return Enum(SOFTWARE);
            else
                return Enum(INTERNAL);
        }
        case FRAME_PERIOD:
            return m_period->op()->getParameter(PeriodicDelay::PERIOD);
        case IMAGE:
            return m_input->op()->getParameter(ConstImage::IMAGE);
        case NUM_BUFFERS:
            return m_buffer->op()->getParameter(camera::CameraBuffer::NUM_BUFFERS);
        case BUFFER_SIZE:
            return m_buffer->op()->getParameter(camera::CameraBuffer::BUFFER_SIZE);
        default:
            throw ParameterIdException(id, *this);
        }
    }  
    
    void Camera::execute(DataProvider& provider)
    {
        DataContainer image = m_imageQueue->op()->getOutputData(Queue::OUTPUT);
        DataContainer index = m_indexQueue->op()->getOutputData(Queue::OUTPUT);
        
        m_imageQueue->op()->clearOutputData(Queue::OUTPUT);
        m_indexQueue->op()->clearOutputData(Queue::OUTPUT);
        
        Id2DataPair imageMapper(OUTPUT, image);
        Id2DataPair indexMapper(INDEX, index);
        
        provider.sendOutputData(imageMapper && indexMapper);
    }
    
    void Camera::activate()
    {
        m_stream->start();
    }

    void Camera::deactivate()
    {
        m_stream->stop();
        m_stream->join();
    }
    
    const std::vector<const stream::Description*> Camera::setupInputs()
    {
        std::vector<const Description*> inputs;
        
        return inputs;
    }
    
    const std::vector<const Description*> Camera::setupOutputs()
    {
        std::vector<const Description*> outputs;
        
        Description* output = new Description(OUTPUT, DataType::IMAGE);
        output->setName("Output");
        outputs.push_back(output);
        
        Description* index = new Description(INDEX, DataType::UINT_32);
        index->setName("Index");
        outputs.push_back(index);
        
        return outputs;
    }
    
    const std::vector<const Parameter*> Camera::setupParameters()
    {
        std::vector<const stream::Parameter*> parameters;
        
        Parameter* image = new Parameter(IMAGE, DataType::IMAGE);
        image->setAccessMode(stream::Parameter::ACTIVATED_WRITE);
        parameters.push_back(image);
        
        EnumParameter* triggerMode = new EnumParameter(TRIGGER_MODE);
        triggerMode->setName("Trigger mode");
        triggerMode->setAccessMode(stream::Parameter::ACTIVATED_WRITE);
        triggerMode->add(EnumDescription(Enum(SOFTWARE), "Software trigger"));
        triggerMode->add(EnumDescription(Enum(INTERNAL), "Internal"));
        triggerMode->add(EnumDescription(Enum(EXTERNAL), "External"));
        parameters.push_back(triggerMode);
        
        Parameter* trigger = new Parameter(TRIGGER, DataType::TRIGGER);
        trigger->setName("Trigger");
        trigger->setAccessMode(stream::Parameter::ACTIVATED_WRITE);
        parameters.push_back(trigger);
        
        NumericParameter<UInt32>* framePeriod = new NumericParameter<UInt32>(FRAME_PERIOD, DataType::UINT_32);
        framePeriod->setName("Frame period (milliseconds)");
        framePeriod->setAccessMode(stream::Parameter::ACTIVATED_WRITE);
        parameters.push_back(framePeriod);
        
        NumericParameter<UInt32>* numBuffers = new NumericParameter<UInt32>(NUM_BUFFERS, DataType::UINT_32);
        numBuffers->setName("Number of buffers");
        numBuffers->setAccessMode(stream::Parameter::INITIALIZED_WRITE);
        parameters.push_back(numBuffers);
    
        NumericParameter<UInt32>* bufferSize = new NumericParameter<UInt32>(BUFFER_SIZE, DataType::UINT_32);
        bufferSize->setName("Buffer size in bytes");
        bufferSize->setAccessMode(stream::Parameter::INITIALIZED_WRITE);
        parameters.push_back(bufferSize);
                                    
        return parameters;
    }
} 
