#include "stromx/cvimgproc/Sobel.h"

#include "stromx/cvimgproc/Utility.h"
#include <stromx/cvsupport/Image.h>
#include <stromx/cvsupport/Matrix.h>
#include <stromx/cvsupport/Utilities.h>
#include <stromx/runtime/DataContainer.h>
#include <stromx/runtime/DataProvider.h>
#include <stromx/runtime/Id2DataComposite.h>
#include <stromx/runtime/Id2DataPair.h>
#include <stromx/runtime/ReadAccess.h>
#include <stromx/runtime/WriteAccess.h>
#include <opencv2/imgproc/imgproc.hpp>

namespace stromx
{
    namespace cvimgproc
    {
        const std::string Sobel::PACKAGE(STROMX_CVIMGPROC_PACKAGE_NAME);
        const runtime::Version Sobel::VERSION(STROMX_CVIMGPROC_VERSION_MAJOR, STROMX_CVIMGPROC_VERSION_MINOR, STROMX_CVIMGPROC_VERSION_PATCH);
        const std::string Sobel::TYPE("Sobel");
        
        Sobel::Sobel()
          : runtime::OperatorKernel(TYPE, PACKAGE, VERSION, setupInitParameters()),
            m_ddepth(0),
            m_delta(0.0),
            m_dx(1),
            m_dy(0),
            m_ksize(3),
            m_scale(1.0),
            m_dataFlow()
        {
        }
        
        const runtime::DataRef Sobel::getParameter(unsigned int id) const
        {
            switch(id)
            {
            case DDEPTH:
                return m_ddepth;
            case DELTA:
                return m_delta;
            case DX:
                return m_dx;
            case DY:
                return m_dy;
            case KSIZE:
                return m_ksize;
            case SCALE:
                return m_scale;
            case DATA_FLOW:
                return m_dataFlow;
            default:
                throw runtime::WrongParameterId(id, *this);
            }
        }
        
        void Sobel::setParameter(unsigned int id, const runtime::Data& value)
        {
            try
            {
                switch(id)
                {
                case DDEPTH:
                    {
                        const runtime::Enum & castedValue = runtime::data_cast<runtime::Enum>(value);
                        if(! castedValue.variant().isVariant(runtime::DataVariant::ENUM))
                        {
                            throw runtime::WrongParameterType(parameter(id), *this);
                        }
                        checkEnumValue(castedValue, m_ddepthParameter, *this);
                        m_ddepth = castedValue;
                    }
                    break;
                case DELTA:
                    {
                        const runtime::Float64 & castedValue = runtime::data_cast<runtime::Float64>(value);
                        if(! castedValue.variant().isVariant(runtime::DataVariant::FLOAT_64))
                        {
                            throw runtime::WrongParameterType(parameter(id), *this);
                        }
                        checkNumericValue(castedValue, m_deltaParameter, *this);
                        m_delta = castedValue;
                    }
                    break;
                case DX:
                    {
                        const runtime::UInt32 & castedValue = runtime::data_cast<runtime::UInt32>(value);
                        if(! castedValue.variant().isVariant(runtime::DataVariant::UINT_32))
                        {
                            throw runtime::WrongParameterType(parameter(id), *this);
                        }
                        checkNumericValue(castedValue, m_dxParameter, *this);
                        m_dx = castedValue;
                    }
                    break;
                case DY:
                    {
                        const runtime::UInt32 & castedValue = runtime::data_cast<runtime::UInt32>(value);
                        if(! castedValue.variant().isVariant(runtime::DataVariant::UINT_32))
                        {
                            throw runtime::WrongParameterType(parameter(id), *this);
                        }
                        checkNumericValue(castedValue, m_dyParameter, *this);
                        m_dy = castedValue;
                    }
                    break;
                case KSIZE:
                    {
                        const runtime::UInt32 & castedValue = runtime::data_cast<runtime::UInt32>(value);
                        if(! castedValue.variant().isVariant(runtime::DataVariant::UINT_32))
                        {
                            throw runtime::WrongParameterType(parameter(id), *this);
                        }
                        checkNumericValue(castedValue, m_ksizeParameter, *this);
                        if(int(castedValue) % 2 == 0)
                            throw runtime::WrongParameterValue(*m_ksizeParameter, *this, "Only odd values are allowed");
                        m_ksize = castedValue;
                    }
                    break;
                case SCALE:
                    {
                        const runtime::Float64 & castedValue = runtime::data_cast<runtime::Float64>(value);
                        if(! castedValue.variant().isVariant(runtime::DataVariant::FLOAT_64))
                        {
                            throw runtime::WrongParameterType(parameter(id), *this);
                        }
                        checkNumericValue(castedValue, m_scaleParameter, *this);
                        m_scale = castedValue;
                    }
                    break;
                case DATA_FLOW:
                    {
                        const runtime::Enum & castedValue = runtime::data_cast<runtime::Enum>(value);
                        if(! castedValue.variant().isVariant(runtime::DataVariant::ENUM))
                        {
                            throw runtime::WrongParameterType(parameter(id), *this);
                        }
                        checkEnumValue(castedValue, m_dataFlowParameter, *this);
                        m_dataFlow = castedValue;
                    }
                    break;
                default:
                    throw runtime::WrongParameterId(id, *this);
                }
            }
            catch(runtime::BadCast&)
            {
                throw runtime::WrongParameterType(parameter(id), *this);
            }
        }
        
        const std::vector<const runtime::Parameter*> Sobel::setupInitParameters()
        {
            std::vector<const runtime::Parameter*> parameters;
            
            m_dataFlowParameter = new runtime::EnumParameter(DATA_FLOW);
            m_dataFlowParameter->setAccessMode(runtime::Parameter::NONE_WRITE);
            m_dataFlowParameter->setTitle("Data flow");
            m_dataFlowParameter->add(runtime::EnumDescription(runtime::Enum(MANUAL), "Manual"));
            m_dataFlowParameter->add(runtime::EnumDescription(runtime::Enum(ALLOCATE), "Allocate"));
            parameters.push_back(m_dataFlowParameter);
            
            return parameters;
        }
        
        const std::vector<const runtime::Parameter*> Sobel::setupParameters()
        {
            std::vector<const runtime::Parameter*> parameters;
            
            switch(int(m_dataFlow))
            {
            case(MANUAL):
                {
                    m_ddepthParameter = new runtime::EnumParameter(DDEPTH);
                    m_ddepthParameter->setAccessMode(runtime::Parameter::ACTIVATED_WRITE);
                    m_ddepthParameter->setTitle("Destination depth");
                    m_ddepthParameter->add(runtime::EnumDescription(runtime::Enum(SAME), "Same as input"));
                    m_ddepthParameter->add(runtime::EnumDescription(runtime::Enum(DEPTH_8_BIT), "8-bit"));
                    m_ddepthParameter->add(runtime::EnumDescription(runtime::Enum(DEPTH_16_BIT), "16-bit"));
                    parameters.push_back(m_ddepthParameter);
                    
                    m_dxParameter = new runtime::NumericParameter<runtime::UInt32>(DX);
                    m_dxParameter->setAccessMode(runtime::Parameter::ACTIVATED_WRITE);
                    m_dxParameter->setTitle("Order X derivative");
                    parameters.push_back(m_dxParameter);
                    
                    m_dyParameter = new runtime::NumericParameter<runtime::UInt32>(DY);
                    m_dyParameter->setAccessMode(runtime::Parameter::ACTIVATED_WRITE);
                    m_dyParameter->setTitle("Order Y derivative");
                    parameters.push_back(m_dyParameter);
                    
                    m_ksizeParameter = new runtime::NumericParameter<runtime::UInt32>(KSIZE);
                    m_ksizeParameter->setAccessMode(runtime::Parameter::ACTIVATED_WRITE);
                    m_ksizeParameter->setTitle("Kernel size");
                    m_ksizeParameter->setMax(runtime::UInt32(7));
                    m_ksizeParameter->setMin(runtime::UInt32(1));
                    m_ksizeParameter->setStep(runtime::UInt32(2));
                    parameters.push_back(m_ksizeParameter);
                    
                    m_scaleParameter = new runtime::NumericParameter<runtime::Float64>(SCALE);
                    m_scaleParameter->setAccessMode(runtime::Parameter::ACTIVATED_WRITE);
                    m_scaleParameter->setTitle("Scale");
                    parameters.push_back(m_scaleParameter);
                    
                    m_deltaParameter = new runtime::NumericParameter<runtime::Float64>(DELTA);
                    m_deltaParameter->setAccessMode(runtime::Parameter::ACTIVATED_WRITE);
                    m_deltaParameter->setTitle("Delta");
                    parameters.push_back(m_deltaParameter);
                    
                }
                break;
            case(ALLOCATE):
                {
                    m_ddepthParameter = new runtime::EnumParameter(DDEPTH);
                    m_ddepthParameter->setAccessMode(runtime::Parameter::ACTIVATED_WRITE);
                    m_ddepthParameter->setTitle("Destination depth");
                    m_ddepthParameter->add(runtime::EnumDescription(runtime::Enum(SAME), "Same as input"));
                    m_ddepthParameter->add(runtime::EnumDescription(runtime::Enum(DEPTH_8_BIT), "8-bit"));
                    m_ddepthParameter->add(runtime::EnumDescription(runtime::Enum(DEPTH_16_BIT), "16-bit"));
                    parameters.push_back(m_ddepthParameter);
                    
                    m_dxParameter = new runtime::NumericParameter<runtime::UInt32>(DX);
                    m_dxParameter->setAccessMode(runtime::Parameter::ACTIVATED_WRITE);
                    m_dxParameter->setTitle("Order X derivative");
                    parameters.push_back(m_dxParameter);
                    
                    m_dyParameter = new runtime::NumericParameter<runtime::UInt32>(DY);
                    m_dyParameter->setAccessMode(runtime::Parameter::ACTIVATED_WRITE);
                    m_dyParameter->setTitle("Order Y derivative");
                    parameters.push_back(m_dyParameter);
                    
                    m_ksizeParameter = new runtime::NumericParameter<runtime::UInt32>(KSIZE);
                    m_ksizeParameter->setAccessMode(runtime::Parameter::ACTIVATED_WRITE);
                    m_ksizeParameter->setTitle("Kernel size");
                    m_ksizeParameter->setMax(runtime::UInt32(7));
                    m_ksizeParameter->setMin(runtime::UInt32(1));
                    m_ksizeParameter->setStep(runtime::UInt32(2));
                    parameters.push_back(m_ksizeParameter);
                    
                    m_scaleParameter = new runtime::NumericParameter<runtime::Float64>(SCALE);
                    m_scaleParameter->setAccessMode(runtime::Parameter::ACTIVATED_WRITE);
                    m_scaleParameter->setTitle("Scale");
                    parameters.push_back(m_scaleParameter);
                    
                    m_deltaParameter = new runtime::NumericParameter<runtime::Float64>(DELTA);
                    m_deltaParameter->setAccessMode(runtime::Parameter::ACTIVATED_WRITE);
                    m_deltaParameter->setTitle("Delta");
                    parameters.push_back(m_deltaParameter);
                    
                }
                break;
            }
            
            return parameters;
        }
        
        const std::vector<const runtime::Description*> Sobel::setupInputs()
        {
            std::vector<const runtime::Description*> inputs;
            
            switch(int(m_dataFlow))
            {
            case(MANUAL):
                {
                    runtime::Description* src = new runtime::Description(SRC, runtime::DataVariant::IMAGE);
                    src->setTitle("Source");
                    inputs.push_back(src);
                    
                    runtime::Description* dst = new runtime::Description(DST, runtime::DataVariant::IMAGE);
                    dst->setTitle("Destination");
                    inputs.push_back(dst);
                    
                }
                break;
            case(ALLOCATE):
                {
                    runtime::Description* src = new runtime::Description(SRC, runtime::DataVariant::IMAGE);
                    src->setTitle("Source");
                    inputs.push_back(src);
                    
                }
                break;
            }
            
            return inputs;
        }
        
        const std::vector<const runtime::Description*> Sobel::setupOutputs()
        {
            std::vector<const runtime::Description*> outputs;
            
            switch(int(m_dataFlow))
            {
            case(MANUAL):
                {
                    runtime::Description* dst = new runtime::Description(DST, runtime::DataVariant::IMAGE);
                    dst->setTitle("Destination");
                    outputs.push_back(dst);
                    
                }
                break;
            case(ALLOCATE):
                {
                    runtime::Description* dst = new runtime::Description(DST, runtime::DataVariant::IMAGE);
                    dst->setTitle("Destination");
                    outputs.push_back(dst);
                    
                }
                break;
            }
            
            return outputs;
        }
        
        void Sobel::initialize()
        {
            runtime::OperatorKernel::initialize(setupInputs(), setupOutputs(), setupParameters());
        }
        
        void Sobel::execute(runtime::DataProvider & provider)
        {
            switch(int(m_dataFlow))
            {
            case(MANUAL):
                {
                    runtime::Id2DataPair srcInMapper(SRC);
                    runtime::Id2DataPair dstInMapper(DST);
                    
                    provider.receiveInputData(srcInMapper && dstInMapper);
                    
                    const runtime::Data* srcData = 0;
                    runtime::Data* dstData = 0;
                    
                    runtime::ReadAccess<> srcReadAccess;
                    runtime::DataContainer inContainer = dstInMapper.data();
                    runtime::WriteAccess<> writeAccess(inContainer);
                    dstData = &writeAccess();
                    
                    if(srcInMapper.data() == inContainer)
                    {
                        throw runtime::InputError(SRC, *this, "Can not operate in place.");
                    }
                    else
                    {
                        srcReadAccess = runtime::ReadAccess<>(srcInMapper.data());
                        srcData = &srcReadAccess();
                    }
                    
                    if(! srcData->variant().isVariant(runtime::DataVariant::IMAGE))
                    {
                        throw runtime::InputError(SRC, *this, "Wrong input data variant.");
                    }
                    if(! dstData->variant().isVariant(runtime::DataVariant::IMAGE))
                    {
                        throw runtime::InputError(DST, *this, "Wrong input data variant.");
                    }
                    
                    const runtime::Image* srcCastedData = runtime::data_cast<runtime::Image>(srcData);
                    runtime::Image * dstCastedData = runtime::data_cast<runtime::Image>(dstData);
                    
                    runtime::Image::PixelType pixelType = cvsupport::computeOutPixelType(convertDdepth(m_ddepth), srcCastedData->pixelType());
                    unsigned int stride = runtime::Image::pixelSize(pixelType) * srcCastedData->width();
                    dstCastedData->initializeImage(srcCastedData->width(), srcCastedData->height(), stride, dstCastedData->data(), pixelType);
                    
                    cv::Mat srcCvData = cvsupport::getOpenCvMat(*srcCastedData);
                    cv::Mat dstCvData = cvsupport::getOpenCvMat(*dstCastedData);
                    int ddepthCvData = convertDdepth(m_ddepth);
                    int dxCvData = int(m_dx);
                    int dyCvData = int(m_dy);
                    int ksizeCvData = int(m_ksize);
                    double scaleCvData = double(m_scale);
                    double deltaCvData = double(m_delta);
                    
                    cv::Sobel(srcCvData, dstCvData, ddepthCvData, dxCvData, dyCvData, ksizeCvData, scaleCvData, deltaCvData);
                    
                    runtime::DataContainer outContainer = inContainer;
                    runtime::Id2DataPair outputMapper(DST, outContainer);
                    
                    provider.sendOutputData(outputMapper);
                }
                break;
            case(ALLOCATE):
                {
                    runtime::Id2DataPair srcInMapper(SRC);
                    
                    provider.receiveInputData(srcInMapper);
                    
                    const runtime::Data* srcData = 0;
                    
                    runtime::ReadAccess<> srcReadAccess;
                    
                    srcReadAccess = runtime::ReadAccess<>(srcInMapper.data());
                    srcData = &srcReadAccess();
                    
                    if(! srcData->variant().isVariant(runtime::DataVariant::IMAGE))
                    {
                        throw runtime::InputError(SRC, *this, "Wrong input data variant.");
                    }
                    
                    const runtime::Image* srcCastedData = runtime::data_cast<runtime::Image>(srcData);
                    
                    cv::Mat srcCvData = cvsupport::getOpenCvMat(*srcCastedData);
                    cv::Mat dstCvData;
                    int ddepthCvData = convertDdepth(m_ddepth);
                    int dxCvData = int(m_dx);
                    int dyCvData = int(m_dy);
                    int ksizeCvData = int(m_ksize);
                    double scaleCvData = double(m_scale);
                    double deltaCvData = double(m_delta);
                    
                    cv::Sobel(srcCvData, dstCvData, ddepthCvData, dxCvData, dyCvData, ksizeCvData, scaleCvData, deltaCvData);
                    
                    runtime::Image* dstCastedData = new cvsupport::Image(dstCvData);
                    runtime::DataContainer outContainer = runtime::DataContainer(dstCastedData);
                    runtime::Id2DataPair outputMapper(DST, outContainer);
                    
                    runtime::Image::PixelType pixelType = cvsupport::computeOutPixelType(convertDdepth(m_ddepth), srcCastedData->pixelType());
                    unsigned int stride = runtime::Image::pixelSize(pixelType) * srcCastedData->width();
                    dstCastedData->initializeImage(dstCastedData->width(), dstCastedData->height(), stride, dstCastedData->data(), pixelType);
                    provider.sendOutputData(outputMapper);
                }
                break;
            }
        }
        
        int Sobel::convertDdepth(const runtime::Enum & value)
        {
            switch(int(value))
            {
            case SAME:
                return -1;
            case DEPTH_8_BIT:
                return CV_8U;
            case DEPTH_16_BIT:
                return CV_16U;
            default:
                throw runtime::WrongParameterValue(parameter(DDEPTH), *this);
            }
        }
        
    }
}

