#include "Camera.h"

#include "Config.h"

#include <stream/OperatorException.h>
#include <stream/DataContainer.h>
#include <stream/DataProvider.h>
#include <stream/Id2DataPair.h>
#include <stream/Id2DataComposite.h>
#include <stream/Stream.h>
#include <stream/Thread.h>
#include <stream/EnumParameter.h>
#include <stream/NumericParameter.h>
#include <stream/OperatorException.h>
#include <stream/Trigger.h>
#include <stream/Operator.h>

#include "ConstImage.h"
#include "AdjustRgbChannels.h"
#include "PeriodicDelay.h"
#include "Trigger.h"
#include "Clip.h"
#include "ConvertPixelType.h"
#include "impl/CameraBuffer.h"
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
        m_stream = new Stream();
        
        m_input = new Operator(new ConstImage);
        m_adjustRgbChannels = new Operator(new AdjustRgbChannels);
        m_clip = new Operator(new Clip);
        m_buffer = new Operator(new impl::CameraBuffer);
        m_period = new Operator(new PeriodicDelay);
        m_trigger = new Operator(new Trigger);
        m_pixelType = new Operator(new ConvertPixelType);
        m_imageQueue = new Operator(new Queue);
        m_indexQueue = new Operator(new Queue);
        
        m_input->initialize();
        m_adjustRgbChannels->initialize();
        m_clip->initialize();
        m_buffer->initialize();
        m_period->initialize();
        m_trigger->initialize();
        m_pixelType->initialize();
        m_imageQueue->initialize();
        m_indexQueue->initialize();
        
        m_stream->addOperator(m_input);
        m_stream->addOperator(m_adjustRgbChannels);
        m_stream->addOperator(m_clip);
        m_stream->addOperator(m_buffer);
        m_stream->addOperator(m_period);
        m_stream->addOperator(m_trigger);
        m_stream->addOperator(m_pixelType);
        m_stream->addOperator(m_imageQueue);
        m_stream->addOperator(m_indexQueue);
        
        m_stream->connect(m_adjustRgbChannels, AdjustRgbChannels::INPUT, m_input, ConstImage::OUTPUT);
        m_stream->connect(m_clip, Clip::INPUT, m_adjustRgbChannels, AdjustRgbChannels::OUTPUT);
        m_stream->connect(m_trigger, Trigger::INPUT, m_clip, Clip::OUTPUT);
        m_stream->connect(m_period, PeriodicDelay::INPUT, m_trigger, Trigger::OUTPUT);
        m_stream->connect(m_buffer, impl::CameraBuffer::INPUT, m_period, PeriodicDelay::OUTPUT);
        m_stream->connect(m_pixelType, ConvertPixelType::SOURCE, m_buffer, impl::CameraBuffer::OUTPUT);
        m_stream->connect(m_pixelType, ConvertPixelType::DESTINATION, m_buffer, impl::CameraBuffer::BUFFER);
        m_stream->connect(m_imageQueue, Queue::INPUT, m_pixelType, ConvertPixelType::OUTPUT);
        m_stream->connect(m_imageQueue, Queue::INPUT, m_pixelType, ConvertPixelType::OUTPUT);
        m_stream->connect(m_indexQueue, Queue::INPUT, m_buffer, impl::CameraBuffer::INDEX);
        
        Thread* frameThread = m_stream->addThread();
        frameThread->addNode(m_adjustRgbChannels, AdjustRgbChannels::INPUT);
        frameThread->addNode(m_clip, Clip::INPUT);
        frameThread->addNode(m_trigger, Trigger::INPUT);
        frameThread->addNode(m_period, PeriodicDelay::INPUT);
        frameThread->addNode(m_buffer, impl::CameraBuffer::INPUT);
        
        Thread* mainThread = m_stream->addThread();
        mainThread->addNode(m_pixelType, ConvertPixelType::SOURCE);
        mainThread->addNode(m_pixelType, ConvertPixelType::DESTINATION);
        mainThread->addNode(m_imageQueue, Queue::INPUT);
        mainThread->addNode(m_indexQueue, Queue::INPUT);
        
        // start with software trigger
        m_trigger->setParameter(Trigger::ACTIVE, Bool(true));
        m_period->setParameter(PeriodicDelay::PERIOD, UInt32(0));
    }

    void Camera::setParameter(unsigned int id, const Data& value)
    {
        try
        {
            switch(id)
            {       
            case TRIGGER:
                m_trigger->setParameter(Trigger::TRIGGER, stream::Trigger());
                break;
            case IMAGE:
                m_input->setParameter(ConstImage::IMAGE, value);
                break;
            case NUM_BUFFERS:
                m_buffer->setParameter(impl::CameraBuffer::NUM_BUFFERS, value);
                m_imageQueue->setParameter(Queue::SIZE, value);
                m_indexQueue->setParameter(Queue::SIZE, value);
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
                    m_trigger->setParameter(Trigger::ACTIVE, Bool(true));
                    break;
                case INTERNAL:
                    m_trigger->setParameter(Trigger::ACTIVE, Bool(false));
                    break;
                default:
                    throw ParameterValueException(parameter(TRIGGER_MODE), *this);
                }
                break;
            case FRAME_PERIOD:
                m_period->setParameter(PeriodicDelay::PERIOD, value);
                break;
            case BUFFER_SIZE:
                m_buffer->setParameter(impl::CameraBuffer::BUFFER_SIZE, value);
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
            const Data& value = m_trigger->getParameter(Trigger::ACTIVE);
            const Bool& triggerActive = dynamic_cast<const Bool&>(value);
            
            if(triggerActive)
                return Enum(SOFTWARE);
            else
                return Enum(INTERNAL);
        }
        case FRAME_PERIOD:
            return m_period->getParameter(PeriodicDelay::PERIOD);
        case IMAGE:
            return m_input->getParameter(ConstImage::IMAGE);
        case NUM_BUFFERS:
            return m_buffer->getParameter(impl::CameraBuffer::NUM_BUFFERS);
        case BUFFER_SIZE:
            return m_buffer->getParameter(impl::CameraBuffer::BUFFER_SIZE);
        default:
            throw ParameterIdException(id, *this);
        }
    }  
    
    void Camera::execute(DataProvider& provider)
    {
        DataContainer image = m_imageQueue->getOutputData(Queue::OUTPUT);
        DataContainer index = m_indexQueue->getOutputData(Queue::OUTPUT);
        
        m_imageQueue->clearOutputData(Queue::OUTPUT);
        m_indexQueue->clearOutputData(Queue::OUTPUT);
        
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
