/* 
*  Copyright 2011 Matthias Fuchs
*
*  Licensed under the Apache License, Version 2.0 (the "License");
*  you may not use this file except in compliance with the License.
*  You may obtain a copy of the License at
*
*      http://www.apache.org/licenses/LICENSE-2.0
*
*  Unless required by applicable law or agreed to in writing, software
*  distributed under the License is distributed on an "AS IS" BASIS,
*  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
*  See the License for the specific language governing permissions and
*  limitations under the License.
*/

#include "stromx/example/AdjustRgbChannels.h"
#include "stromx/example/Camera.h"
#include "stromx/example/Clip.h"
#include "stromx/example/ConstImage.h"
#include "stromx/example/ConvertPixelType.h"
#include "stromx/example/Image.h"
#include "stromx/example/impl/CameraBuffer.h"
#include <stromx/runtime/DataContainer.h>
#include <stromx/runtime/DataProvider.h>
#include <stromx/runtime/EnumParameter.h>
#include <stromx/runtime/Id2DataComposite.h>
#include <stromx/runtime/Id2DataPair.h>
#include <stromx/runtime/NumericParameter.h>
#include <stromx/runtime/Operator.h>
#include <stromx/runtime/OperatorException.h>
#include <stromx/runtime/ParameterGroup.h>
#include <stromx/runtime/PeriodicDelay.h>
#include <stromx/runtime/Queue.h>
#include <stromx/runtime/Stream.h>
#include <stromx/runtime/Thread.h>
#include <stromx/runtime/Trigger.h>

namespace stromx
{
    using namespace runtime;

    namespace example
    {
        const std::string Camera::TYPE("Camera");
        
        const std::string Camera::PACKAGE(STROMX_EXAMPLE_PACKAGE_NAME);
        const Version Camera::VERSION(STROMX_EXAMPLE_VERSION_MAJOR, STROMX_EXAMPLE_VERSION_MINOR, STROMX_EXAMPLE_VERSION_PATCH);
        
        Camera::Camera()
          : OperatorKernel(TYPE, PACKAGE, VERSION, setupInitParameters()),
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
            m_outputIndex(false),
            m_left(0),
            m_top(0),
            m_width(0),
            m_height(0),
            m_imageWidth(0),
            m_imageHeight(0),
            m_imageDepth(0),
            m_exposure(BASE_EXPOSURE),
            m_wbRed(1.0),
            m_wbGreen(1.0),
            m_wbBlue(1.0),
            m_isFirstInitialization(true)
        {
            m_input = new Operator(new ConstImage);
            m_adjustRgbChannels = new Operator(new AdjustRgbChannels);
            m_clip = new Operator(new Clip);
            m_buffer = new Operator(new impl::CameraBuffer);
            m_period = new Operator(new PeriodicDelay);
            m_trigger = new Operator(new Trigger);
            m_pixelType = new Operator(new ConvertPixelType);
            m_imageQueue = new Operator(new Queue);
            m_indexQueue = new Operator(new Queue);
            
            m_stream = new Stream();
        }
        
        Camera::~Camera()
        {
            delete m_stream;
            
            delete m_indexQueue;
            delete m_imageQueue;
            delete m_pixelType;
            delete m_trigger;
            delete m_period;
            delete m_buffer;
            delete m_clip;
            delete m_adjustRgbChannels;
            delete m_input;
        }
        
        void Camera::deinitialize()
        {
            // remove all operators from the stream and deinitialize them
            std::vector<Operator*> ops = m_stream->operators();
            for(std::vector<Operator*>::const_iterator iter = ops.begin();
                iter != ops.end();
                ++iter)
            {
                m_stream->removeOperator(*iter);
                (*iter)->deinitialize();
            }
            
            OperatorKernel::deinitialize();
        }
        
        void Camera::initialize()
        {
            OperatorKernel::initialize(setupInputs(), setupOutputs(), setupParameters());
            
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
            frameThread->addInput(m_adjustRgbChannels, AdjustRgbChannels::INPUT);
            frameThread->addInput(m_clip, Clip::INPUT);
            frameThread->addInput(m_trigger, Trigger::INPUT);
            frameThread->addInput(m_period, PeriodicDelay::INPUT);
            frameThread->addInput(m_buffer, impl::CameraBuffer::INPUT);
            
            Thread* mainThread = m_stream->addThread();
            mainThread->addInput(m_pixelType, ConvertPixelType::SOURCE);
            mainThread->addInput(m_pixelType, ConvertPixelType::DESTINATION);
            mainThread->addInput(m_imageQueue, Queue::INPUT);
            mainThread->addInput(m_indexQueue, Queue::INPUT);
            
            // if this is the first time the operator is initialized, set its parameters
            // to reasonable values
            if(m_isFirstInitialization)
            {
                m_period->setParameter(PeriodicDelay::PERIOD, UInt32(1000));
                m_buffer->setParameter(impl::CameraBuffer::NUM_BUFFERS, UInt32(2));
                m_buffer->setParameter(impl::CameraBuffer::BUFFER_SIZE, UInt32(1000000));
                m_imageQueue->setParameter(Queue::SIZE, UInt32(2));
                m_indexQueue->setParameter(Queue::SIZE, UInt32(2));
                m_isFirstInitialization = false;
            }
        }

        void Camera::setParameter(unsigned int id, const Data& value)
        {
            try
            {
                switch(id)
                {       
                case OUTPUT_INDEX:
                    m_outputIndex = data_cast<stromx::Bool>(value);
                    break;
                case TRIGGER:
                    m_trigger->setParameter(Trigger::TRIGGER, runtime::TriggerData());
                    break;
                case IMAGE:
                {
                    const stromx::Image & image = data_cast<stromx::Image>(value);
                    DataRef size = getParameter(BUFFER_SIZE);
                    DataRef pixelType = getParameter(PIXEL_TYPE);
                    
                    if(validateBufferSize(data_cast<UInt32>(size), image.width(), image.height(),
                        Image::depth(image.pixelType()), data_cast<Enum>(pixelType)))
                    {
                        m_input->setParameter(ConstImage::IMAGE, image);
                    }
                    else
                    {
                        throw WrongParameterValue(parameter(BUFFER_SIZE), *this, "Buffer size is too small for this image.");
                    }
                    
                    m_imageWidth = image.width();
                    m_imageHeight = image.height();
                    m_imageDepth = Image::depth(image.pixelType());
                    
                    m_clip->setParameter(Clip::LEFT, UInt32(0));
                    m_clip->setParameter(Clip::TOP, UInt32(0));
                    m_clip->setParameter(Clip::WIDTH, UInt32(m_imageWidth));
                    m_clip->setParameter(Clip::HEIGHT, UInt32(m_imageHeight));
                    
                    m_left->setMax(UInt32(0));
                    m_top->setMax(UInt32(0));
                    m_width->setMax(UInt32(m_imageWidth));
                    m_height->setMax(UInt32(m_imageHeight));
                    
                    break;
                }
                case NUM_BUFFERS:
                    m_buffer->setParameter(impl::CameraBuffer::NUM_BUFFERS, value);
                    m_imageQueue->setParameter(Queue::SIZE, value);
                    m_indexQueue->setParameter(Queue::SIZE, value);
                    break;
                case TRIGGER_MODE:
                    int triggerMode;
                    try
                    {
                        triggerMode = data_cast<Enum>(value);
                    }
                    catch(BadCast&)
                    {
                        throw WrongParameterType(parameter(TRIGGER_MODE), *this);
                    }
                    
                    switch(triggerMode)
                    {
                    case SOFTWARE:
                        m_trigger->setParameter(Trigger::STATE, Enum(Trigger::TRIGGER_ACTIVE));
                        break;
                    case INTERNAL:
                        m_trigger->setParameter(Trigger::STATE, Enum(Trigger::ALWAYS_PASS));
                        break;
                    case EXTERNAL:
                        m_trigger->setParameter(Trigger::STATE, Enum(Trigger::ALWAYS_STOP));
                        break;
                    default:
                        throw WrongParameterValue(parameter(TRIGGER_MODE), *this);
                    }
                    break;
                case FRAME_PERIOD:
                    m_period->setParameter(PeriodicDelay::PERIOD, value);
                    break;
                case BUFFER_SIZE:
                {
                    UInt32 size = data_cast<UInt32>(value);
                    DataRef image = getParameter(IMAGE);
                    DataRef pixelType = getParameter(PIXEL_TYPE);
                    
                    if(validateBufferSize(size, m_imageWidth, m_imageHeight, m_imageDepth, data_cast<Enum>(pixelType)))
                        m_buffer->setParameter(impl::CameraBuffer::BUFFER_SIZE, value);
                    else
                        throw WrongParameterValue(parameter(BUFFER_SIZE), *this, "This buffer size is too small for the current image size and pixel type.");
                    break;
                }
                case PIXEL_TYPE:
                {
                    DataRef size = getParameter(BUFFER_SIZE);
                    DataRef image = getParameter(IMAGE);
                    Enum pixelType = data_cast<Enum>(value);
                    
                    if(validateBufferSize(data_cast<UInt32>(size), m_imageWidth, m_imageHeight, m_imageDepth, pixelType))
                        m_pixelType->setParameter(ConvertPixelType::PIXEL_TYPE, value);
                    else
                        throw WrongParameterValue(parameter(PIXEL_TYPE), *this, "Buffer is too small to support this pixel size.");
                    break;
                }
                case LEFT:
                    m_clip->setParameter(Clip::LEFT, value);
                    try
                    {
                        UInt32 intValue = data_cast<UInt32>(value);
                        m_width->setMax(UInt32(m_imageWidth - intValue));
                    }
                    catch(BadCast&)
                    {
                        throw WrongParameterType(parameter(LEFT), *this);
                    }
                    break;
                case TOP:
                    m_clip->setParameter(Clip::TOP, value);
                    try
                    {
                        UInt32 intValue = data_cast<UInt32>(value);
                        m_height->setMax(UInt32(m_imageHeight - intValue));
                    }
                    catch(BadCast&)
                    {
                        throw WrongParameterType(parameter(TOP), *this);
                    }
                    break;
                case WIDTH:
                    m_clip->setParameter(Clip::WIDTH, value);
                    try
                    {
                        UInt32 intValue = data_cast<UInt32>(value);
                        m_left->setMax(UInt32(m_imageWidth - intValue));
                    }
                    catch(BadCast&)
                    {
                        throw WrongParameterType(parameter(WIDTH), *this);
                    }
                    break;
                case HEIGHT:
                    m_clip->setParameter(Clip::HEIGHT, value);
                    try
                    {
                        UInt32 intValue = data_cast<UInt32>(value);
                        m_top->setMax(UInt32(m_imageHeight - intValue));
                    }
                    catch(BadCast&)
                    {
                        throw WrongParameterType(parameter(HEIGHT), *this);
                    }
                    break;
                case EXPOSURE:
                    try
                    {
                        UInt32 intValue = data_cast<UInt32>(value);
                        m_exposure = intValue;
                        setRgbParameters();
                    }
                    catch(BadCast&)
                    {
                        throw WrongParameterType(parameter(EXPOSURE), *this);
                    }
                    break;
                case WHITE_BALANCE_RED:
                    try
                    {
                        Double fpValue = data_cast<Double>(value);
                        m_wbRed = std::max(0.0, std::min(double(WHITE_BALANCE_MAX), double(fpValue)));
                        setRgbParameters();
                    }
                    catch(BadCast&)
                    {
                        throw WrongParameterType(parameter(WHITE_BALANCE_RED), *this);
                    }
                    break;
                case WHITE_BALANCE_GREEN:
                    try
                    {
                        Double fpValue = data_cast<Double>(value);
                        m_wbGreen = std::max(0.0, std::min(double(WHITE_BALANCE_MAX), double(fpValue)));
                        setRgbParameters();
                    }
                    catch(BadCast&)
                    {
                        throw WrongParameterType(parameter(WHITE_BALANCE_GREEN), *this);
                    }
                    break;
                case WHITE_BALANCE_BLUE:
                    try
                    {
                        Double fpValue = data_cast<Double>(value);
                        m_wbBlue = std::max(0.0, std::min(double(WHITE_BALANCE_MAX), double(fpValue)));
                        setRgbParameters();
                    }
                    catch(BadCast&)
                    {
                        throw WrongParameterType(parameter(WHITE_BALANCE_BLUE), *this);
                    }
                    break;
                default:
                    throw WrongParameterId(id, *this);
                }
            }
            catch(BadCast&)
            {
                throw WrongParameterType(*parameters()[id], *this);
            }
        }

        const DataRef Camera::getParameter(unsigned int id) const
        {
            switch(id)
            {
            case OUTPUT_INDEX:
                return m_outputIndex; 
            case TRIGGER:
                return TriggerData();
            case TRIGGER_MODE:
            {
                DataRef value = m_trigger->getParameter(Trigger::STATE);
                const Enum& triggerState = data_cast<Enum>(value);
                
                switch(triggerState)
                {
                case Trigger::ALWAYS_PASS:
                    return Enum(INTERNAL);
                case Trigger::ALWAYS_STOP:
                    return Enum(EXTERNAL);
                case Trigger::TRIGGER_ACTIVE:
                    return Enum(SOFTWARE);
                default:
                    throw WrongParameterValue(parameter(id), *this);
                }
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
                return m_exposure;
            case WHITE_BALANCE_RED:
                return m_wbRed;
            case WHITE_BALANCE_GREEN:
                return m_wbGreen;
            case WHITE_BALANCE_BLUE:
                return m_wbBlue;
            case PIXEL_TYPE:
                 return m_pixelType->getParameter(ConvertPixelType::PIXEL_TYPE);
            default:
                throw WrongParameterId(id, *this);
            }
        }  
        
        void Camera::execute(DataProvider& provider)
        {
            // allow parameter access while waiting for data
            provider.unlockParameters();
            DataContainer image = m_imageQueue->getOutputData(Queue::OUTPUT);
            DataContainer index = m_indexQueue->getOutputData(Queue::OUTPUT);
            
            m_imageQueue->clearOutputData(Queue::OUTPUT);
            m_indexQueue->clearOutputData(Queue::OUTPUT);
            
            Id2DataPair imageMapper(OUTPUT, image);
            Id2DataPair indexMapper(INDEX, index);
            
            if(m_outputIndex)
                provider.sendOutputData(imageMapper && indexMapper);
            else
                provider.sendOutputData(imageMapper);
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
        
        const std::vector<const runtime::Description*> Camera::setupInputs()
        {
            std::vector<const Description*> inputs;
            
            return inputs;
        }
        
        const std::vector<const Description*> Camera::setupOutputs()
        {
            std::vector<const Description*> outputs;
            
            Description* output = new Description(OUTPUT, DataVariant::IMAGE);
            output->setTitle("Output");
            outputs.push_back(output);
            
            if(m_outputIndex)
            {
                Description* index = new Description(INDEX, DataVariant::UINT_32);
                index->setTitle("Index");
                outputs.push_back(index);
            }
            
            return outputs;
        }
        
        const std::vector<const Parameter*> Camera::setupInitParameters()
        {
            std::vector<const runtime::Parameter*> parameters;
            
            Parameter* outputIndex = new Parameter(OUTPUT_INDEX, DataVariant::BOOL);
            outputIndex->setTitle("Output index");
            outputIndex->setAccessMode(runtime::Parameter::NONE_WRITE);
            parameters.push_back(outputIndex);
            
            return parameters;
        }
        
        const std::vector<const Parameter*> Camera::setupParameters()
        {
            std::vector<const runtime::Parameter*> parameters;
            
            EnumParameter* triggerMode = new EnumParameter(TRIGGER_MODE);
            triggerMode->setTitle("Trigger mode");
            triggerMode->setAccessMode(runtime::Parameter::ACTIVATED_WRITE);
            triggerMode->add(EnumDescription(Enum(SOFTWARE), "Software trigger"));
            triggerMode->add(EnumDescription(Enum(INTERNAL), "Internal"));
            triggerMode->add(EnumDescription(Enum(EXTERNAL), "External"));
            parameters.push_back(triggerMode);
            
            Parameter* trigger = new Parameter(TRIGGER, DataVariant::TRIGGER);
            trigger->setTitle("Trigger");
            trigger->setAccessMode(runtime::Parameter::ACTIVATED_WRITE);
            parameters.push_back(trigger);
            
            NumericParameter<UInt32>* exposure = new NumericParameter<UInt32>(EXPOSURE);
            exposure->setTitle("Exposure (milliseconds)");
            exposure->setAccessMode(runtime::Parameter::ACTIVATED_WRITE);
            parameters.push_back(exposure);
            
            ParameterGroup* wbGroup = new ParameterGroup(WHITE_BALANCE_GROUP);
            wbGroup->setTitle("White balance");
            parameters.push_back(wbGroup);
            
            NumericParameter<Double>* wbRed = new NumericParameter<Double>(WHITE_BALANCE_RED, wbGroup);
            wbRed->setTitle("WB red");
            wbRed->setAccessMode(runtime::Parameter::ACTIVATED_WRITE);
            wbRed->setMin(Double(0));
            wbRed->setMax(Double(WHITE_BALANCE_MAX));
            parameters.push_back(wbRed);
            
            NumericParameter<Double>* wbGreen = new NumericParameter<Double>(WHITE_BALANCE_GREEN, wbGroup);
            wbGreen->setTitle("WB green");
            wbGreen->setAccessMode(runtime::Parameter::ACTIVATED_WRITE);
            wbGreen->setMin(Double(0));
            wbGreen->setMax(Double(WHITE_BALANCE_MAX));
            parameters.push_back(wbGreen);
            
            NumericParameter<Double>* wbBlue = new NumericParameter<Double>(WHITE_BALANCE_BLUE, wbGroup);
            wbBlue->setTitle("WB blue");
            wbBlue->setAccessMode(runtime::Parameter::ACTIVATED_WRITE);
            wbBlue->setMin(Double(0));
            wbBlue->setMax(Double(WHITE_BALANCE_MAX));
            parameters.push_back(wbBlue);
            
            NumericParameter<UInt32>* framePeriod = new NumericParameter<UInt32>(FRAME_PERIOD);
            framePeriod->setTitle("Frame period (milliseconds)");
            framePeriod->setAccessMode(runtime::Parameter::ACTIVATED_WRITE);
            parameters.push_back(framePeriod);
            
            NumericParameter<UInt32>* numBuffers = new NumericParameter<UInt32>(NUM_BUFFERS);
            numBuffers->setTitle("Number of buffers");
            numBuffers->setAccessMode(runtime::Parameter::INITIALIZED_WRITE);
            numBuffers->setMin(UInt32(1));
            parameters.push_back(numBuffers);
        
            NumericParameter<UInt32>* bufferSize = new NumericParameter<UInt32>(BUFFER_SIZE);
            bufferSize->setTitle("Buffer size in bytes");
            bufferSize->setAccessMode(runtime::Parameter::INITIALIZED_WRITE);
            parameters.push_back(bufferSize);
            
            Parameter* image = new Parameter(IMAGE, DataVariant::RGB_IMAGE);
            image->setTitle("Image");
            image->setAccessMode(runtime::Parameter::ACTIVATED_WRITE);
            parameters.push_back(image);
            
            ParameterGroup* roiGroup = new ParameterGroup(ROI_GROUP);
            roiGroup->setTitle("Region of interest");
            parameters.push_back(roiGroup);
        
            m_width = new NumericParameter<UInt32>(WIDTH, roiGroup);
            m_width->setTitle("ROI width");
            m_width->setAccessMode(runtime::Parameter::INITIALIZED_WRITE);
            parameters.push_back(m_width);
        
            m_height = new NumericParameter<UInt32>(HEIGHT, roiGroup);
            m_height->setTitle("ROI height");
            m_height->setAccessMode(runtime::Parameter::INITIALIZED_WRITE);
            parameters.push_back(m_height);
            
            m_top = new NumericParameter<UInt32>(TOP, roiGroup);
            m_top->setTitle("ROI top offset");
            m_top->setAccessMode(runtime::Parameter::INITIALIZED_WRITE);
            parameters.push_back(m_top);
            
            m_left = new NumericParameter<UInt32>(LEFT, roiGroup);
            m_left->setTitle("ROI left offset");
            m_left->setAccessMode(runtime::Parameter::INITIALIZED_WRITE);
            parameters.push_back(m_left);
            
            EnumParameter* pixelType = new EnumParameter(PIXEL_TYPE);
            pixelType->setTitle("Pixel type");
            pixelType->setAccessMode(runtime::Parameter::ACTIVATED_WRITE);
            pixelType->add(EnumDescription(Enum(runtime::Image::MONO_8), "Mono image 8-bit"));
            pixelType->add(EnumDescription(Enum(runtime::Image::RGB_24), "RGB image 24-bit"));
            pixelType->add(EnumDescription(Enum(runtime::Image::BGR_24), "BGR image 24-bit"));
            pixelType->add(EnumDescription(Enum(runtime::Image::BAYERBG_8), "Bayer BG pattern 8-bit"));
            pixelType->add(EnumDescription(Enum(runtime::Image::BAYERGB_8), "Bayer GB pattern 8-bit"));
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
  
        bool Camera::validateBufferSize(unsigned int bufferSize, unsigned int width, unsigned int height,
                                        unsigned int depth, const stromx::runtime::Enum outputType)
        {
            Image::PixelType outputPixelType = Image::PixelType(int(outputType));
            unsigned int outputSize = width * height * depth * Image::numChannels(outputPixelType);
        
            return outputSize <= (unsigned int)(bufferSize);
        }
    } 
}
