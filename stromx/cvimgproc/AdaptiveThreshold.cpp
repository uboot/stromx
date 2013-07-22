#include "stromx/cvimgproc/AdaptiveThreshold.h"

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
        const std::string AdaptiveThreshold::PACKAGE(STROMX_CVIMGPROC_PACKAGE_NAME);
        const runtime::Version AdaptiveThreshold::VERSION(STROMX_CVIMGPROC_VERSION_MAJOR, STROMX_CVIMGPROC_VERSION_MINOR, STROMX_CVIMGPROC_VERSION_PATCH);
        const std::string AdaptiveThreshold::TYPE("AdaptiveThreshold");
        
        AdaptiveThreshold::AdaptiveThreshold()
          : runtime::OperatorKernel(TYPE, PACKAGE, VERSION, setupInitParameters()),
            m_adaptiveMethod(0),
            m_blockSize(3),
            m_maxval(255.0),
            m_thresholdType(0),
            m_dataFlow()
        {
        }
        
        const runtime::DataRef AdaptiveThreshold::getParameter(unsigned int id) const
        {
            switch(id)
            {
            case ADAPTIVE_METHOD:
                return m_adaptiveMethod;
            case BLOCK_SIZE:
                return m_blockSize;
            case MAXVAL:
                return m_maxval;
            case THRESHOLD_TYPE:
                return m_thresholdType;
            case DATA_FLOW:
                return m_dataFlow;
            default:
                throw runtime::WrongParameterId(id, *this);
            }
        }
        
        void AdaptiveThreshold::setParameter(unsigned int id, const runtime::Data& value)
        {
            try
            {
                switch(id)
                {
                case ADAPTIVE_METHOD:
                    {
                        const runtime::Enum & castedValue = runtime::data_cast<runtime::Enum>(value);
                        if(! castedValue.variant().isVariant(runtime::DataVariant::ENUM))
                        {
                            throw runtime::WrongParameterType(parameter(id), *this);
                        }
                        checkEnumValue(castedValue, m_adaptiveMethodParameter, *this);
                        m_adaptiveMethod = castedValue;
                    }
                    break;
                case BLOCK_SIZE:
                    {
                        const runtime::UInt32 & castedValue = runtime::data_cast<runtime::UInt32>(value);
                        if(! castedValue.variant().isVariant(runtime::DataVariant::UINT_32))
                        {
                            throw runtime::WrongParameterType(parameter(id), *this);
                        }
                        checkNumericValue(castedValue, m_blockSizeParameter, *this);
                        if(int(castedValue) % 2 == 0)
                            throw runtime::WrongParameterValue(*m_blockSizeParameter, *this, "Only odd values are allowed");
                        m_blockSize = castedValue;
                    }
                    break;
                case MAXVAL:
                    {
                        const runtime::Float64 & castedValue = runtime::data_cast<runtime::Float64>(value);
                        if(! castedValue.variant().isVariant(runtime::DataVariant::FLOAT_64))
                        {
                            throw runtime::WrongParameterType(parameter(id), *this);
                        }
                        checkNumericValue(castedValue, m_maxvalParameter, *this);
                        m_maxval = castedValue;
                    }
                    break;
                case THRESHOLD_TYPE:
                    {
                        const runtime::Enum & castedValue = runtime::data_cast<runtime::Enum>(value);
                        if(! castedValue.variant().isVariant(runtime::DataVariant::ENUM))
                        {
                            throw runtime::WrongParameterType(parameter(id), *this);
                        }
                        checkEnumValue(castedValue, m_thresholdTypeParameter, *this);
                        m_thresholdType = castedValue;
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
        
        const std::vector<const runtime::Parameter*> AdaptiveThreshold::setupInitParameters()
        {
            std::vector<const runtime::Parameter*> parameters;
            
            m_dataFlowParameter = new runtime::EnumParameter(DATA_FLOW);
            m_dataFlowParameter->setAccessMode(runtime::Parameter::NONE_WRITE);
            m_dataFlowParameter->setTitle("Data flow");
            m_dataFlowParameter->add(runtime::EnumDescription(runtime::Enum(MANUAL), "Manual"));
            m_dataFlowParameter->add(runtime::EnumDescription(runtime::Enum(ALLOCATE), "Allocate"));
            m_dataFlowParameter->add(runtime::EnumDescription(runtime::Enum(IN_PLACE), "In place"));
            parameters.push_back(m_dataFlowParameter);
            
            return parameters;
        }
        
        const std::vector<const runtime::Parameter*> AdaptiveThreshold::setupParameters()
        {
            std::vector<const runtime::Parameter*> parameters;
            
            switch(int(m_dataFlow))
            {
            case(MANUAL):
                {
                    m_maxvalParameter = new runtime::NumericParameter<runtime::Float64>(MAXVAL);
                    m_maxvalParameter->setAccessMode(runtime::Parameter::ACTIVATED_WRITE);
                    m_maxvalParameter->setTitle("Maximal value");
                    parameters.push_back(m_maxvalParameter);
                    
                    m_adaptiveMethodParameter = new runtime::EnumParameter(ADAPTIVE_METHOD);
                    m_adaptiveMethodParameter->setAccessMode(runtime::Parameter::ACTIVATED_WRITE);
                    m_adaptiveMethodParameter->setTitle("Adaptive method");
                    m_adaptiveMethodParameter->add(runtime::EnumDescription(runtime::Enum(ADAPTIVE_THRESH_MEAN_C), "Mean of block"));
                    m_adaptiveMethodParameter->add(runtime::EnumDescription(runtime::Enum( ADAPTIVE_THRESH_GAUSSIAN_C), "Weighted sum of block"));
                    parameters.push_back(m_adaptiveMethodParameter);
                    
                    m_thresholdTypeParameter = new runtime::EnumParameter(THRESHOLD_TYPE);
                    m_thresholdTypeParameter->setAccessMode(runtime::Parameter::ACTIVATED_WRITE);
                    m_thresholdTypeParameter->setTitle("Threshold type");
                    m_thresholdTypeParameter->add(runtime::EnumDescription(runtime::Enum(THRESH_BINARY), "Binary"));
                    m_thresholdTypeParameter->add(runtime::EnumDescription(runtime::Enum(THRESH_BINARY_INV), "Binary inverted"));
                    m_thresholdTypeParameter->add(runtime::EnumDescription(runtime::Enum(THRESH_TRUNC), "Truncate"));
                    m_thresholdTypeParameter->add(runtime::EnumDescription(runtime::Enum(THRESH_TOZERO), "Truncate to zero"));
                    m_thresholdTypeParameter->add(runtime::EnumDescription(runtime::Enum(THRESH_TOZERO_INV), "Truncate to zero inverted"));
                    parameters.push_back(m_thresholdTypeParameter);
                    
                    m_blockSizeParameter = new runtime::NumericParameter<runtime::UInt32>(BLOCK_SIZE);
                    m_blockSizeParameter->setAccessMode(runtime::Parameter::ACTIVATED_WRITE);
                    m_blockSizeParameter->setTitle("Block size");
                    m_blockSizeParameter->setMin(runtime::UInt32(1));
                    parameters.push_back(m_blockSizeParameter);
                    
                }
                break;
            case(ALLOCATE):
                {
                    m_maxvalParameter = new runtime::NumericParameter<runtime::Float64>(MAXVAL);
                    m_maxvalParameter->setAccessMode(runtime::Parameter::ACTIVATED_WRITE);
                    m_maxvalParameter->setTitle("Maximal value");
                    parameters.push_back(m_maxvalParameter);
                    
                    m_adaptiveMethodParameter = new runtime::EnumParameter(ADAPTIVE_METHOD);
                    m_adaptiveMethodParameter->setAccessMode(runtime::Parameter::ACTIVATED_WRITE);
                    m_adaptiveMethodParameter->setTitle("Adaptive method");
                    m_adaptiveMethodParameter->add(runtime::EnumDescription(runtime::Enum(ADAPTIVE_THRESH_MEAN_C), "Mean of block"));
                    m_adaptiveMethodParameter->add(runtime::EnumDescription(runtime::Enum( ADAPTIVE_THRESH_GAUSSIAN_C), "Weighted sum of block"));
                    parameters.push_back(m_adaptiveMethodParameter);
                    
                    m_thresholdTypeParameter = new runtime::EnumParameter(THRESHOLD_TYPE);
                    m_thresholdTypeParameter->setAccessMode(runtime::Parameter::ACTIVATED_WRITE);
                    m_thresholdTypeParameter->setTitle("Threshold type");
                    m_thresholdTypeParameter->add(runtime::EnumDescription(runtime::Enum(THRESH_BINARY), "Binary"));
                    m_thresholdTypeParameter->add(runtime::EnumDescription(runtime::Enum(THRESH_BINARY_INV), "Binary inverted"));
                    m_thresholdTypeParameter->add(runtime::EnumDescription(runtime::Enum(THRESH_TRUNC), "Truncate"));
                    m_thresholdTypeParameter->add(runtime::EnumDescription(runtime::Enum(THRESH_TOZERO), "Truncate to zero"));
                    m_thresholdTypeParameter->add(runtime::EnumDescription(runtime::Enum(THRESH_TOZERO_INV), "Truncate to zero inverted"));
                    parameters.push_back(m_thresholdTypeParameter);
                    
                    m_blockSizeParameter = new runtime::NumericParameter<runtime::UInt32>(BLOCK_SIZE);
                    m_blockSizeParameter->setAccessMode(runtime::Parameter::ACTIVATED_WRITE);
                    m_blockSizeParameter->setTitle("Block size");
                    m_blockSizeParameter->setMin(runtime::UInt32(1));
                    parameters.push_back(m_blockSizeParameter);
                    
                }
                break;
            case(IN_PLACE):
                {
                    m_maxvalParameter = new runtime::NumericParameter<runtime::Float64>(MAXVAL);
                    m_maxvalParameter->setAccessMode(runtime::Parameter::ACTIVATED_WRITE);
                    m_maxvalParameter->setTitle("Maximal value");
                    parameters.push_back(m_maxvalParameter);
                    
                    m_adaptiveMethodParameter = new runtime::EnumParameter(ADAPTIVE_METHOD);
                    m_adaptiveMethodParameter->setAccessMode(runtime::Parameter::ACTIVATED_WRITE);
                    m_adaptiveMethodParameter->setTitle("Adaptive method");
                    m_adaptiveMethodParameter->add(runtime::EnumDescription(runtime::Enum(ADAPTIVE_THRESH_MEAN_C), "Mean of block"));
                    m_adaptiveMethodParameter->add(runtime::EnumDescription(runtime::Enum( ADAPTIVE_THRESH_GAUSSIAN_C), "Weighted sum of block"));
                    parameters.push_back(m_adaptiveMethodParameter);
                    
                    m_thresholdTypeParameter = new runtime::EnumParameter(THRESHOLD_TYPE);
                    m_thresholdTypeParameter->setAccessMode(runtime::Parameter::ACTIVATED_WRITE);
                    m_thresholdTypeParameter->setTitle("Threshold type");
                    m_thresholdTypeParameter->add(runtime::EnumDescription(runtime::Enum(THRESH_BINARY), "Binary"));
                    m_thresholdTypeParameter->add(runtime::EnumDescription(runtime::Enum(THRESH_BINARY_INV), "Binary inverted"));
                    m_thresholdTypeParameter->add(runtime::EnumDescription(runtime::Enum(THRESH_TRUNC), "Truncate"));
                    m_thresholdTypeParameter->add(runtime::EnumDescription(runtime::Enum(THRESH_TOZERO), "Truncate to zero"));
                    m_thresholdTypeParameter->add(runtime::EnumDescription(runtime::Enum(THRESH_TOZERO_INV), "Truncate to zero inverted"));
                    parameters.push_back(m_thresholdTypeParameter);
                    
                    m_blockSizeParameter = new runtime::NumericParameter<runtime::UInt32>(BLOCK_SIZE);
                    m_blockSizeParameter->setAccessMode(runtime::Parameter::ACTIVATED_WRITE);
                    m_blockSizeParameter->setTitle("Block size");
                    m_blockSizeParameter->setMin(runtime::UInt32(1));
                    parameters.push_back(m_blockSizeParameter);
                    
                }
                break;
            }
            
            return parameters;
        }
        
        const std::vector<const runtime::Description*> AdaptiveThreshold::setupInputs()
        {
            std::vector<const runtime::Description*> inputs;
            
            switch(int(m_dataFlow))
            {
            case(MANUAL):
                {
                    runtime::Description* src = new runtime::Description(SRC, runtime::DataVariant::MONO_8_IMAGE);
                    src->setTitle("Source");
                    inputs.push_back(src);
                    
                    runtime::Description* dst = new runtime::Description(DST, runtime::DataVariant::IMAGE);
                    dst->setTitle("Destination");
                    inputs.push_back(dst);
                    
                }
                break;
            case(ALLOCATE):
                {
                    runtime::Description* src = new runtime::Description(SRC, runtime::DataVariant::MONO_8_IMAGE);
                    src->setTitle("Source");
                    inputs.push_back(src);
                    
                }
                break;
            case(IN_PLACE):
                {
                    runtime::Description* src = new runtime::Description(SRC, runtime::DataVariant::MONO_8_IMAGE);
                    src->setTitle("Source");
                    inputs.push_back(src);
                    
                }
                break;
            }
            
            return inputs;
        }
        
        const std::vector<const runtime::Description*> AdaptiveThreshold::setupOutputs()
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
            case(IN_PLACE):
                {
                    runtime::Description* src = new runtime::Description(SRC, runtime::DataVariant::MONO_8_IMAGE);
                    src->setTitle("Source");
                    outputs.push_back(src);
                    
                }
                break;
            }
            
            return outputs;
        }
        
        void AdaptiveThreshold::initialize()
        {
            runtime::OperatorKernel::initialize(setupInputs(), setupOutputs(), setupParameters());
        }
        
        void AdaptiveThreshold::execute(runtime::DataProvider & provider)
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
                        srcData = &writeAccess();
                    }
                    else
                    {
                        srcReadAccess = runtime::ReadAccess<>(srcInMapper.data());
                        srcData = &srcReadAccess();
                    }
                    
                    if(! srcData->variant().isVariant(runtime::DataVariant::MONO_8_IMAGE))
                    {
                        throw runtime::InputError(SRC, *this, "Wrong input data variant.");
                    }
                    if(! dstData->variant().isVariant(runtime::DataVariant::IMAGE))
                    {
                        throw runtime::InputError(DST, *this, "Wrong input data variant.");
                    }
                    
                    const runtime::Image* srcCastedData = runtime::data_cast<runtime::Image>(srcData);
                    runtime::Image * dstCastedData = runtime::data_cast<runtime::Image>(dstData);
                    
                    dstCastedData->initializeImage(srcCastedData->width(), srcCastedData->height(), srcCastedData->stride(), dstCastedData->data(), srcCastedData->pixelType());
                    
                    cv::Mat srcCvData = cvsupport::getOpenCvMat(*srcCastedData);
                    cv::Mat dstCvData = cvsupport::getOpenCvMat(*dstCastedData);
                    double maxvalCvData = double(m_maxval);
                    int adaptiveMethodCvData = convertAdaptiveMethod(m_adaptiveMethod);
                    int thresholdTypeCvData = convertThresholdType(m_thresholdType);
                    int blockSizeCvData = int(m_blockSize);
                    
                    cv::adaptiveThreshold(srcCvData, dstCvData, maxvalCvData, adaptiveMethodCvData, thresholdTypeCvData, blockSizeCvData, 0);
                    
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
                    
                    if(! srcData->variant().isVariant(runtime::DataVariant::MONO_8_IMAGE))
                    {
                        throw runtime::InputError(SRC, *this, "Wrong input data variant.");
                    }
                    
                    const runtime::Image* srcCastedData = runtime::data_cast<runtime::Image>(srcData);
                    
                    cv::Mat srcCvData = cvsupport::getOpenCvMat(*srcCastedData);
                    cv::Mat dstCvData;
                    double maxvalCvData = double(m_maxval);
                    int adaptiveMethodCvData = convertAdaptiveMethod(m_adaptiveMethod);
                    int thresholdTypeCvData = convertThresholdType(m_thresholdType);
                    int blockSizeCvData = int(m_blockSize);
                    
                    cv::adaptiveThreshold(srcCvData, dstCvData, maxvalCvData, adaptiveMethodCvData, thresholdTypeCvData, blockSizeCvData, 0);
                    
                    runtime::Image* dstCastedData = new cvsupport::Image(dstCvData);
                    runtime::DataContainer outContainer = runtime::DataContainer(dstCastedData);
                    runtime::Id2DataPair outputMapper(DST, outContainer);
                    
                    dstCastedData->initializeImage(dstCastedData->width(), dstCastedData->height(), dstCastedData->stride(), dstCastedData->data(), srcCastedData->pixelType());
                    provider.sendOutputData(outputMapper);
                }
                break;
            case(IN_PLACE):
                {
                    runtime::Id2DataPair srcInMapper(SRC);
                    
                    provider.receiveInputData(srcInMapper);
                    
                    runtime::Data* srcData = 0;
                    
                    runtime::DataContainer inContainer = srcInMapper.data();
                    runtime::WriteAccess<> writeAccess(inContainer);
                    srcData = &writeAccess();
                    
                    if(! srcData->variant().isVariant(runtime::DataVariant::MONO_8_IMAGE))
                    {
                        throw runtime::InputError(SRC, *this, "Wrong input data variant.");
                    }
                    
                    runtime::Image * srcCastedData = runtime::data_cast<runtime::Image>(srcData);
                    
                    cv::Mat srcCvData = cvsupport::getOpenCvMat(*srcCastedData);
                    cv::Mat dstCvData = srcCvData;
                    double maxvalCvData = double(m_maxval);
                    int adaptiveMethodCvData = convertAdaptiveMethod(m_adaptiveMethod);
                    int thresholdTypeCvData = convertThresholdType(m_thresholdType);
                    int blockSizeCvData = int(m_blockSize);
                    
                    cv::adaptiveThreshold(srcCvData, dstCvData, maxvalCvData, adaptiveMethodCvData, thresholdTypeCvData, blockSizeCvData, 0);
                    
                    runtime::DataContainer outContainer = inContainer;
                    runtime::Id2DataPair outputMapper(SRC, outContainer);
                    
                    provider.sendOutputData(outputMapper);
                }
                break;
            }
        }
        
        int AdaptiveThreshold::convertAdaptiveMethod(const runtime::Enum & value)
        {
            switch(int(value))
            {
            case ADAPTIVE_THRESH_MEAN_C:
                return cv::ADAPTIVE_THRESH_MEAN_C;
            case  ADAPTIVE_THRESH_GAUSSIAN_C:
                return cv:: ADAPTIVE_THRESH_GAUSSIAN_C;
            default:
                throw runtime::WrongParameterValue(parameter(ADAPTIVE_METHOD), *this);
            }
        }
        
        int AdaptiveThreshold::convertThresholdType(const runtime::Enum & value)
        {
            switch(int(value))
            {
            case THRESH_BINARY:
                return cv::THRESH_BINARY;
            case THRESH_BINARY_INV:
                return cv::THRESH_BINARY_INV;
            case THRESH_TRUNC:
                return cv::THRESH_TRUNC;
            case THRESH_TOZERO:
                return cv::THRESH_TOZERO;
            case THRESH_TOZERO_INV:
                return cv::THRESH_TOZERO_INV;
            default:
                throw runtime::WrongParameterValue(parameter(THRESHOLD_TYPE), *this);
            }
        }
        
    }
}

