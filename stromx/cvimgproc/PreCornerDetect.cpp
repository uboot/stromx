#include "stromx/cvimgproc/PreCornerDetect.h"

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
        const std::string PreCornerDetect::PACKAGE(STROMX_CVIMGPROC_PACKAGE_NAME);
        const runtime::Version PreCornerDetect::VERSION(STROMX_CVIMGPROC_VERSION_MAJOR, STROMX_CVIMGPROC_VERSION_MINOR, STROMX_CVIMGPROC_VERSION_PATCH);
        const std::string PreCornerDetect::TYPE("PreCornerDetect");
        
        PreCornerDetect::PreCornerDetect()
          : runtime::OperatorKernel(TYPE, PACKAGE, VERSION, setupInitParameters()),
            m_borderType(BORDER_DEFAULT),
            m_ksize(3),
            m_dataFlow()
        {
        }
        
        const runtime::DataRef PreCornerDetect::getParameter(unsigned int id) const
        {
            switch(id)
            {
            case BORDER_TYPE:
                return m_borderType;
            case KSIZE:
                return m_ksize;
            case DATA_FLOW:
                return m_dataFlow;
            default:
                throw runtime::WrongParameterId(id, *this);
            }
        }
        
        void PreCornerDetect::setParameter(unsigned int id, const runtime::Data& value)
        {
            try
            {
                switch(id)
                {
                case BORDER_TYPE:
                    {
                        const runtime::Enum & castedValue = runtime::data_cast<runtime::Enum>(value);
                        if(! castedValue.variant().isVariant(runtime::Variant::ENUM))
                        {
                            throw runtime::WrongParameterType(parameter(id), *this);
                        }
                        cvsupport::checkEnumValue(castedValue, m_borderTypeParameter, *this);
                        m_borderType = castedValue;
                    }
                    break;
                case KSIZE:
                    {
                        const runtime::UInt32 & castedValue = runtime::data_cast<runtime::UInt32>(value);
                        if(! castedValue.variant().isVariant(runtime::Variant::UINT_32))
                        {
                            throw runtime::WrongParameterType(parameter(id), *this);
                        }
                        cvsupport::checkNumericValue(castedValue, m_ksizeParameter, *this);
                        if(int(castedValue) % 2 == 0)
                            throw runtime::WrongParameterValue(*m_ksizeParameter, *this, "Only odd values are allowed");
                        m_ksize = castedValue;
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
        
        const std::vector<const runtime::Parameter*> PreCornerDetect::setupInitParameters()
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
        
        const std::vector<const runtime::Parameter*> PreCornerDetect::setupParameters()
        {
            std::vector<const runtime::Parameter*> parameters;
            
            switch(int(m_dataFlow))
            {
            case(MANUAL):
                {
                    m_ksizeParameter = new runtime::NumericParameter<runtime::UInt32>(KSIZE);
                    m_ksizeParameter->setAccessMode(runtime::Parameter::ACTIVATED_WRITE);
                    m_ksizeParameter->setTitle(L_("Kernel size"));
                    m_ksizeParameter->setMax(runtime::UInt32(7));
                    m_ksizeParameter->setMin(runtime::UInt32(1));
                    m_ksizeParameter->setStep(runtime::UInt32(2));
                    parameters.push_back(m_ksizeParameter);
                    
                    m_borderTypeParameter = new runtime::EnumParameter(BORDER_TYPE);
                    m_borderTypeParameter->setAccessMode(runtime::Parameter::ACTIVATED_WRITE);
                    m_borderTypeParameter->setTitle(L_("Border type"));
                    m_borderTypeParameter->add(runtime::EnumDescription(runtime::Enum(BORDER_DEFAULT), L_("Default")));
                    m_borderTypeParameter->add(runtime::EnumDescription(runtime::Enum(BORDER_CONSTANT), L_("Constant")));
                    m_borderTypeParameter->add(runtime::EnumDescription(runtime::Enum(BORDER_REFLECT), L_("Reflect")));
                    m_borderTypeParameter->add(runtime::EnumDescription(runtime::Enum(BORDER_REPLICATE), L_("Replicate")));
                    parameters.push_back(m_borderTypeParameter);
                    
                }
                break;
            case(ALLOCATE):
                {
                    m_ksizeParameter = new runtime::NumericParameter<runtime::UInt32>(KSIZE);
                    m_ksizeParameter->setAccessMode(runtime::Parameter::ACTIVATED_WRITE);
                    m_ksizeParameter->setTitle(L_("Kernel size"));
                    m_ksizeParameter->setMax(runtime::UInt32(7));
                    m_ksizeParameter->setMin(runtime::UInt32(1));
                    m_ksizeParameter->setStep(runtime::UInt32(2));
                    parameters.push_back(m_ksizeParameter);
                    
                    m_borderTypeParameter = new runtime::EnumParameter(BORDER_TYPE);
                    m_borderTypeParameter->setAccessMode(runtime::Parameter::ACTIVATED_WRITE);
                    m_borderTypeParameter->setTitle(L_("Border type"));
                    m_borderTypeParameter->add(runtime::EnumDescription(runtime::Enum(BORDER_DEFAULT), L_("Default")));
                    m_borderTypeParameter->add(runtime::EnumDescription(runtime::Enum(BORDER_CONSTANT), L_("Constant")));
                    m_borderTypeParameter->add(runtime::EnumDescription(runtime::Enum(BORDER_REFLECT), L_("Reflect")));
                    m_borderTypeParameter->add(runtime::EnumDescription(runtime::Enum(BORDER_REPLICATE), L_("Replicate")));
                    parameters.push_back(m_borderTypeParameter);
                    
                }
                break;
            }
            
            return parameters;
        }
        
        const std::vector<const runtime::Description*> PreCornerDetect::setupInputs()
        {
            std::vector<const runtime::Description*> inputs;
            
            switch(int(m_dataFlow))
            {
            case(MANUAL):
                {
                    m_srcDescription = new runtime::Description(SRC, runtime::Variant::MONO_8_IMAGE);
                    m_srcDescription->setTitle(L_("Source"));
                    inputs.push_back(m_srcDescription);
                    
                    m_dstDescription = new runtime::Description(DST, runtime::Variant::MATRIX);
                    m_dstDescription->setTitle(L_("Destination"));
                    inputs.push_back(m_dstDescription);
                    
                }
                break;
            case(ALLOCATE):
                {
                    m_srcDescription = new runtime::Description(SRC, runtime::Variant::MONO_8_IMAGE);
                    m_srcDescription->setTitle(L_("Source"));
                    inputs.push_back(m_srcDescription);
                    
                }
                break;
            }
            
            return inputs;
        }
        
        const std::vector<const runtime::Description*> PreCornerDetect::setupOutputs()
        {
            std::vector<const runtime::Description*> outputs;
            
            switch(int(m_dataFlow))
            {
            case(MANUAL):
                {
                    runtime::Description* dst = new runtime::Description(DST, runtime::Variant::FLOAT_32_MATRIX);
                    dst->setTitle(L_("Destination"));
                    outputs.push_back(dst);
                    
                }
                break;
            case(ALLOCATE):
                {
                    runtime::Description* dst = new runtime::Description(DST, runtime::Variant::FLOAT_32_MATRIX);
                    dst->setTitle(L_("Destination"));
                    outputs.push_back(dst);
                    
                }
                break;
            }
            
            return outputs;
        }
        
        void PreCornerDetect::initialize()
        {
            runtime::OperatorKernel::initialize(setupInputs(), setupOutputs(), setupParameters());
        }
        
        void PreCornerDetect::execute(runtime::DataProvider & provider)
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
                    runtime::Matrix * dstCastedData = runtime::data_cast<runtime::Matrix>(dstData);
                    
                    unsigned int stride = srcCastedData->cols() * runtime::Matrix::valueSize(runtime::Matrix::FLOAT_32);
                    dstCastedData->initializeMatrix(srcCastedData->rows(), srcCastedData->cols(), stride, dstCastedData->data(), runtime::Matrix::FLOAT_32);
                    
                    cv::Mat srcCvData = cvsupport::getOpenCvMat(*srcCastedData);
                    cv::Mat dstCvData = cvsupport::getOpenCvMat(*dstCastedData);
                    int ksizeCvData = int(m_ksize);
                    int borderTypeCvData = convertBorderType(m_borderType);
                    
                    cv::preCornerDetect(srcCvData, dstCvData, ksizeCvData, borderTypeCvData);
                    
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
                    int ksizeCvData = int(m_ksize);
                    int borderTypeCvData = convertBorderType(m_borderType);
                    
                    cv::preCornerDetect(srcCvData, dstCvData, ksizeCvData, borderTypeCvData);
                    
                    runtime::Matrix* dstCastedData = new cvsupport::Matrix(dstCvData);
                    runtime::DataContainer dstOutContainer = runtime::DataContainer(dstCastedData);
                    runtime::Id2DataPair dstOutMapper(DST, dstOutContainer);
                    
                    provider.sendOutputData(dstOutMapper);
                }
                break;
            }
        }
        
        int PreCornerDetect::convertBorderType(const runtime::Enum & value)
        {
            switch(int(value))
            {
            case BORDER_DEFAULT:
                return cv::BORDER_DEFAULT;
            case BORDER_CONSTANT:
                return cv::BORDER_CONSTANT;
            case BORDER_REFLECT:
                return cv::BORDER_REFLECT;
            case BORDER_REPLICATE:
                return cv::BORDER_REPLICATE;
            default:
                throw runtime::WrongParameterValue(parameter(BORDER_TYPE), *this);
            }
        }
        
    } // cvimgproc
} // stromx

