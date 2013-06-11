#include "stromx/imgproc/Resize.h"

#include "stromx/imgproc/Utility.h"
#include <stromx/imgutil/Image.h>
#include <stromx/imgutil/Matrix.h>
#include <stromx/imgutil/Utilities.h>
#include <stromx/runtime/DataContainer.h>
#include <stromx/runtime/DataProvider.h>
#include <stromx/runtime/Id2DataComposite.h>
#include <stromx/runtime/Id2DataPair.h>
#include <stromx/runtime/ReadAccess.h>
#include <stromx/runtime/WriteAccess.h>
#include <opencv2/imgproc/imgproc.hpp>

namespace stromx
{
    namespace imgproc
    {
        const std::string Resize::PACKAGE(STROMX_IMGPROC_PACKAGE_NAME);
        const runtime::Version Resize::VERSION(STROMX_IMGPROC_VERSION_MAJOR, STROMX_IMGPROC_VERSION_MINOR, STROMX_IMGPROC_VERSION_PATCH);
        const std::string Resize::TYPE("Resize");
        
        Resize::Resize()
          : runtime::OperatorKernel(TYPE, PACKAGE, VERSION, setupInitParameters()),
            m_fy(1.0),
            m_dsizex(),
            m_fx(1.0),
            m_interpolation(1),
            m_dsizey(),
            m_dataFlow()
        {
        }
        
        const runtime::DataRef Resize::getParameter(unsigned int id) const
        {
            switch(id)
            {
            case FY:
                return m_fy;
            case DSIZEX:
                return m_dsizex;
            case FX:
                return m_fx;
            case INTERPOLATION:
                return m_interpolation;
            case DSIZEY:
                return m_dsizey;
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
                case FY:
                    {
                        runtime::Double castedValue = runtime::data_cast<runtime::Double>(value);
                        checkNumericValue(castedValue, m_fyParameter, *this);
                        m_fy = castedValue;
                    }
                    break;
                case DSIZEX:
                    {
                        runtime::UInt32 castedValue = runtime::data_cast<runtime::UInt32>(value);
                        checkNumericValue(castedValue, m_dsizexParameter, *this);
                        m_dsizex = castedValue;
                    }
                    break;
                case FX:
                    {
                        runtime::Double castedValue = runtime::data_cast<runtime::Double>(value);
                        checkNumericValue(castedValue, m_fxParameter, *this);
                        m_fx = castedValue;
                    }
                    break;
                case INTERPOLATION:
                    {
                        runtime::Enum castedValue = runtime::data_cast<runtime::Enum>(value);
                        checkEnumValue(castedValue, m_interpolationParameter, *this);
                        m_interpolation = castedValue;
                    }
                    break;
                case DSIZEY:
                    {
                        runtime::UInt32 castedValue = runtime::data_cast<runtime::UInt32>(value);
                        checkNumericValue(castedValue, m_dsizeyParameter, *this);
                        m_dsizey = castedValue;
                    }
                    break;
                case DATA_FLOW:
                    {
                        runtime::Enum castedValue = runtime::data_cast<runtime::Enum>(value);
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
        
        const std::vector<const runtime::Parameter*> Resize::setupInitParameters()
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
        
        const std::vector<const runtime::Parameter*> Resize::setupParameters()
        {
            std::vector<const runtime::Parameter*> parameters;
            
            switch(int(m_dataFlow))
            {
            case(MANUAL):
                {
                    m_dsizexParameter = new runtime::NumericParameter<runtime::UInt32>(DSIZEX);
                    m_dsizexParameter->setAccessMode(runtime::Parameter::ACTIVATED_WRITE);
                    m_dsizexParameter->setTitle("Size X");
                    parameters.push_back(m_dsizexParameter);
                    
                    m_dsizeyParameter = new runtime::NumericParameter<runtime::UInt32>(DSIZEY);
                    m_dsizeyParameter->setAccessMode(runtime::Parameter::ACTIVATED_WRITE);
                    m_dsizeyParameter->setTitle("Size Y");
                    parameters.push_back(m_dsizeyParameter);
                    
                    m_fxParameter = new runtime::NumericParameter<runtime::Double>(FX);
                    m_fxParameter->setAccessMode(runtime::Parameter::ACTIVATED_WRITE);
                    m_fxParameter->setTitle("Scale X");
                    parameters.push_back(m_fxParameter);
                    
                    m_fyParameter = new runtime::NumericParameter<runtime::Double>(FY);
                    m_fyParameter->setAccessMode(runtime::Parameter::ACTIVATED_WRITE);
                    m_fyParameter->setTitle("Scale Y");
                    parameters.push_back(m_fyParameter);
                    
                    m_interpolationParameter = new runtime::EnumParameter(INTERPOLATION);
                    m_interpolationParameter->setAccessMode(runtime::Parameter::ACTIVATED_WRITE);
                    m_interpolationParameter->setTitle("Interpolation");
                    m_interpolationParameter->add(runtime::EnumDescription(runtime::Enum(INTER_NEAREST), "Nearest neighbour"));
                    m_interpolationParameter->add(runtime::EnumDescription(runtime::Enum(INTER_LINEAR), "Bilinear"));
                    parameters.push_back(m_interpolationParameter);
                    
                }
                break;
            case(ALLOCATE):
                {
                    m_dsizexParameter = new runtime::NumericParameter<runtime::UInt32>(DSIZEX);
                    m_dsizexParameter->setAccessMode(runtime::Parameter::ACTIVATED_WRITE);
                    m_dsizexParameter->setTitle("Size X");
                    parameters.push_back(m_dsizexParameter);
                    
                    m_dsizeyParameter = new runtime::NumericParameter<runtime::UInt32>(DSIZEY);
                    m_dsizeyParameter->setAccessMode(runtime::Parameter::ACTIVATED_WRITE);
                    m_dsizeyParameter->setTitle("Size Y");
                    parameters.push_back(m_dsizeyParameter);
                    
                    m_fxParameter = new runtime::NumericParameter<runtime::Double>(FX);
                    m_fxParameter->setAccessMode(runtime::Parameter::ACTIVATED_WRITE);
                    m_fxParameter->setTitle("Scale X");
                    parameters.push_back(m_fxParameter);
                    
                    m_fyParameter = new runtime::NumericParameter<runtime::Double>(FY);
                    m_fyParameter->setAccessMode(runtime::Parameter::ACTIVATED_WRITE);
                    m_fyParameter->setTitle("Scale Y");
                    parameters.push_back(m_fyParameter);
                    
                    m_interpolationParameter = new runtime::EnumParameter(INTERPOLATION);
                    m_interpolationParameter->setAccessMode(runtime::Parameter::ACTIVATED_WRITE);
                    m_interpolationParameter->setTitle("Interpolation");
                    m_interpolationParameter->add(runtime::EnumDescription(runtime::Enum(INTER_NEAREST), "Nearest neighbour"));
                    m_interpolationParameter->add(runtime::EnumDescription(runtime::Enum(INTER_LINEAR), "Bilinear"));
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
        
        const std::vector<const runtime::Description*> Resize::setupOutputs()
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
                    
                    const runtime::Image* srcCastedData = runtime::data_cast<runtime::Image>(srcData);
                    runtime::Image * dstCastedData = runtime::data_cast<runtime::Image>(dstData);
                    
                    int width = int(m_dsizex) ? int(m_dsizex) : srcCastedData->width() * double(m_fx);
                    int height = int(m_dsizey) ? int(m_dsizey) : srcCastedData->height() * double(m_fy);
                    dstCastedData->initializeImage(width, height, width * srcCastedData->pixelSize(), dstCastedData->data(), srcCastedData->pixelType());
                    
                    cv::Mat srcCvData = imgutil::getOpenCvMat(*srcCastedData);
                    cv::Mat dstCvData = imgutil::getOpenCvMat(*dstCastedData);
                    int dsizexCvData = int(m_dsizex);
                    int dsizeyCvData = int(m_dsizey);
                    double fxCvData = double(m_fx);
                    double fyCvData = double(m_fy);
                    int interpolationCvData = convertInterpolation(m_interpolation);
                    
                    cv::resize(srcCvData, dstCvData, cv::Size(dsizexCvData, dsizeyCvData), fxCvData, fyCvData, interpolationCvData);
                    
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
                    
                    const runtime::Image* srcCastedData = runtime::data_cast<runtime::Image>(srcData);
                    
                    cv::Mat srcCvData = imgutil::getOpenCvMat(*srcCastedData);
                    cv::Mat dstCvData;
                    int dsizexCvData = int(m_dsizex);
                    int dsizeyCvData = int(m_dsizey);
                    double fxCvData = double(m_fx);
                    double fyCvData = double(m_fy);
                    int interpolationCvData = convertInterpolation(m_interpolation);
                    
                    cv::resize(srcCvData, dstCvData, cv::Size(dsizexCvData, dsizeyCvData), fxCvData, fyCvData, interpolationCvData);
                    
                    runtime::Image* dstCastedData = new imgutil::Image(dstCvData);
                    runtime::DataContainer outContainer = runtime::DataContainer(dstCastedData);
                    runtime::Id2DataPair outputMapper(DST, outContainer);
                    
                    dstCastedData->initializeImage(dstCastedData->width(), dstCastedData->height(), dstCastedData->stride(), dstCastedData->data(), srcCastedData->pixelType());
                    provider.sendOutputData(outputMapper);
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
        
    }
}

