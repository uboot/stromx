#include "stromx/cvimgproc/Resize.h"

#include "stromx/cvimgproc/Locale.h"
#include "stromx/cvimgproc/Utility.h"
#include <stromx/cvsupport/Image.h>
#include <stromx/cvsupport/Matrix.h>
#include <stromx/cvsupport/Utilities.h>
#include <stromx/runtime/DataContainer.h>
#include <stromx/runtime/DataProvider.h>
#include <stromx/runtime/Id2DataComposite.h>
#include <stromx/runtime/Id2DataPair.h>
#include <stromx/runtime/ReadAccess.h>
#include <stromx/runtime/VariantComposite.h>
#include <stromx/runtime/WriteAccess.h>
#include <opencv2/imgproc/imgproc.hpp>

namespace stromx
{
    namespace cvimgproc
    {
        const std::string Resize::PACKAGE(STROMX_CVIMGPROC_PACKAGE_NAME);
        const runtime::Version Resize::VERSION(STROMX_CVIMGPROC_VERSION_MAJOR, STROMX_CVIMGPROC_VERSION_MINOR, STROMX_CVIMGPROC_VERSION_PATCH);
        const std::string Resize::TYPE("Resize");
        
        Resize::Resize()
          : runtime::OperatorKernel(TYPE, PACKAGE, VERSION, setupInitParameters()),
            m_dsizex(),
            m_dsizey(),
            m_fx(1.0),
            m_fy(1.0),
            m_interpolation(1),
            m_dataFlow()
        {
        }
        
        const runtime::DataRef Resize::getParameter(unsigned int id) const
        {
            switch(id)
            {
            case DSIZEX:
                return m_dsizex;
            case DSIZEY:
                return m_dsizey;
            case FX:
                return m_fx;
            case FY:
                return m_fy;
            case INTERPOLATION:
                return m_interpolation;
            case DATA_FLOW:
                return m_dataFlow;
            default:
                throw runtime::WrongParameterId(id, *this);
            }
        }
        
        void Resize::setParameter(unsigned int id, const runtime::Data& value)
        {
            try
            {
                switch(id)
                {
                case DSIZEX:
                    {
                        const runtime::UInt32 & castedValue = runtime::data_cast<runtime::UInt32>(value);
                        if(! castedValue.variant().isVariant(runtime::Variant::UINT_32))
                        {
                            throw runtime::WrongParameterType(parameter(id), *this);
                        }
                        cvsupport::checkNumericValue(castedValue, m_dsizexParameter, *this);
                        m_dsizex = castedValue;
                    }
                    break;
                case DSIZEY:
                    {
                        const runtime::UInt32 & castedValue = runtime::data_cast<runtime::UInt32>(value);
                        if(! castedValue.variant().isVariant(runtime::Variant::UINT_32))
                        {
                            throw runtime::WrongParameterType(parameter(id), *this);
                        }
                        cvsupport::checkNumericValue(castedValue, m_dsizeyParameter, *this);
                        m_dsizey = castedValue;
                    }
                    break;
                case FX:
                    {
                        const runtime::Float64 & castedValue = runtime::data_cast<runtime::Float64>(value);
                        if(! castedValue.variant().isVariant(runtime::Variant::FLOAT_64))
                        {
                            throw runtime::WrongParameterType(parameter(id), *this);
                        }
                        cvsupport::checkNumericValue(castedValue, m_fxParameter, *this);
                        m_fx = castedValue;
                    }
                    break;
                case FY:
                    {
                        const runtime::Float64 & castedValue = runtime::data_cast<runtime::Float64>(value);
                        if(! castedValue.variant().isVariant(runtime::Variant::FLOAT_64))
                        {
                            throw runtime::WrongParameterType(parameter(id), *this);
                        }
                        cvsupport::checkNumericValue(castedValue, m_fyParameter, *this);
                        m_fy = castedValue;
                    }
                    break;
                case INTERPOLATION:
                    {
                        const runtime::Enum & castedValue = runtime::data_cast<runtime::Enum>(value);
                        if(! castedValue.variant().isVariant(runtime::Variant::ENUM))
                        {
                            throw runtime::WrongParameterType(parameter(id), *this);
                        }
                        cvsupport::checkEnumValue(castedValue, m_interpolationParameter, *this);
                        m_interpolation = castedValue;
                    }
                    break;
                case DATA_FLOW:
                    {
                        const runtime::Enum & castedValue = runtime::data_cast<runtime::Enum>(value);
                        if(! castedValue.variant().isVariant(runtime::Variant::ENUM))
                        {
                            throw runtime::WrongParameterType(parameter(id), *this);
                        }
                        cvsupport::checkEnumValue(castedValue, m_dataFlowParameter, *this);
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
        
        const std::vector<const runtime::Parameter*> Resize::setupInitParameters()
        {
            std::vector<const runtime::Parameter*> parameters;
            
            m_dataFlowParameter = new runtime::EnumParameter(DATA_FLOW);
            m_dataFlowParameter->setAccessMode(runtime::Parameter::NONE_WRITE);
            m_dataFlowParameter->setTitle(L_("Data flow"));
            m_dataFlowParameter->add(runtime::EnumDescription(runtime::Enum(MANUAL), L_("Manual")));
            m_dataFlowParameter->add(runtime::EnumDescription(runtime::Enum(ALLOCATE), L_("Allocate")));
            parameters.push_back(m_dataFlowParameter);
            
            return parameters;
        }
        
        const std::vector<const runtime::Parameter*> Resize::setupParameters()
        {
            std::vector<const runtime::Parameter*> parameters;
            
            switch(int(m_dataFlow))
            {
            case(MANUAL):
                {
                    m_dsizexParameter = new runtime::NumericParameter<runtime::UInt32>(DSIZEX);
                    m_dsizexParameter->setAccessMode(runtime::Parameter::ACTIVATED_WRITE);
                    m_dsizexParameter->setTitle(L_("Size X"));
                    parameters.push_back(m_dsizexParameter);
                    
                    m_dsizeyParameter = new runtime::NumericParameter<runtime::UInt32>(DSIZEY);
                    m_dsizeyParameter->setAccessMode(runtime::Parameter::ACTIVATED_WRITE);
                    m_dsizeyParameter->setTitle(L_("Size Y"));
                    parameters.push_back(m_dsizeyParameter);
                    
                    m_fxParameter = new runtime::NumericParameter<runtime::Float64>(FX);
                    m_fxParameter->setAccessMode(runtime::Parameter::ACTIVATED_WRITE);
                    m_fxParameter->setTitle(L_("Scale X"));
                    parameters.push_back(m_fxParameter);
                    
                    m_fyParameter = new runtime::NumericParameter<runtime::Float64>(FY);
                    m_fyParameter->setAccessMode(runtime::Parameter::ACTIVATED_WRITE);
                    m_fyParameter->setTitle(L_("Scale Y"));
                    parameters.push_back(m_fyParameter);
                    
                    m_interpolationParameter = new runtime::EnumParameter(INTERPOLATION);
                    m_interpolationParameter->setAccessMode(runtime::Parameter::ACTIVATED_WRITE);
                    m_interpolationParameter->setTitle(L_("Interpolation"));
                    m_interpolationParameter->add(runtime::EnumDescription(runtime::Enum(INTER_NEAREST), L_("Nearest neighbour")));
                    m_interpolationParameter->add(runtime::EnumDescription(runtime::Enum(INTER_LINEAR), L_("Bilinear")));
                    parameters.push_back(m_interpolationParameter);
                    
                }
                break;
            case(ALLOCATE):
                {
                    m_dsizexParameter = new runtime::NumericParameter<runtime::UInt32>(DSIZEX);
                    m_dsizexParameter->setAccessMode(runtime::Parameter::ACTIVATED_WRITE);
                    m_dsizexParameter->setTitle(L_("Size X"));
                    parameters.push_back(m_dsizexParameter);
                    
                    m_dsizeyParameter = new runtime::NumericParameter<runtime::UInt32>(DSIZEY);
                    m_dsizeyParameter->setAccessMode(runtime::Parameter::ACTIVATED_WRITE);
                    m_dsizeyParameter->setTitle(L_("Size Y"));
                    parameters.push_back(m_dsizeyParameter);
                    
                    m_fxParameter = new runtime::NumericParameter<runtime::Float64>(FX);
                    m_fxParameter->setAccessMode(runtime::Parameter::ACTIVATED_WRITE);
                    m_fxParameter->setTitle(L_("Scale X"));
                    parameters.push_back(m_fxParameter);
                    
                    m_fyParameter = new runtime::NumericParameter<runtime::Float64>(FY);
                    m_fyParameter->setAccessMode(runtime::Parameter::ACTIVATED_WRITE);
                    m_fyParameter->setTitle(L_("Scale Y"));
                    parameters.push_back(m_fyParameter);
                    
                    m_interpolationParameter = new runtime::EnumParameter(INTERPOLATION);
                    m_interpolationParameter->setAccessMode(runtime::Parameter::ACTIVATED_WRITE);
                    m_interpolationParameter->setTitle(L_("Interpolation"));
                    m_interpolationParameter->add(runtime::EnumDescription(runtime::Enum(INTER_NEAREST), L_("Nearest neighbour")));
                    m_interpolationParameter->add(runtime::EnumDescription(runtime::Enum(INTER_LINEAR), L_("Bilinear")));
                    parameters.push_back(m_interpolationParameter);
                    
                }
                break;
            }
            
            return parameters;
        }
        
        const std::vector<const runtime::Description*> Resize::setupInputs()
        {
            std::vector<const runtime::Description*> inputs;
            
            switch(int(m_dataFlow))
            {
            case(MANUAL):
                {
                    m_srcDescription = new runtime::Description(SRC, runtime::Variant::IMAGE);
                    m_srcDescription->setTitle(L_("Source"));
                    inputs.push_back(m_srcDescription);
                    
                    m_dstDescription = new runtime::Description(DST, runtime::Variant::IMAGE);
                    m_dstDescription->setTitle(L_("Destination"));
                    inputs.push_back(m_dstDescription);
                    
                }
                break;
            case(ALLOCATE):
                {
                    m_srcDescription = new runtime::Description(SRC, runtime::Variant::IMAGE);
                    m_srcDescription->setTitle(L_("Source"));
                    inputs.push_back(m_srcDescription);
                    
                }
                break;
            }
            
            return inputs;
        }
        
        const std::vector<const runtime::Description*> Resize::setupOutputs()
        {
            std::vector<const runtime::Description*> outputs;
            
            switch(int(m_dataFlow))
            {
            case(MANUAL):
                {
                    runtime::Description* dst = new runtime::Description(DST, runtime::Variant::IMAGE);
                    dst->setTitle(L_("Destination"));
                    outputs.push_back(dst);
                    
                }
                break;
            case(ALLOCATE):
                {
                    runtime::Description* dst = new runtime::Description(DST, runtime::Variant::IMAGE);
                    dst->setTitle(L_("Destination"));
                    outputs.push_back(dst);
                    
                }
                break;
            }
            
            return outputs;
        }
        
        void Resize::initialize()
        {
            runtime::OperatorKernel::initialize(setupInputs(), setupOutputs(), setupParameters());
        }
        
        void Resize::execute(runtime::DataProvider & provider)
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
                    
                    if(! srcData->variant().isVariant(m_srcDescription->variant()))
                    {
                        throw runtime::InputError(SRC, *this, "Wrong input data variant.");
                    }
                    if(! dstData->variant().isVariant(m_dstDescription->variant()))
                    {
                        throw runtime::InputError(DST, *this, "Wrong input data variant.");
                    }
                    
                    const runtime::Image* srcCastedData = runtime::data_cast<runtime::Image>(srcData);
                    runtime::Image * dstCastedData = runtime::data_cast<runtime::Image>(dstData);
                    
                    int width = int(m_dsizex) ? int(m_dsizex) : int(srcCastedData->width() * double(m_fx));
                    int height = int(m_dsizey) ? int(m_dsizey) : int(srcCastedData->height() * double(m_fy));
                    dstCastedData->initializeImage(width, height, width * srcCastedData->pixelSize(), dstCastedData->data(), srcCastedData->pixelType());
                    
                    cv::Mat srcCvData = cvsupport::getOpenCvMat(*srcCastedData);
                    cv::Mat dstCvData = cvsupport::getOpenCvMat(*dstCastedData);
                    int dsizexCvData = int(m_dsizex);
                    int dsizeyCvData = int(m_dsizey);
                    double fxCvData = double(m_fx);
                    double fyCvData = double(m_fy);
                    int interpolationCvData = convertInterpolation(m_interpolation);
                    
                    cv::resize(srcCvData, dstCvData, cv::Size(dsizexCvData, dsizeyCvData), fxCvData, fyCvData, interpolationCvData);
                    
                    runtime::DataContainer dstOutContainer = inContainer;
                    runtime::Id2DataPair dstOutMapper(DST, dstOutContainer);
                    
                    provider.sendOutputData(dstOutMapper);
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
                    
                    if(! srcData->variant().isVariant(m_srcDescription->variant()))
                    {
                        throw runtime::InputError(SRC, *this, "Wrong input data variant.");
                    }
                    
                    const runtime::Image* srcCastedData = runtime::data_cast<runtime::Image>(srcData);
                    
                    cv::Mat srcCvData = cvsupport::getOpenCvMat(*srcCastedData);
                    cv::Mat dstCvData;
                    int dsizexCvData = int(m_dsizex);
                    int dsizeyCvData = int(m_dsizey);
                    double fxCvData = double(m_fx);
                    double fyCvData = double(m_fy);
                    int interpolationCvData = convertInterpolation(m_interpolation);
                    
                    cv::resize(srcCvData, dstCvData, cv::Size(dsizexCvData, dsizeyCvData), fxCvData, fyCvData, interpolationCvData);
                    
                    runtime::Image* dstCastedData = new cvsupport::Image(dstCvData);
                    runtime::DataContainer dstOutContainer = runtime::DataContainer(dstCastedData);
                    runtime::Id2DataPair dstOutMapper(DST, dstOutContainer);
                    
                    dstCastedData->initializeImage(dstCastedData->width(), dstCastedData->height(), dstCastedData->stride(), dstCastedData->data(), srcCastedData->pixelType());
                    provider.sendOutputData(dstOutMapper);
                }
                break;
            }
        }
        
        int Resize::convertInterpolation(const runtime::Enum & value)
        {
            switch(int(value))
            {
            case INTER_NEAREST:
                return cv::INTER_NEAREST;
            case INTER_LINEAR:
                return cv::INTER_LINEAR;
            default:
                throw runtime::WrongParameterValue(parameter(INTERPOLATION), *this);
            }
        }
        
    } // cvimgproc
} // stromx

