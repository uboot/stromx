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

#include "stromx/cvsupport/AdjustRgbChannels.h"
#include "stromx/cvsupport/Flicker.h"
#include "stromx/cvsupport/DummyCamera.h"
#include "stromx/cvsupport/Clip.h"
#include "stromx/cvsupport/ConstImage.h"
#include "stromx/cvsupport/ConvertPixelType.h"
#include "stromx/cvsupport/Image.h"
#include "stromx/cvsupport/impl/CameraBuffer.h"
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
#include <stromx/runtime/Block.h>

namespace stromx
{
    using namespace runtime;

    namespace cvsupport
    {
        const std::string DummyCamera::TYPE("DummyCamera");
        
        const std::string DummyCamera::PACKAGE(STROMX_CVSUPPORT_PACKAGE_NAME);
        const Version DummyCamera::VERSION(0, 1, 0);
        
        DummyCamera::DummyCamera()
          : OperatorKernel(TYPE, PACKAGE, VERSION, setupInitParameters()),
            m_stream(0),
            m_input(0),
            m_clip(0),
            m_adjustRgbChannels(0),
            m_flicker(0),
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
            m_stream = new Stream();
            
            m_input = m_stream->addOperator(new ConstImage);
            m_adjustRgbChannels = m_stream->addOperator(new AdjustRgbChannels);
            m_flicker = m_stream->addOperator(new Flicker);
            m_clip = m_stream->addOperator(new Clip);
            m_buffer = m_stream->addOperator(new impl::CameraBuffer);
            m_period = m_stream->addOperator(new PeriodicDelay);
            m_trigger = m_stream->addOperator(new Block);
            m_pixelType = m_stream->addOperator(new ConvertPixelType);
            m_imageQueue = m_stream->addOperator(new Queue);
            m_indexQueue = m_stream->addOperator(new Queue);
        }
        
        DummyCamera::~DummyCamera()
        {
            delete m_stream;
        }
        
        void DummyCamera::deinitialize()
        {
            // remove all operators from the stream and deinitialize them
            std::vector<Operator*> ops = m_stream->operators();
            for(std::vector<Operator*>::const_iterator iter = ops.begin();
                iter != ops.end();
                ++iter)
            {
                m_stream->deinitializeOperator(*iter);
            }
            
            OperatorKernel::deinitialize();
        }
        
        void DummyCamera::initialize()
        {
            OperatorKernel::initialize(setupInputs(), setupOutputs(), setupParameters());
            
            m_pixelType->setParameter(ConvertPixelType::DATA_FLOW,
                                      runtime::Enum(ConvertPixelType::MANUAL));
            
            m_stream->initializeOperator(m_input);
            m_stream->initializeOperator(m_adjustRgbChannels);
            m_stream->initializeOperator(m_flicker);
            m_stream->initializeOperator(m_clip);
            m_stream->initializeOperator(m_buffer);
            m_stream->initializeOperator(m_period);
            m_stream->initializeOperator(m_trigger);
            m_stream->initializeOperator(m_pixelType);
            m_stream->initializeOperator(m_imageQueue);
            m_stream->initializeOperator(m_indexQueue);
            
            m_stream->connect(m_input, ConstImage::OUTPUT, m_flicker, Flicker::INPUT);
            m_stream->connect(m_flicker, Flicker::OUTPUT, m_adjustRgbChannels, AdjustRgbChannels::INPUT);
            m_stream->connect(m_adjustRgbChannels, AdjustRgbChannels::OUTPUT, m_clip, Clip::INPUT);
            m_stream->connect(m_clip, Clip::OUTPUT, m_trigger, Block::INPUT);
            m_stream->connect(m_trigger, Block::OUTPUT, m_period, PeriodicDelay::INPUT);
            m_stream->connect(m_period, PeriodicDelay::OUTPUT, m_buffer, impl::CameraBuffer::INPUT);
            m_stream->connect(m_buffer, impl::CameraBuffer::OUTPUT, m_pixelType, ConvertPixelType::SOURCE);
            m_stream->connect(m_buffer, impl::CameraBuffer::BUFFER, m_pixelType, ConvertPixelType::DESTINATION);
            m_stream->connect(m_pixelType, ConvertPixelType::OUTPUT, m_imageQueue, Queue::INPUT);
            m_stream->connect(m_buffer, impl::CameraBuffer::INDEX, m_indexQueue, Queue::INPUT);
            
            Thread* frameThread = m_stream->addThread();
            frameThread->addInput(m_flicker, Flicker::INPUT);
            frameThread->addInput(m_adjustRgbChannels, AdjustRgbChannels::INPUT);
            frameThread->addInput(m_clip, Clip::INPUT);
            frameThread->addInput(m_trigger, Block::INPUT);
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

        void DummyCamera::setParameter(unsigned int id, const Data& value)
        {
            try
            {
                switch(id)
                {       
                case OUTPUT_INDEX:
                    m_outputIndex = data_cast<stromx::Bool>(value);
                    break;
                case TRIGGER:
                    m_trigger->setParameter(Block::TRIGGER, runtime::TriggerData());
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
                        m_trigger->setParameter(Block::STATE, Enum(Block::TRIGGER_ACTIVE));
                        break;
                    case INTERNAL:
                        m_trigger->setParameter(Block::STATE, Enum(Block::PASS_ALWAYS));
                        break;
                    case EXTERNAL:
                        m_trigger->setParameter(Block::STATE, Enum(Block::BLOCK_ALWAYS));
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
                        Float64 fpValue = data_cast<Float64>(value);
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
                        Float64 fpValue = data_cast<Float64>(value);
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
                        Float64 fpValue = data_cast<Float64>(value);
                        m_wbBlue = std::max(0.0, std::min(double(WHITE_BALANCE_MAX), double(fpValue)));
                        setRgbParameters();
                    }
                    catch(BadCast&)
                    {
                        throw WrongParameterType(parameter(WHITE_BALANCE_BLUE), *this);
                    }
                    break;
                case FLICKER_AMOUNT:
                    m_flicker->setParameter(Flicker::AMOUNT, value);
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

        const DataRef DummyCamera::getParameter(unsigned int id) const
        {
            switch(id)
            {
            case OUTPUT_INDEX:
                return m_outputIndex; 
            case TRIGGER:
                return TriggerData();
            case TRIGGER_MODE:
            {
                DataRef value = m_trigger->getParameter(Block::STATE);
                const Enum& triggerState = data_cast<Enum>(value);
                
                switch(triggerState)
                {
                case Block::PASS_ALWAYS:
                    return Enum(INTERNAL);
                case Block::BLOCK_ALWAYS:
                    return Enum(EXTERNAL);
                case Block::TRIGGER_ACTIVE:
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
            case FLICKER_AMOUNT:
                return m_flicker->getParameter(Flicker::AMOUNT);
            default:
                throw WrongParameterId(id, *this);
            }
        }  
        
        void DummyCamera::execute(DataProvider& provider)
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
        
        void DummyCamera::activate()
        {
            m_stream->start();
        }

        void DummyCamera::deactivate()
        {
            m_stream->stop();
            m_stream->join();
        }
        
        const std::vector<const runtime::Description*> DummyCamera::setupInputs()
        {
            std::vector<const Description*> inputs;
            
            return inputs;
        }
        
        const std::vector<const Description*> DummyCamera::setupOutputs()
        {
            std::vector<const Description*> outputs;
            
            Description* output = new Description(OUTPUT, Variant::IMAGE);
            output->setTitle("Output");
            outputs.push_back(output);
            
            if(m_outputIndex)
            {
                Description* index = new Description(INDEX, Variant::UINT_32);
                index->setTitle("Index");
                outputs.push_back(index);
            }
            
            return outputs;
        }
        
        const std::vector<const Parameter*> DummyCamera::setupInitParameters()
        {
            std::vector<const runtime::Parameter*> parameters;
            
            Parameter* outputIndex = new Parameter(OUTPUT_INDEX, Variant::BOOL);
            outputIndex->setTitle("Output index");
            outputIndex->setAccessMode(runtime::Parameter::NONE_WRITE);
            parameters.push_back(outputIndex);
            
            return parameters;
        }
        
        const std::vector<const Parameter*> DummyCamera::setupParameters()
        {
            std::vector<const runtime::Parameter*> parameters;
            
            EnumParameter* triggerMode = new EnumParameter(TRIGGER_MODE);
            triggerMode->setTitle("Trigger mode");
            triggerMode->setAccessMode(runtime::Parameter::ACTIVATED_WRITE);
            triggerMode->add(EnumDescription(Enum(SOFTWARE), "Software trigger"));
            triggerMode->add(EnumDescription(Enum(INTERNAL), "Internal"));
            triggerMode->add(EnumDescription(Enum(EXTERNAL), "External"));
            parameters.push_back(triggerMode);
            
            Parameter* trigger = new Parameter(TRIGGER, Variant::TRIGGER);
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
            
            NumericParameter<Float64>* wbRed = new NumericParameter<Float64>(WHITE_BALANCE_RED, wbGroup);
            wbRed->setTitle("WB red");
            wbRed->setAccessMode(runtime::Parameter::ACTIVATED_WRITE);
            wbRed->setMin(Float64(0));
            wbRed->setMax(Float64(WHITE_BALANCE_MAX));
            parameters.push_back(wbRed);
            
            NumericParameter<Float64>* wbGreen = new NumericParameter<Float64>(WHITE_BALANCE_GREEN, wbGroup);
            wbGreen->setTitle("WB green");
            wbGreen->setAccessMode(runtime::Parameter::ACTIVATED_WRITE);
            wbGreen->setMin(Float64(0));
            wbGreen->setMax(Float64(WHITE_BALANCE_MAX));
            parameters.push_back(wbGreen);
            
            NumericParameter<Float64>* wbBlue = new NumericParameter<Float64>(WHITE_BALANCE_BLUE, wbGroup);
            wbBlue->setTitle("WB blue");
            wbBlue->setAccessMode(runtime::Parameter::ACTIVATED_WRITE);
            wbBlue->setMin(Float64(0));
            wbBlue->setMax(Float64(WHITE_BALANCE_MAX));
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
            
            Parameter* image = new Parameter(IMAGE, Variant::RGB_IMAGE);
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
            
            NumericParameter<Float64>* flicker = new NumericParameter<Float64>(FLICKER_AMOUNT);
            flicker->setTitle("Flicker amount");
            flicker->setAccessMode(runtime::Parameter::ACTIVATED_WRITE);
            flicker->setMin(Float64(0));
            flicker->setMax(Float64(1.0));
            parameters.push_back(flicker);
                                        
            return parameters;
        }
        
        void DummyCamera::setRgbParameters()
        {
            double exposureCoeff = double(m_exposure) / double(BASE_EXPOSURE);
            
            m_adjustRgbChannels->setParameter(AdjustRgbChannels::RED, Float64(exposureCoeff * m_wbRed));
            m_adjustRgbChannels->setParameter(AdjustRgbChannels::GREEN, Float64(exposureCoeff * m_wbGreen));
            m_adjustRgbChannels->setParameter(AdjustRgbChannels::BLUE, Float64(exposureCoeff * m_wbBlue));
        }
  
        bool DummyCamera::validateBufferSize(unsigned int bufferSize, unsigned int width, unsigned int height,
                                        unsigned int depth, const stromx::runtime::Enum outputType)
        {
            Image::PixelType outputPixelType = Image::PixelType(int(outputType));
            unsigned int outputSize = width * height * depth * Image::numChannels(outputPixelType);
        
            return outputSize <= (unsigned int)(bufferSize);
        }
    } 
}
