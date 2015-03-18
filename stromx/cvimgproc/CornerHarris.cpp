#include "stromx/cvimgproc/CornerHarris.h"

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
        const std::string CornerHarris::PACKAGE(STROMX_CVIMGPROC_PACKAGE_NAME);
        const runtime::Version CornerHarris::VERSION(STROMX_CVIMGPROC_VERSION_MAJOR, STROMX_CVIMGPROC_VERSION_MINOR, STROMX_CVIMGPROC_VERSION_PATCH);
        const std::string CornerHarris::TYPE("CornerHarris");
        
        CornerHarris::CornerHarris()
          : runtime::OperatorKernel(TYPE, PACKAGE, VERSION, setupInitParameters()),
            m_blockSize(3),
            m_k(1),
            m_ksize(3),
            m_dataFlow()
        {
        }
        
        const runtime::DataRef CornerHarris::getParameter(unsigned int id) const
        {
            switch(id)
            {
            case BLOCK_SIZE:
                return m_blockSize;
            case K:
                return m_k;
            case KSIZE:
                return m_ksize;
            case DATA_FLOW:
                return m_dataFlow;
            default:
                throw runtime::WrongParameterId(id, *this);
            }
        }
        
        void CornerHarris::setParameter(unsigned int id, const runtime::Data& value)
        {
            try
            {
                switch(id)
                {
                case BLOCK_SIZE:
                    {
                        const runtime::UInt32 & castedValue = runtime::data_cast<runtime::UInt32>(value);
                        if(! castedValue.variant().isVariant(runtime::Variant::UINT_32))
                        {
                            throw runtime::WrongParameterType(parameter(id), *this);
                        }
                        cvsupport::checkNumericValue(castedValue, m_blockSizeParameter, *this);
                        if(int(castedValue) % 2 == 0)
                            throw runtime::WrongParameterValue(*m_blockSizeParameter, *this, "Only odd values are allowed");
                        m_blockSize = castedValue;
                    }
                    break;
                case K:
                    {
                        const runtime::Float64 & castedValue = runtime::data_cast<runtime::Float64>(value);
                        if(! castedValue.variant().isVariant(runtime::Variant::FLOAT_64))
                        {
                            throw runtime::WrongParameterType(parameter(id), *this);
                        }
                        cvsupport::checkNumericValue(castedValue, m_kParameter, *this);
                        m_k = castedValue;
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
        
        const std::vector<const runtime::Parameter*> CornerHarris::setupInitParameters()
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
        
        const std::vector<const runtime::Parameter*> CornerHarris::setupParameters()
        {
            std::vector<const runtime::Parameter*> parameters;
            
            switch(int(m_dataFlow))
            {
            case(MANUAL):
                {
                    m_blockSizeParameter = new runtime::NumericParameter<runtime::UInt32>(BLOCK_SIZE);
                    m_blockSizeParameter->setAccessMode(runtime::Parameter::ACTIVATED_WRITE);
                    m_blockSizeParameter->setTitle(L_("Block size"));
                    m_blockSizeParameter->setMin(runtime::UInt32(1));
                    parameters.push_back(m_blockSizeParameter);
                    
                    m_ksizeParameter = new runtime::NumericParameter<runtime::UInt32>(KSIZE);
                    m_ksizeParameter->setAccessMode(runtime::Parameter::ACTIVATED_WRITE);
                    m_ksizeParameter->setTitle(L_("Kernel size"));
                    m_ksizeParameter->setMin(runtime::UInt32(1));
                    m_ksizeParameter->setStep(runtime::UInt32(2));
                    parameters.push_back(m_ksizeParameter);
                    
                    m_kParameter = new runtime::NumericParameter<runtime::Float64>(K);
                    m_kParameter->setAccessMode(runtime::Parameter::ACTIVATED_WRITE);
                    m_kParameter->setTitle(L_("Harris parameter"));
                    parameters.push_back(m_kParameter);
                    
                }
                break;
            case(ALLOCATE):
                {
                    m_blockSizeParameter = new runtime::NumericParameter<runtime::UInt32>(BLOCK_SIZE);
                    m_blockSizeParameter->setAccessMode(runtime::Parameter::ACTIVATED_WRITE);
                    m_blockSizeParameter->setTitle(L_("Block size"));
                    m_blockSizeParameter->setMin(runtime::UInt32(1));
                    parameters.push_back(m_blockSizeParameter);
                    
                    m_ksizeParameter = new runtime::NumericParameter<runtime::UInt32>(KSIZE);
                    m_ksizeParameter->setAccessMode(runtime::Parameter::ACTIVATED_WRITE);
                    m_ksizeParameter->setTitle(L_("Kernel size"));
                    m_ksizeParameter->setMin(runtime::UInt32(1));
                    m_ksizeParameter->setStep(runtime::UInt32(2));
                    parameters.push_back(m_ksizeParameter);
                    
                    m_kParameter = new runtime::NumericParameter<runtime::Float64>(K);
                    m_kParameter->setAccessMode(runtime::Parameter::ACTIVATED_WRITE);
                    m_kParameter->setTitle(L_("Harris parameter"));
                    parameters.push_back(m_kParameter);
                    
                }
                break;
            }
            
            return parameters;
        }
        
        const std::vector<const runtime::Description*> CornerHarris::setupInputs()
        {
            std::vector<const runtime::Description*> inputs;
            
            switch(int(m_dataFlow))
            {
            case(MANUAL):
                {
                    m_srcDescription = new runtime::Description(SRC, runtime::Variant::MONO_IMAGE);
                    m_srcDescription->setTitle(L_("Source"));
                    inputs.push_back(m_srcDescription);
                    
                    m_dstDescription = new runtime::Description(DST, runtime::Variant::MATRIX);
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
            }
            
            return inputs;
        }
        
        const std::vector<const runtime::Description*> CornerHarris::setupOutputs()
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
        
        void CornerHarris::initialize()
        {
            runtime::OperatorKernel::initialize(setupInputs(), setupOutputs(), setupParameters());
        }
        
        void CornerHarris::execute(runtime::DataProvider & provider)
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
                    int blockSizeCvData = int(m_blockSize);
                    int ksizeCvData = int(m_ksize);
                    double kCvData = double(m_k);
                    
                    cv::cornerHarris(srcCvData, dstCvData, blockSizeCvData, ksizeCvData, kCvData);
                    
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
                    int blockSizeCvData = int(m_blockSize);
                    int ksizeCvData = int(m_ksize);
                    double kCvData = double(m_k);
                    
                    cv::cornerHarris(srcCvData, dstCvData, blockSizeCvData, ksizeCvData, kCvData);
                    
                    runtime::Matrix* dstCastedData = new cvsupport::Matrix(dstCvData);
                    runtime::DataContainer dstOutContainer = runtime::DataContainer(dstCastedData);
                    runtime::Id2DataPair dstOutMapper(DST, dstOutContainer);
                    
                    provider.sendOutputData(dstOutMapper);
                }
                break;
            }
        }
        
    } // cvimgproc
} // stromx

