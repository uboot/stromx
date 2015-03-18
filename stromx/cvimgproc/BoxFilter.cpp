#include "stromx/cvimgproc/BoxFilter.h"

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
        const std::string BoxFilter::PACKAGE(STROMX_CVIMGPROC_PACKAGE_NAME);
        const runtime::Version BoxFilter::VERSION(STROMX_CVIMGPROC_VERSION_MAJOR, STROMX_CVIMGPROC_VERSION_MINOR, STROMX_CVIMGPROC_VERSION_PATCH);
        const std::string BoxFilter::TYPE("BoxFilter");
        
        BoxFilter::BoxFilter()
          : runtime::OperatorKernel(TYPE, PACKAGE, VERSION, setupInitParameters()),
            m_ksizex(3),
            m_ksizey(3),
            m_dataFlow()
        {
        }
        
        const runtime::DataRef BoxFilter::getParameter(unsigned int id) const
        {
            switch(id)
            {
            case KSIZEX:
                return m_ksizex;
            case KSIZEY:
                return m_ksizey;
            case DATA_FLOW:
                return m_dataFlow;
            default:
                throw runtime::WrongParameterId(id, *this);
            }
        }
        
        void BoxFilter::setParameter(unsigned int id, const runtime::Data& value)
        {
            try
            {
                switch(id)
                {
                case KSIZEX:
                    {
                        const runtime::UInt32 & castedValue = runtime::data_cast<runtime::UInt32>(value);
                        if(! castedValue.variant().isVariant(runtime::Variant::UINT_32))
                        {
                            throw runtime::WrongParameterType(parameter(id), *this);
                        }
                        cvsupport::checkNumericValue(castedValue, m_ksizexParameter, *this);
                        m_ksizex = castedValue;
                    }
                    break;
                case KSIZEY:
                    {
                        const runtime::UInt32 & castedValue = runtime::data_cast<runtime::UInt32>(value);
                        if(! castedValue.variant().isVariant(runtime::Variant::UINT_32))
                        {
                            throw runtime::WrongParameterType(parameter(id), *this);
                        }
                        cvsupport::checkNumericValue(castedValue, m_ksizeyParameter, *this);
                        m_ksizey = castedValue;
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
        
        const std::vector<const runtime::Parameter*> BoxFilter::setupInitParameters()
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
        
        const std::vector<const runtime::Parameter*> BoxFilter::setupParameters()
        {
            std::vector<const runtime::Parameter*> parameters;
            
            switch(int(m_dataFlow))
            {
            case(MANUAL):
                {
                    m_ksizexParameter = new runtime::NumericParameter<runtime::UInt32>(KSIZEX);
                    m_ksizexParameter->setAccessMode(runtime::Parameter::ACTIVATED_WRITE);
                    m_ksizexParameter->setTitle(L_("Kernel size X"));
                    m_ksizexParameter->setMin(runtime::UInt32(1));
                    parameters.push_back(m_ksizexParameter);
                    
                    m_ksizeyParameter = new runtime::NumericParameter<runtime::UInt32>(KSIZEY);
                    m_ksizeyParameter->setAccessMode(runtime::Parameter::ACTIVATED_WRITE);
                    m_ksizeyParameter->setTitle(L_("Kernel size Y"));
                    m_ksizeyParameter->setMin(runtime::UInt32(1));
                    parameters.push_back(m_ksizeyParameter);
                    
                }
                break;
            case(ALLOCATE):
                {
                    m_ksizexParameter = new runtime::NumericParameter<runtime::UInt32>(KSIZEX);
                    m_ksizexParameter->setAccessMode(runtime::Parameter::ACTIVATED_WRITE);
                    m_ksizexParameter->setTitle(L_("Kernel size X"));
                    m_ksizexParameter->setMin(runtime::UInt32(1));
                    parameters.push_back(m_ksizexParameter);
                    
                    m_ksizeyParameter = new runtime::NumericParameter<runtime::UInt32>(KSIZEY);
                    m_ksizeyParameter->setAccessMode(runtime::Parameter::ACTIVATED_WRITE);
                    m_ksizeyParameter->setTitle(L_("Kernel size Y"));
                    m_ksizeyParameter->setMin(runtime::UInt32(1));
                    parameters.push_back(m_ksizeyParameter);
                    
                }
                break;
            case(IN_PLACE):
                {
                    m_ksizexParameter = new runtime::NumericParameter<runtime::UInt32>(KSIZEX);
                    m_ksizexParameter->setAccessMode(runtime::Parameter::ACTIVATED_WRITE);
                    m_ksizexParameter->setTitle(L_("Kernel size X"));
                    m_ksizexParameter->setMin(runtime::UInt32(1));
                    parameters.push_back(m_ksizexParameter);
                    
                    m_ksizeyParameter = new runtime::NumericParameter<runtime::UInt32>(KSIZEY);
                    m_ksizeyParameter->setAccessMode(runtime::Parameter::ACTIVATED_WRITE);
                    m_ksizeyParameter->setTitle(L_("Kernel size Y"));
                    m_ksizeyParameter->setMin(runtime::UInt32(1));
                    parameters.push_back(m_ksizeyParameter);
                    
                }
                break;
            }
            
            return parameters;
        }
        
        const std::vector<const runtime::Description*> BoxFilter::setupInputs()
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
            case(IN_PLACE):
                {
                    m_srcDescription = new runtime::Description(SRC, runtime::Variant::IMAGE);
                    m_srcDescription->setTitle(L_("Source"));
                    inputs.push_back(m_srcDescription);
                    
                }
                break;
            }
            
            return inputs;
        }
        
        const std::vector<const runtime::Description*> BoxFilter::setupOutputs()
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
                    runtime::Description* src = new runtime::Description(SRC, runtime::Variant::IMAGE);
                    src->setTitle(L_("Source"));
                    outputs.push_back(src);
                    
                }
                break;
            }
            
            return outputs;
        }
        
        void BoxFilter::initialize()
        {
            runtime::OperatorKernel::initialize(setupInputs(), setupOutputs(), setupParameters());
        }
        
        void BoxFilter::execute(runtime::DataProvider & provider)
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
                    int ksizexCvData = int(m_ksizex);
                    int ksizeyCvData = int(m_ksizey);
                    
                    cv::boxFilter(srcCvData, dstCvData, -1, cv::Size(ksizexCvData, ksizeyCvData));
                    
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
                    int ksizexCvData = int(m_ksizex);
                    int ksizeyCvData = int(m_ksizey);
                    
                    cv::boxFilter(srcCvData, dstCvData, -1, cv::Size(ksizexCvData, ksizeyCvData));
                    
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
                    int ksizexCvData = int(m_ksizex);
                    int ksizeyCvData = int(m_ksizey);
                    
                    cv::boxFilter(srcCvData, dstCvData, -1, cv::Size(ksizexCvData, ksizeyCvData));
                    
                    runtime::DataContainer srcOutContainer = inContainer;
                    runtime::Id2DataPair srcOutMapper(SRC, srcOutContainer);
                    
                    provider.sendOutputData(srcOutMapper);
                }
                break;
            }
        }
        
    } // cvimgproc
} // stromx

