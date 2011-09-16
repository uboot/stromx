#include "Camera.h"

#include "Config.h"

#include <strom/OperatorException.h>
#include <strom/DataContainer.h>
#include <strom/DataProvider.h>
#include <strom/Id2DataPair.h>
#include <strom/Id2DataComposite.h>
#include <strom/Stream.h>
#include <strom/Thread.h>
#include <strom/EnumParameter.h>
#include <strom/NumericParameter.h>
#include <strom/OperatorException.h>
#include <strom/Trigger.h>
#include <strom/Operator.h>

#include "ConstImage.h"
#include "AdjustRgbChannels.h"
#include "PeriodicDelay.h"
#include "Trigger.h"
#include "Clip.h"
#include "ConvertPixelType.h"
#include "impl/CameraBuffer.h"
#include "Queue.h"

using namespace strom;

namespace base
{
    const std::string Camera::TYPE("Camera");
    
    const std::string Camera::PACKAGE(PACKAGE_NAME);
    const Version Camera::VERSION(BASE_VERSION_MAJOR, BASE_VERSION_MINOR);
    
    Camera::Camera()
      : OperatorKernel(TYPE, PACKAGE, VERSION, setupInputs(), setupOutputs(), setupParameters()),
        m_stream(0),
        m_input(0),
        m_clip(0),
        m_adjustRgbChannels(0),
        m_period(0),
        m_trigger(0),
        m_buffer(0),
        m_pixelType(0),
        m_imageQueue(0),
        m_indexQueue(0),
        m_imageWidth(0),
        m_imageHeight(0),
        m_exposure(BASE_EXPOSURE),
        m_wbRed(1.0),
        m_wbGreen(1.0),
        m_wbBlue(1.0)
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
        
        m_stream->connect(m_input, ConstImage::OUTPUT, m_adjustRgbChannels, AdjustRgbChannels::INPUT);
        m_stream->connect(m_adjustRgbChannels, AdjustRgbChannels::OUTPUT, m_clip, Clip::INPUT);
        m_stream->connect(m_clip, Clip::OUTPUT, m_trigger, Trigger::INPUT);
        m_stream->connect(m_trigger, Trigger::OUTPUT, m_period, PeriodicDelay::INPUT);
        m_stream->connect(m_period, PeriodicDelay::OUTPUT, m_buffer, impl::CameraBuffer::INPUT);
        m_stream->connect(m_buffer, impl::CameraBuffer::OUTPUT, m_pixelType, ConvertPixelType::SOURCE);
        m_stream->connect(m_buffer, impl::CameraBuffer::BUFFER, m_pixelType, ConvertPixelType::DESTINATION);
        m_stream->connect(m_pixelType, ConvertPixelType::OUTPUT, m_imageQueue, Queue::INPUT);
        m_stream->connect(m_buffer, impl::CameraBuffer::INDEX, m_indexQueue, Queue::INPUT);
        
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
                m_trigger->setParameter(Trigger::TRIGGER, strom::Trigger());
                break;
            case IMAGE:
                m_input->setParameter(ConstImage::IMAGE, value);
                
                try
                {
                    const strom::Image& image = dynamic_cast<const strom::Image &>(value);
                    
                    m_imageWidth = image.width();
                    m_imageHeight = image.height();
                    
                    m_clip->setParameter(Clip::LEFT, UInt32(0));
                    m_clip->setParameter(Clip::TOP, UInt32(0));
                    m_clip->setParameter(Clip::WIDTH, UInt32(m_imageWidth));
                    m_clip->setParameter(Clip::HEIGHT, UInt32(m_imageHeight));
                    
                    m_left->setMax(UInt32(0));
                    m_top->setMax(UInt32(0));
                    m_width->setMax(UInt32(m_imageWidth));
                    m_height->setMax(UInt32(m_imageHeight));
                }
                catch(std::bad_cast&)
                {
                    throw WrongParameterType(parameter(TRIGGER_MODE), *this);
                }
                
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
                    throw WrongParameterType(parameter(TRIGGER_MODE), *this);
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
                    throw WrongParameterValue(parameter(TRIGGER_MODE), *this);
                }
                break;
            case FRAME_PERIOD:
                m_period->setParameter(PeriodicDelay::PERIOD, value);
                break;
            case BUFFER_SIZE:
                m_buffer->setParameter(impl::CameraBuffer::BUFFER_SIZE, value);
                break;
            case PIXEL_TYPE:
                m_pixelType->setParameter(ConvertPixelType::PIXEL_TYPE, value);
                break;
            case LEFT:
                m_clip->setParameter(Clip::LEFT, value);
                try
                {
                    UInt32 intValue = dynamic_cast<const UInt32 &>(value);
                    m_width->setMax(UInt32(m_imageWidth - intValue));
                }
                catch(std::bad_cast&)
                {
                    throw WrongParameterType(parameter(LEFT), *this);
                }
                break;
            case TOP:
                m_clip->setParameter(Clip::TOP, value);
                try
                {
                    UInt32 intValue = dynamic_cast<const UInt32 &>(value);
                    m_height->setMax(UInt32(m_imageHeight - intValue));
                }
                catch(std::bad_cast&)
                {
                    throw WrongParameterType(parameter(TOP), *this);
                }
                break;
            case WIDTH:
                m_clip->setParameter(Clip::WIDTH, value);
                try
                {
                    UInt32 intValue = dynamic_cast<const UInt32 &>(value);
                    m_left->setMax(UInt32(m_imageWidth - intValue));
                }
                catch(std::bad_cast&)
                {
                    throw WrongParameterType(parameter(WIDTH), *this);
                }
                break;
            case HEIGHT:
                m_clip->setParameter(Clip::HEIGHT, value);
                try
                {
                    UInt32 intValue = dynamic_cast<const UInt32 &>(value);
                    m_top->setMax(UInt32(m_imageHeight - intValue));
                }
                catch(std::bad_cast&)
                {
                    throw WrongParameterType(parameter(HEIGHT), *this);
                }
                break;
            case EXPOSURE:
                try
                {
                    UInt32 intValue = dynamic_cast<const UInt32 &>(value);
                    m_exposure = intValue;
                    setRgbParameters();
                }
                catch(std::bad_cast&)
                {
                    throw WrongParameterType(parameter(EXPOSURE), *this);
                }
                break;
            case WHITE_BALANCE_RED:
                try
                {
                    Double fpValue = dynamic_cast<const Double &>(value);
                    m_wbRed = std::max(0.0, std::min(double(WHITE_BALANCE_MAX), double(fpValue)));
                    setRgbParameters();
                }
                catch(std::bad_cast&)
                {
                    throw WrongParameterType(parameter(EXPOSURE), *this);
                }
                break;
            case WHITE_BALANCE_GREEN:
                try
                {
                    Double fpValue = dynamic_cast<const Double &>(value);
                    m_wbGreen = std::max(0.0, std::min(double(WHITE_BALANCE_MAX), double(fpValue)));
                    setRgbParameters();
                }
                catch(std::bad_cast&)
                {
                    throw WrongParameterType(parameter(EXPOSURE), *this);
                }
                break;
            case WHITE_BALANCE_BLUE:
                try
                {
                    Double fpValue = dynamic_cast<const Double &>(value);
                    m_wbBlue = std::max(0.0, std::min(double(WHITE_BALANCE_MAX), double(fpValue)));
                    setRgbParameters();
                }
                catch(std::bad_cast&)
                {
                    throw WrongParameterType(parameter(EXPOSURE), *this);
                }
                break;
            default:
                throw WrongParameterId(id, *this);
            }
        }
        catch(std::bad_cast&)
        {
            throw WrongParameterType(*parameters()[id], *this);
        }
    }

    const Data& Camera::getParameter(unsigned int id)
    {
        switch(id)
        {
        case TRIGGER:
            return strom::Trigger();
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
        case WIDTH:
            return m_clip->getParameter(Clip::WIDTH);
        case HEIGHT:
            return m_clip->getParameter(Clip::HEIGHT);
        case TOP:
            return m_clip->getParameter(Clip::TOP);
        case LEFT:
            return m_clip->getParameter(Clip::LEFT);
        case EXPOSURE:
            return UInt32(m_exposure);
        case WHITE_BALANCE_RED:
            return Double(m_wbRed);
        case WHITE_BALANCE_GREEN:
            return Double(m_wbGreen);
        case WHITE_BALANCE_BLUE:
            return Double(m_wbBlue);
        default:
            throw WrongParameterId(id, *this);
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
    
    const std::vector<const strom::Description*> Camera::setupInputs()
    {
        std::vector<const Description*> inputs;
        
        return inputs;
    }
    
    const std::vector<const Description*> Camera::setupOutputs()
    {
        std::vector<const Description*> outputs;
        
        Description* output = new Description(OUTPUT, DataVariant::IMAGE);
        output->setName("Output");
        outputs.push_back(output);
        
        Description* index = new Description(INDEX, DataVariant::UINT_32);
        index->setName("Index");
        outputs.push_back(index);
        
        return outputs;
    }
    
    const std::vector<const Parameter*> Camera::setupParameters()
    {
        std::vector<const strom::Parameter*> parameters;
        
        Parameter* image = new Parameter(IMAGE, DataVariant::IMAGE);
        image->setAccessMode(strom::Parameter::ACTIVATED_WRITE);
        parameters.push_back(image);
        
        EnumParameter* triggerMode = new EnumParameter(TRIGGER_MODE);
        triggerMode->setName("Trigger mode");
        triggerMode->setAccessMode(strom::Parameter::ACTIVATED_WRITE);
        triggerMode->add(EnumDescription(Enum(SOFTWARE), "Software trigger"));
        triggerMode->add(EnumDescription(Enum(INTERNAL), "Internal"));
        triggerMode->add(EnumDescription(Enum(EXTERNAL), "External"));
        parameters.push_back(triggerMode);
        
        Parameter* trigger = new Parameter(TRIGGER, DataVariant::TRIGGER);
        trigger->setName("Trigger");
        trigger->setAccessMode(strom::Parameter::ACTIVATED_WRITE);
        parameters.push_back(trigger);
        
        NumericParameter<UInt32>* exposure = new NumericParameter<UInt32>(EXPOSURE, DataVariant::UINT_32);
        exposure->setName("Exposure (milliseconds)");
        exposure->setAccessMode(strom::Parameter::ACTIVATED_WRITE);
        parameters.push_back(exposure);
        
        NumericParameter<Double>* wbRed = new NumericParameter<Double>(WHITE_BALANCE_RED, DataVariant::DOUBLE);
        wbRed->setName("White balance red");
        wbRed->setAccessMode(strom::Parameter::ACTIVATED_WRITE);
        wbRed->setMin(Double(0));
        wbRed->setMax(Double(WHITE_BALANCE_MAX));
        parameters.push_back(wbRed);
        
        NumericParameter<Double>* wbGreen = new NumericParameter<Double>(WHITE_BALANCE_GREEN, DataVariant::DOUBLE);
        wbGreen->setName("White balance green");
        wbGreen->setAccessMode(strom::Parameter::ACTIVATED_WRITE);
        wbGreen->setMin(Double(0));
        wbGreen->setMax(Double(WHITE_BALANCE_MAX));
        parameters.push_back(wbGreen);
        
        NumericParameter<Double>* wbBlue = new NumericParameter<Double>(WHITE_BALANCE_BLUE, DataVariant::DOUBLE);
        wbBlue->setName("White balance blue");
        wbBlue->setAccessMode(strom::Parameter::ACTIVATED_WRITE);
        wbBlue->setMin(Double(0));
        wbBlue->setMax(Double(WHITE_BALANCE_MAX));
        parameters.push_back(wbBlue);
        
        NumericParameter<UInt32>* framePeriod = new NumericParameter<UInt32>(FRAME_PERIOD, DataVariant::UINT_32);
        framePeriod->setName("Frame period (milliseconds)");
        framePeriod->setAccessMode(strom::Parameter::ACTIVATED_WRITE);
        parameters.push_back(framePeriod);
        
        NumericParameter<UInt32>* numBuffers = new NumericParameter<UInt32>(NUM_BUFFERS, DataVariant::UINT_32);
        numBuffers->setName("Number of buffers");
        numBuffers->setAccessMode(strom::Parameter::INITIALIZED_WRITE);
        parameters.push_back(numBuffers);
    
        NumericParameter<UInt32>* bufferSize = new NumericParameter<UInt32>(BUFFER_SIZE, DataVariant::UINT_32);
        bufferSize->setName("Buffer size in bytes");
        bufferSize->setAccessMode(strom::Parameter::INITIALIZED_WRITE);
        parameters.push_back(bufferSize);
    
        m_width = new NumericParameter<UInt32>(WIDTH, DataVariant::UINT_32);
        m_width->setName("ROI width");
        m_width->setAccessMode(strom::Parameter::INITIALIZED_WRITE);
        parameters.push_back(m_width);
    
        m_height = new NumericParameter<UInt32>(HEIGHT, DataVariant::UINT_32);
        m_height->setName("ROI height");
        m_height->setAccessMode(strom::Parameter::INITIALIZED_WRITE);
        parameters.push_back(m_height);
        
        m_top = new NumericParameter<UInt32>(TOP, DataVariant::UINT_32);
        m_top->setName("ROI top offset");
        m_top->setAccessMode(strom::Parameter::INITIALIZED_WRITE);
        parameters.push_back(m_top);
        
        m_left = new NumericParameter<UInt32>(LEFT, DataVariant::UINT_32);
        m_left->setName("ROI left offset");
        m_left->setAccessMode(strom::Parameter::INITIALIZED_WRITE);
        parameters.push_back(m_left);
        
        EnumParameter* pixelType = new EnumParameter(PIXEL_TYPE);
        pixelType->setName("Pixel type");
        pixelType->setAccessMode(strom::Parameter::ACTIVATED_WRITE);
        pixelType->add(EnumDescription(Enum(strom::Image::MONO_8), "Mono image 8-bit"));
        pixelType->add(EnumDescription(Enum(strom::Image::RGB_24), "RGB image 24-bit"));
        pixelType->add(EnumDescription(Enum(strom::Image::BGR_24), "BGR image 24-bit"));
        pixelType->add(EnumDescription(Enum(strom::Image::BAYERBG_8), "Bayer BG pattern 8-bit"));
        pixelType->add(EnumDescription(Enum(strom::Image::BAYERGB_8), "Bayer GB pattern 8-bit"));
        parameters.push_back(pixelType);
                                    
        return parameters;
    }
    
    void Camera::setRgbParameters()
    {
        double exposureCoeff = double(m_exposure) / double(BASE_EXPOSURE);
        
        m_adjustRgbChannels->setParameter(AdjustRgbChannels::RED, Double(exposureCoeff * m_wbRed));
        m_adjustRgbChannels->setParameter(AdjustRgbChannels::GREEN, Double(exposureCoeff * m_wbGreen));
        m_adjustRgbChannels->setParameter(AdjustRgbChannels::BLUE, Double(exposureCoeff * m_wbBlue));
    }
} 
