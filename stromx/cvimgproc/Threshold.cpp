#include "stromx/cvimgproc/Threshold.h"

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
        const std::string Threshold::PACKAGE(STROMX_CVIMGPROC_PACKAGE_NAME);
        const runtime::Version Threshold::VERSION(STROMX_CVIMGPROC_VERSION_MAJOR, STROMX_CVIMGPROC_VERSION_MINOR, STROMX_CVIMGPROC_VERSION_PATCH);
        const std::string Threshold::TYPE("Threshold");
        
        Threshold::Threshold()
          : runtime::OperatorKernel(TYPE, PACKAGE, VERSION, setupInitParameters()),
            m_maxval(255.0),
            m_threshold(127.0),
            m_thresholdType(0),
            m_dataFlow()
        {
        }
        
        const runtime::DataRef Threshold::getParameter(unsigned int id) const
        {
            switch(id)
            {
            case MAXVAL:
                return m_maxval;
            case THRESHOLD:
                return m_threshold;
            case THRESHOLD_TYPE:
                return m_thresholdType;
            case DATA_FLOW:
                return m_dataFlow;
            default:
                throw runtime::WrongParameterId(id, *this);
            }
        }
        
        void Threshold::setParameter(unsigned int id, const runtime::Data& value)
        {
            try
            {
                switch(id)
                {
                case MAXVAL:
                    {
                        const runtime::Double & castedValue = runtime::data_cast<runtime::Double>(value);
                        checkNumericValue(castedValue, m_maxvalParameter, *this);
                        m_maxval = castedValue;
                    }
                    break;
                case THRESHOLD:
                    {
                        const runtime::Double & castedValue = runtime::data_cast<runtime::Double>(value);
                        checkNumericValue(castedValue, m_thresholdParameter, *this);
                        m_threshold = castedValue;
                    }
                    break;
                case THRESHOLD_TYPE:
                    {
                        const runtime::Enum & castedValue = runtime::data_cast<runtime::Enum>(value);
                        checkEnumValue(castedValue, m_thresholdTypeParameter, *this);
                        m_thresholdType = castedValue;
                    }
                    break;
                case DATA_FLOW:
                    {
                        const runtime::Enum & castedValue = runtime::data_cast<runtime::Enum>(value);
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
        
        const std::vector<const runtime::Parameter*> Threshold::setupInitParameters()
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
        
        const std::vector<const runtime::Parameter*> Threshold::setupParameters()
        {
            std::vector<const runtime::Parameter*> parameters;
            
            switch(int(m_dataFlow))
            {
            case(MANUAL):
                {
                    m_thresholdParameter = new runtime::NumericParameter<runtime::Double>(THRESHOLD);
                    m_thresholdParameter->setAccessMode(runtime::Parameter::ACTIVATED_WRITE);
                    m_thresholdParameter->setTitle("Threshold");
                    parameters.push_back(m_thresholdParameter);
                    
                    m_maxvalParameter = new runtime::NumericParameter<runtime::Double>(MAXVAL);
                    m_maxvalParameter->setAccessMode(runtime::Parameter::ACTIVATED_WRITE);
                    m_maxvalParameter->setTitle("Maximal value");
                    parameters.push_back(m_maxvalParameter);
                    
                    m_thresholdTypeParameter = new runtime::EnumParameter(THRESHOLD_TYPE);
                    m_thresholdTypeParameter->setAccessMode(runtime::Parameter::ACTIVATED_WRITE);
                    m_thresholdTypeParameter->setTitle("Threshold type");
                    m_thresholdTypeParameter->add(runtime::EnumDescription(runtime::Enum(THRESH_BINARY), "Binary"));
                    m_thresholdTypeParameter->add(runtime::EnumDescription(runtime::Enum(THRESH_BINARY_INV), "Binary inverted"));
                    m_thresholdTypeParameter->add(runtime::EnumDescription(runtime::Enum(THRESH_TRUNC), "Truncate"));
                    m_thresholdTypeParameter->add(runtime::EnumDescription(runtime::Enum(THRESH_TOZERO), "Truncate to zero"));
                    m_thresholdTypeParameter->add(runtime::EnumDescription(runtime::Enum(THRESH_TOZERO_INV), "Truncate to zero inverted"));
                    parameters.push_back(m_thresholdTypeParameter);
                    
                }
                break;
            case(ALLOCATE):
                {
                    m_thresholdParameter = new runtime::NumericParameter<runtime::Double>(THRESHOLD);
                    m_thresholdParameter->setAccessMode(runtime::Parameter::ACTIVATED_WRITE);
                    m_thresholdParameter->setTitle("Threshold");
                    parameters.push_back(m_thresholdParameter);
                    
                    m_maxvalParameter = new runtime::NumericParameter<runtime::Double>(MAXVAL);
                    m_maxvalParameter->setAccessMode(runtime::Parameter::ACTIVATED_WRITE);
                    m_maxvalParameter->setTitle("Maximal value");
                    parameters.push_back(m_maxvalParameter);
                    
                    m_thresholdTypeParameter = new runtime::EnumParameter(THRESHOLD_TYPE);
                    m_thresholdTypeParameter->setAccessMode(runtime::Parameter::ACTIVATED_WRITE);
                    m_thresholdTypeParameter->setTitle("Threshold type");
                    m_thresholdTypeParameter->add(runtime::EnumDescription(runtime::Enum(THRESH_BINARY), "Binary"));
                    m_thresholdTypeParameter->add(runtime::EnumDescription(runtime::Enum(THRESH_BINARY_INV), "Binary inverted"));
                    m_thresholdTypeParameter->add(runtime::EnumDescription(runtime::Enum(THRESH_TRUNC), "Truncate"));
                    m_thresholdTypeParameter->add(runtime::EnumDescription(runtime::Enum(THRESH_TOZERO), "Truncate to zero"));
                    m_thresholdTypeParameter->add(runtime::EnumDescription(runtime::Enum(THRESH_TOZERO_INV), "Truncate to zero inverted"));
                    parameters.push_back(m_thresholdTypeParameter);
                    
                }
                break;
            case(IN_PLACE):
                {
                    m_thresholdParameter = new runtime::NumericParameter<runtime::Double>(THRESHOLD);
                    m_thresholdParameter->setAccessMode(runtime::Parameter::ACTIVATED_WRITE);
                    m_thresholdParameter->setTitle("Threshold");
                    parameters.push_back(m_thresholdParameter);
                    
                    m_maxvalParameter = new runtime::NumericParameter<runtime::Double>(MAXVAL);
                    m_maxvalParameter->setAccessMode(runtime::Parameter::ACTIVATED_WRITE);
                    m_maxvalParameter->setTitle("Maximal value");
                    parameters.push_back(m_maxvalParameter);
                    
                    m_thresholdTypeParameter = new runtime::EnumParameter(THRESHOLD_TYPE);
                    m_thresholdTypeParameter->setAccessMode(runtime::Parameter::ACTIVATED_WRITE);
                    m_thresholdTypeParameter->setTitle("Threshold type");
                    m_thresholdTypeParameter->add(runtime::EnumDescription(runtime::Enum(THRESH_BINARY), "Binary"));
                    m_thresholdTypeParameter->add(runtime::EnumDescription(runtime::Enum(THRESH_BINARY_INV), "Binary inverted"));
                    m_thresholdTypeParameter->add(runtime::EnumDescription(runtime::Enum(THRESH_TRUNC), "Truncate"));
                    m_thresholdTypeParameter->add(runtime::EnumDescription(runtime::Enum(THRESH_TOZERO), "Truncate to zero"));
                    m_thresholdTypeParameter->add(runtime::EnumDescription(runtime::Enum(THRESH_TOZERO_INV), "Truncate to zero inverted"));
                    parameters.push_back(m_thresholdTypeParameter);
                    
                }
                break;
            }
            
            return parameters;
        }
        
        const std::vector<const runtime::Description*> Threshold::setupInputs()
        {
            std::vector<const runtime::Description*> inputs;
            
            switch(int(m_dataFlow))
            {
            case(MANUAL):
                {
                    runtime::Description* src = new runtime::Description(SRC, runtime::DataVariant::MONO_IMAGE);
                    src->setTitle("Source");
                    inputs.push_back(src);
                    
                    runtime::Description* dst = new runtime::Description(DST, runtime::DataVariant::IMAGE);
                    dst->setTitle("Destination");
                    inputs.push_back(dst);
                    
                }
                break;
            case(ALLOCATE):
                {
                    runtime::Description* src = new runtime::Description(SRC, runtime::DataVariant::MONO_IMAGE);
                    src->setTitle("Source");
                    inputs.push_back(src);
                    
                }
                break;
            case(IN_PLACE):
                {
                    runtime::Description* src = new runtime::Description(SRC, runtime::DataVariant::MONO_IMAGE);
                    src->setTitle("Source");
                    inputs.push_back(src);
                    
                }
                break;
            }
            
            return inputs;
        }
        
        const std::vector<const runtime::Description*> Threshold::setupOutputs()
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
                    runtime::Description* src = new runtime::Description(SRC, runtime::DataVariant::MONO_IMAGE);
                    src->setTitle("Source");
                    outputs.push_back(src);
                    
                }
                break;
            }
            
            return outputs;
        }
        
        void Threshold::initialize()
        {
            runtime::OperatorKernel::initialize(setupInputs(), setupOutputs(), setupParameters());
        }
        
        void Threshold::execute(runtime::DataProvider & provider)
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
                    
                    if(! srcData->variant().isVariant(runtime::DataVariant::MONO_IMAGE))
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
                    double thresholdCvData = double(m_threshold);
                    double maxvalCvData = double(m_maxval);
                    int thresholdTypeCvData = convertThresholdType(m_thresholdType);
                    
                    cv::threshold(srcCvData, dstCvData, thresholdCvData, maxvalCvData, thresholdTypeCvData);
                    
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
                    
                    if(! srcData->variant().isVariant(runtime::DataVariant::MONO_IMAGE))
                    {
                        throw runtime::InputError(SRC, *this, "Wrong input data variant.");
                    }
                    
                    const runtime::Image* srcCastedData = runtime::data_cast<runtime::Image>(srcData);
                    
                    cv::Mat srcCvData = cvsupport::getOpenCvMat(*srcCastedData);
                    cv::Mat dstCvData;
                    double thresholdCvData = double(m_threshold);
                    double maxvalCvData = double(m_maxval);
                    int thresholdTypeCvData = convertThresholdType(m_thresholdType);
                    
                    cv::threshold(srcCvData, dstCvData, thresholdCvData, maxvalCvData, thresholdTypeCvData);
                    
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
                    
                    if(! srcData->variant().isVariant(runtime::DataVariant::MONO_IMAGE))
                    {
                        throw runtime::InputError(SRC, *this, "Wrong input data variant.");
                    }
                    
                    runtime::Image * srcCastedData = runtime::data_cast<runtime::Image>(srcData);
                    
                    cv::Mat srcCvData = cvsupport::getOpenCvMat(*srcCastedData);
                    cv::Mat dstCvData = srcCvData;
                    double thresholdCvData = double(m_threshold);
                    double maxvalCvData = double(m_maxval);
                    int thresholdTypeCvData = convertThresholdType(m_thresholdType);
                    
                    cv::threshold(srcCvData, dstCvData, thresholdCvData, maxvalCvData, thresholdTypeCvData);
                    
                    runtime::DataContainer outContainer = inContainer;
                    runtime::Id2DataPair outputMapper(SRC, outContainer);
                    
                    provider.sendOutputData(outputMapper);
                }
                break;
            }
        }
        
        int Threshold::convertThresholdType(const runtime::Enum & value)
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

