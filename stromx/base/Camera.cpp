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

#include "AdjustRgbChannels.h"
#include "Camera.h"
#include "Clip.h"
#include "ConstImage.h"
#include "ConvertPixelType.h"
#include "Image.h"
#include "impl/CameraBuffer.h"
#include <stromx/core/OperatorException.h>
#include <stromx/core/DataContainer.h>
#include <stromx/core/DataProvider.h>
#include <stromx/core/Id2DataPair.h>
#include <stromx/core/Id2DataComposite.h>
#include <stromx/core/Stream.h>
#include <stromx/core/Thread.h>
#include <stromx/core/EnumParameter.h>
#include <stromx/core/NumericParameter.h>
#include <stromx/core/OperatorException.h>
#include <stromx/core/Operator.h>
#include <stromx/core/PeriodicDelay.h>
#include <stromx/core/ParameterGroup.h>
#include <stromx/core/Queue.h>
#include <stromx/core/Trigger.h>

namespace stromx
{
    using namespace core;

    namespace base
    {
        const std::string Camera::TYPE("Camera");
        
        const std::string Camera::PACKAGE(STROMX_BASE_PACKAGE_NAME);
        const Version Camera::VERSION(BASE_VERSION_MAJOR, BASE_VERSION_MINOR, BASE_VERSION_PATCH);
        
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
            m_imageWidth(0),
            m_imageHeight(0),
            m_exposure(BASE_EXPOSURE),
            m_wbRed(1.0),
            m_wbGreen(1.0),
            m_wbBlue(1.0),
            m_valueSoftware(SOFTWARE),
            m_valueInternal(INTERNAL),
            m_valueExternal(EXTERNAL)
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
        }

        void Camera::setParameter(unsigned int id, const Data& value)
        {
            try
            {
                switch(id)
                {       
                case OUTPUT_INDEX:
                    m_outputIndex = data_cast<const stromx::Bool &>(value);
                    break;
                case TRIGGER:
                    m_trigger->setParameter(Trigger::TRIGGER, core::TriggerData());
                    break;
                case IMAGE:
                {
                    const stromx::Image & image = data_cast<const stromx::Image &>(value);
                    UInt32 size = data_cast<const UInt32 &>(getParameter(BUFFER_SIZE));
                    Enum pixelType = data_cast<const Enum &>(getParameter(PIXEL_TYPE));
                    
                    if(validateBufferSize(size, image, pixelType))
                        m_input->setParameter(ConstImage::IMAGE, image);
                    else
                        throw WrongParameterValue(parameter(BUFFER_SIZE), *this);
                    
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
                        triggerMode = data_cast<const Enum&>(value);
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
                    UInt32 size = data_cast<const UInt32 &>(value);
                    const stromx::Image & image = data_cast<const stromx::Image &>(getParameter(IMAGE));
                    Enum pixelType = data_cast<const Enum &>(getParameter(PIXEL_TYPE));
                    
                    if(validateBufferSize(size, image, pixelType))
                        m_buffer->setParameter(impl::CameraBuffer::BUFFER_SIZE, value);
                    else
                        throw WrongParameterValue(parameter(BUFFER_SIZE), *this);
                    break;
                }
                case PIXEL_TYPE:
                {
                    UInt32 size = data_cast<const UInt32 &>(getParameter(BUFFER_SIZE));
                    const stromx::Image & image = data_cast<const stromx::Image &>(getParameter(IMAGE));
                    Enum pixelType = data_cast<const Enum &>(value);
                    
                    if(validateBufferSize(size, image, pixelType))
                        m_pixelType->setParameter(ConvertPixelType::PIXEL_TYPE, value);
                    else
                        throw WrongParameterValue(parameter(BUFFER_SIZE), *this);
                    break;
                }
                case LEFT:
                    m_clip->setParameter(Clip::LEFT, value);
                    try
                    {
                        UInt32 intValue = data_cast<const UInt32 &>(value);
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
                        UInt32 intValue = data_cast<const UInt32 &>(value);
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
                        UInt32 intValue = data_cast<const UInt32 &>(value);
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
                        UInt32 intValue = data_cast<const UInt32 &>(value);
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
                        UInt32 intValue = data_cast<const UInt32 &>(value);
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
                        Double fpValue = data_cast<const Double &>(value);
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
                        Double fpValue = data_cast<const Double &>(value);
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
                        Double fpValue = data_cast<const Double &>(value);
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

        const Data& Camera::getParameter(unsigned int id) const
        {
            switch(id)
            {
            case OUTPUT_INDEX:
                return m_outputIndex; 
            case TRIGGER:
                return m_valueTrigger;
            case TRIGGER_MODE:
            {
                const Data& value = m_trigger->getParameter(Trigger::STATE);
                const Enum& triggerState = data_cast<const Enum&>(value);
                
                switch(triggerState)
                {
                case Trigger::ALWAYS_PASS:
                    return m_valueInternal;
                case Trigger::ALWAYS_STOP:
                    return m_valueExternal;
                case Trigger::TRIGGER_ACTIVE:
                    return m_valueSoftware;
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
        
        const std::vector<const core::Description*> Camera::setupInputs()
        {
            std::vector<const Description*> inputs;
            
            return inputs;
        }
        
        const std::vector<const Description*> Camera::setupOutputs()
        {
            std::vector<const Description*> outputs;
            
            Description* output = new Description(OUTPUT, DataVariant::IMAGE);
            output->setDoc("Output");
            outputs.push_back(output);
            
            if(m_outputIndex)
            {
                Description* index = new Description(INDEX, DataVariant::UINT_32);
                index->setDoc("Index");
                outputs.push_back(index);
            }
            
            return outputs;
        }
        
        const std::vector<const Parameter*> Camera::setupInitParameters()
        {
            std::vector<const core::Parameter*> parameters;
            
            Parameter* outputIndex = new Parameter(OUTPUT_INDEX, DataVariant::BOOL);
            outputIndex->setDoc("Output index");
            outputIndex->setAccessMode(core::Parameter::NONE_WRITE);
            parameters.push_back(outputIndex);
            
            return parameters;
        }
        
        const std::vector<const Parameter*> Camera::setupParameters()
        {
            std::vector<const core::Parameter*> parameters;
            
            EnumParameter* triggerMode = new EnumParameter(TRIGGER_MODE);
            triggerMode->setDoc("Trigger mode");
            triggerMode->setAccessMode(core::Parameter::ACTIVATED_WRITE);
            triggerMode->add(EnumDescription(Enum(SOFTWARE), "Software trigger"));
            triggerMode->add(EnumDescription(Enum(INTERNAL), "Internal"));
            triggerMode->add(EnumDescription(Enum(EXTERNAL), "External"));
            parameters.push_back(triggerMode);
            
            Parameter* trigger = new Parameter(TRIGGER, DataVariant::TRIGGER);
            trigger->setDoc("Trigger");
            trigger->setAccessMode(core::Parameter::ACTIVATED_WRITE);
            parameters.push_back(trigger);
            
            NumericParameter<UInt32>* exposure = new NumericParameter<UInt32>(EXPOSURE);
            exposure->setDoc("Exposure (milliseconds)");
            exposure->setAccessMode(core::Parameter::ACTIVATED_WRITE);
            parameters.push_back(exposure);
            
            ParameterGroup* wbGroup = new ParameterGroup(WHITE_BALANCE_GROUP);
            wbGroup->setDoc("White balance");
            parameters.push_back(wbGroup);
            
            NumericParameter<Double>* wbRed = new NumericParameter<Double>(WHITE_BALANCE_RED, wbGroup);
            wbRed->setDoc("WB red");
            wbRed->setAccessMode(core::Parameter::ACTIVATED_WRITE);
            wbRed->setMin(Double(0));
            wbRed->setMax(Double(WHITE_BALANCE_MAX));
            parameters.push_back(wbRed);
            
            NumericParameter<Double>* wbGreen = new NumericParameter<Double>(WHITE_BALANCE_GREEN, wbGroup);
            wbGreen->setDoc("WB green");
            wbGreen->setAccessMode(core::Parameter::ACTIVATED_WRITE);
            wbGreen->setMin(Double(0));
            wbGreen->setMax(Double(WHITE_BALANCE_MAX));
            parameters.push_back(wbGreen);
            
            NumericParameter<Double>* wbBlue = new NumericParameter<Double>(WHITE_BALANCE_BLUE, wbGroup);
            wbBlue->setDoc("WB blue");
            wbBlue->setAccessMode(core::Parameter::ACTIVATED_WRITE);
            wbBlue->setMin(Double(0));
            wbBlue->setMax(Double(WHITE_BALANCE_MAX));
            parameters.push_back(wbBlue);
            
            NumericParameter<UInt32>* framePeriod = new NumericParameter<UInt32>(FRAME_PERIOD);
            framePeriod->setDoc("Frame period (milliseconds)");
            framePeriod->setAccessMode(core::Parameter::ACTIVATED_WRITE);
            parameters.push_back(framePeriod);
            
            NumericParameter<UInt32>* numBuffers = new NumericParameter<UInt32>(NUM_BUFFERS);
            numBuffers->setDoc("Number of buffers");
            numBuffers->setAccessMode(core::Parameter::INITIALIZED_WRITE);
            numBuffers->setMin(UInt32(1));
            parameters.push_back(numBuffers);
        
            NumericParameter<UInt32>* bufferSize = new NumericParameter<UInt32>(BUFFER_SIZE);
            bufferSize->setDoc("Buffer size in bytes");
            bufferSize->setAccessMode(core::Parameter::INITIALIZED_WRITE);
            parameters.push_back(bufferSize);
            
            Parameter* image = new Parameter(IMAGE, DataVariant::RGB_IMAGE);
            image->setDoc("Image");
            image->setAccessMode(core::Parameter::ACTIVATED_WRITE);
            parameters.push_back(image);
            
            ParameterGroup* roiGroup = new ParameterGroup(ROI_GROUP);
            roiGroup->setDoc("Region of interest");
            parameters.push_back(roiGroup);
        
            m_width = new NumericParameter<UInt32>(WIDTH, roiGroup);
            m_width->setDoc("ROI width");
            m_width->setAccessMode(core::Parameter::INITIALIZED_WRITE);
            parameters.push_back(m_width);
        
            m_height = new NumericParameter<UInt32>(HEIGHT, roiGroup);
            m_height->setDoc("ROI height");
            m_height->setAccessMode(core::Parameter::INITIALIZED_WRITE);
            parameters.push_back(m_height);
            
            m_top = new NumericParameter<UInt32>(TOP, roiGroup);
            m_top->setDoc("ROI top offset");
            m_top->setAccessMode(core::Parameter::INITIALIZED_WRITE);
            parameters.push_back(m_top);
            
            m_left = new NumericParameter<UInt32>(LEFT, roiGroup);
            m_left->setDoc("ROI left offset");
            m_left->setAccessMode(core::Parameter::INITIALIZED_WRITE);
            parameters.push_back(m_left);
            
            EnumParameter* pixelType = new EnumParameter(PIXEL_TYPE);
            pixelType->setDoc("Pixel type");
            pixelType->setAccessMode(core::Parameter::ACTIVATED_WRITE);
            pixelType->add(EnumDescription(Enum(core::Image::MONO_8), "Mono image 8-bit"));
            pixelType->add(EnumDescription(Enum(core::Image::RGB_24), "RGB image 24-bit"));
            pixelType->add(EnumDescription(Enum(core::Image::BGR_24), "BGR image 24-bit"));
            pixelType->add(EnumDescription(Enum(core::Image::BAYERBG_8), "Bayer BG pattern 8-bit"));
            pixelType->add(EnumDescription(Enum(core::Image::BAYERGB_8), "Bayer GB pattern 8-bit"));
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
        
        bool Camera::validateBufferSize(const UInt32 bufferSize, const core::Image & image, const core::Enum outputType)
        {
            Image::PixelType pixelType = Image::PixelType(int(outputType));
            unsigned int outputSize = image.width() * image.height() * 
                                      Image::depth(pixelType) * Image::numChannels(pixelType);
        
            return outputSize <= (unsigned int)(bufferSize);
        }
    } 
}
