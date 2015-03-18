#include "stromx/cvimgproc/Threshold.h"

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
                        const runtime::Float64 & castedValue = runtime::data_cast<runtime::Float64>(value);
                        if(! castedValue.variant().isVariant(runtime::Variant::FLOAT_64))
                        {
                            throw runtime::WrongParameterType(parameter(id), *this);
                        }
                        cvsupport::checkNumericValue(castedValue, m_maxvalParameter, *this);
                        m_maxval = castedValue;
                    }
                    break;
                case THRESHOLD:
                    {
                        const runtime::Float64 & castedValue = runtime::data_cast<runtime::Float64>(value);
                        if(! castedValue.variant().isVariant(runtime::Variant::FLOAT_64))
                        {
                            throw runtime::WrongParameterType(parameter(id), *this);
                        }
                        cvsupport::checkNumericValue(castedValue, m_thresholdParameter, *this);
                        m_threshold = castedValue;
                    }
                    break;
                case THRESHOLD_TYPE:
                    {
                        const runtime::Enum & castedValue = runtime::data_cast<runtime::Enum>(value);
                        if(! castedValue.variant().isVariant(runtime::Variant::ENUM))
                        {
                            throw runtime::WrongParameterType(parameter(id), *this);
                        }
                        cvsupport::checkEnumValue(castedValue, m_thresholdTypeParameter, *this);
                        m_thresholdType = castedValue;
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
        
        const std::vector<const runtime::Parameter*> Threshold::setupInitParameters()
        {
            std::vector<const runtime::Parameter*> parameters;
            
            m_dataFlowParameter = new runtime::EnumParameter(DATA_FLOW);
            m_dataFlowParameter->setAccessMode(runtime::Parameter::NONE_WRITE);
            m_dataFlowParameter->setTitle(L_("Data flow"));
            m_dataFlowParameter->add(runtime::EnumDescription(runtime::Enum(MANUAL), L_("Manual")));
            m_dataFlowParameter->add(runtime::EnumDescription(runtime::Enum(ALLOCATE), L_("Allocate")));
            m_dataFlowParameter->add(runtime::EnumDescription(runtime::Enum(IN_PLACE), L_("In place")));
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
                    m_thresholdParameter = new runtime::NumericParameter<runtime::Float64>(THRESHOLD);
                    m_thresholdParameter->setAccessMode(runtime::Parameter::ACTIVATED_WRITE);
                    m_thresholdParameter->setTitle(L_("Threshold"));
                    parameters.push_back(m_thresholdParameter);
                    
                    m_maxvalParameter = new runtime::NumericParameter<runtime::Float64>(MAXVAL);
                    m_maxvalParameter->setAccessMode(runtime::Parameter::ACTIVATED_WRITE);
                    m_maxvalParameter->setTitle(L_("Maximal value"));
                    parameters.push_back(m_maxvalParameter);
                    
                    m_thresholdTypeParameter = new runtime::EnumParameter(THRESHOLD_TYPE);
                    m_thresholdTypeParameter->setAccessMode(runtime::Parameter::ACTIVATED_WRITE);
                    m_thresholdTypeParameter->setTitle(L_("Threshold type"));
                    m_thresholdTypeParameter->add(runtime::EnumDescription(runtime::Enum(THRESH_BINARY), L_("Binary")));
                    m_thresholdTypeParameter->add(runtime::EnumDescription(runtime::Enum(THRESH_BINARY_INV), L_("Binary inverted")));
                    m_thresholdTypeParameter->add(runtime::EnumDescription(runtime::Enum(THRESH_TRUNC), L_("Truncate")));
                    m_thresholdTypeParameter->add(runtime::EnumDescription(runtime::Enum(THRESH_TOZERO), L_("Truncate to zero")));
                    m_thresholdTypeParameter->add(runtime::EnumDescription(runtime::Enum(THRESH_TOZERO_INV), L_("Truncate to zero inverted")));
                    parameters.push_back(m_thresholdTypeParameter);
                    
                }
                break;
            case(ALLOCATE):
                {
                    m_thresholdParameter = new runtime::NumericParameter<runtime::Float64>(THRESHOLD);
                    m_thresholdParameter->setAccessMode(runtime::Parameter::ACTIVATED_WRITE);
                    m_thresholdParameter->setTitle(L_("Threshold"));
                    parameters.push_back(m_thresholdParameter);
                    
                    m_maxvalParameter = new runtime::NumericParameter<runtime::Float64>(MAXVAL);
                    m_maxvalParameter->setAccessMode(runtime::Parameter::ACTIVATED_WRITE);
                    m_maxvalParameter->setTitle(L_("Maximal value"));
                    parameters.push_back(m_maxvalParameter);
                    
                    m_thresholdTypeParameter = new runtime::EnumParameter(THRESHOLD_TYPE);
                    m_thresholdTypeParameter->setAccessMode(runtime::Parameter::ACTIVATED_WRITE);
                    m_thresholdTypeParameter->setTitle(L_("Threshold type"));
                    m_thresholdTypeParameter->add(runtime::EnumDescription(runtime::Enum(THRESH_BINARY), L_("Binary")));
                    m_thresholdTypeParameter->add(runtime::EnumDescription(runtime::Enum(THRESH_BINARY_INV), L_("Binary inverted")));
                    m_thresholdTypeParameter->add(runtime::EnumDescription(runtime::Enum(THRESH_TRUNC), L_("Truncate")));
                    m_thresholdTypeParameter->add(runtime::EnumDescription(runtime::Enum(THRESH_TOZERO), L_("Truncate to zero")));
                    m_thresholdTypeParameter->add(runtime::EnumDescription(runtime::Enum(THRESH_TOZERO_INV), L_("Truncate to zero inverted")));
                    parameters.push_back(m_thresholdTypeParameter);
                    
                }
                break;
            case(IN_PLACE):
                {
                    m_thresholdParameter = new runtime::NumericParameter<runtime::Float64>(THRESHOLD);
                    m_thresholdParameter->setAccessMode(runtime::Parameter::ACTIVATED_WRITE);
                    m_thresholdParameter->setTitle(L_("Threshold"));
                    parameters.push_back(m_thresholdParameter);
                    
                    m_maxvalParameter = new runtime::NumericParameter<runtime::Float64>(MAXVAL);
                    m_maxvalParameter->setAccessMode(runtime::Parameter::ACTIVATED_WRITE);
                    m_maxvalParameter->setTitle(L_("Maximal value"));
                    parameters.push_back(m_maxvalParameter);
                    
                    m_thresholdTypeParameter = new runtime::EnumParameter(THRESHOLD_TYPE);
                    m_thresholdTypeParameter->setAccessMode(runtime::Parameter::ACTIVATED_WRITE);
                    m_thresholdTypeParameter->setTitle(L_("Threshold type"));
                    m_thresholdTypeParameter->add(runtime::EnumDescription(runtime::Enum(THRESH_BINARY), L_("Binary")));
                    m_thresholdTypeParameter->add(runtime::EnumDescription(runtime::Enum(THRESH_BINARY_INV), L_("Binary inverted")));
                    m_thresholdTypeParameter->add(runtime::EnumDescription(runtime::Enum(THRESH_TRUNC), L_("Truncate")));
                    m_thresholdTypeParameter->add(runtime::EnumDescription(runtime::Enum(THRESH_TOZERO), L_("Truncate to zero")));
                    m_thresholdTypeParameter->add(runtime::EnumDescription(runtime::Enum(THRESH_TOZERO_INV), L_("Truncate to zero inverted")));
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
                    m_srcDescription = new runtime::Description(SRC, runtime::Variant::MONO_IMAGE);
                    m_srcDescription->setTitle(L_("Source"));
                    inputs.push_back(m_srcDescription);
                    
                    m_dstDescription = new runtime::Description(DST, runtime::Variant::IMAGE);
                    m_dstDescription->setTitle(L_("Destination"));
                    inputs.push_back(m_dstDescription);
                    
                }
                break;
            case(ALLOCATE):
                {
                    m_srcDescription = new runtime::Description(SRC, runtime::Variant::MONO_IMAGE);
                    m_srcDescription->setTitle(L_("Source"));
                    inputs.push_back(m_srcDescription);
                    
                }
                break;
            case(IN_PLACE):
                {
                    m_srcDescription = new runtime::Description(SRC, runtime::Variant::MONO_IMAGE);
                    m_srcDescription->setTitle(L_("Source"));
                    inputs.push_back(m_srcDescription);
                    
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
            case(IN_PLACE):
                {
                    runtime::Description* src = new runtime::Description(SRC, runtime::Variant::MONO_IMAGE);
                    src->setTitle(L_("Source"));
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
                    
                    dstCastedData->initializeImage(srcCastedData->width(), srcCastedData->height(), srcCastedData->stride(), dstCastedData->data(), srcCastedData->pixelType());
                    
                    cv::Mat srcCvData = cvsupport::getOpenCvMat(*srcCastedData);
                    cv::Mat dstCvData = cvsupport::getOpenCvMat(*dstCastedData);
                    double thresholdCvData = double(m_threshold);
                    double maxvalCvData = double(m_maxval);
                    int thresholdTypeCvData = convertThresholdType(m_thresholdType);
                    
                    cv::threshold(srcCvData, dstCvData, thresholdCvData, maxvalCvData, thresholdTypeCvData);
                    
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
                    double thresholdCvData = double(m_threshold);
                    double maxvalCvData = double(m_maxval);
                    int thresholdTypeCvData = convertThresholdType(m_thresholdType);
                    
                    cv::threshold(srcCvData, dstCvData, thresholdCvData, maxvalCvData, thresholdTypeCvData);
                    
                    runtime::Image* dstCastedData = new cvsupport::Image(dstCvData);
                    runtime::DataContainer dstOutContainer = runtime::DataContainer(dstCastedData);
                    runtime::Id2DataPair dstOutMapper(DST, dstOutContainer);
                    
                    dstCastedData->initializeImage(dstCastedData->width(), dstCastedData->height(), dstCastedData->stride(), dstCastedData->data(), srcCastedData->pixelType());
                    provider.sendOutputData(dstOutMapper);
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
                    
                    if(! srcData->variant().isVariant(m_srcDescription->variant()))
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
                    
                    runtime::DataContainer srcOutContainer = inContainer;
                    runtime::Id2DataPair srcOutMapper(SRC, srcOutContainer);
                    
                    provider.sendOutputData(srcOutMapper);
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
        
    } // cvimgproc
} // stromx

