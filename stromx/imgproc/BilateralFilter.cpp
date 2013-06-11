#include "stromx/imgproc/BilateralFilter.h"

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
        const std::string BilateralFilter::PACKAGE(STROMX_IMGPROC_PACKAGE_NAME);
        const runtime::Version BilateralFilter::VERSION(STROMX_IMGPROC_VERSION_MAJOR, STROMX_IMGPROC_VERSION_MINOR, STROMX_IMGPROC_VERSION_PATCH);
        const std::string BilateralFilter::TYPE("BilateralFilter");
        
        BilateralFilter::BilateralFilter()
          : runtime::OperatorKernel(TYPE, PACKAGE, VERSION, setupInitParameters()),
            m_d(9),
            m_sigmaSpace(50.0),
            m_sigmaColor(50.0),
            m_dataFlow()
        {
        }
        
        const runtime::DataRef BilateralFilter::getParameter(unsigned int id) const
        {
            switch(id)
            {
            case D:
                return m_d;
            case SIGMA_SPACE:
                return m_sigmaSpace;
            case SIGMA_COLOR:
                return m_sigmaColor;
            case DATA_FLOW:
                return m_dataFlow;
            default:
                throw runtime::WrongParameterId(id, *this);
            }
        }
        
        void BilateralFilter::setParameter(unsigned int id, const runtime::Data& value)
        {
            try
            {
                switch(id)
                {
                case D:
                    {
                        runtime::UInt32 castedValue = runtime::data_cast<runtime::UInt32>(value);
                        checkNumericValue(castedValue, m_dParameter, *this);
                        m_d = castedValue;
                    }
                    break;
                case SIGMA_SPACE:
                    {
                        runtime::Double castedValue = runtime::data_cast<runtime::Double>(value);
                        checkNumericValue(castedValue, m_sigmaSpaceParameter, *this);
                        m_sigmaSpace = castedValue;
                    }
                    break;
                case SIGMA_COLOR:
                    {
                        runtime::Double castedValue = runtime::data_cast<runtime::Double>(value);
                        checkNumericValue(castedValue, m_sigmaColorParameter, *this);
                        m_sigmaColor = castedValue;
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
        
        const std::vector<const runtime::Parameter*> BilateralFilter::setupInitParameters()
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
        
        const std::vector<const runtime::Parameter*> BilateralFilter::setupParameters()
        {
            std::vector<const runtime::Parameter*> parameters;
            
            switch(int(m_dataFlow))
            {
            case(MANUAL):
                {
                    m_dParameter = new runtime::NumericParameter<runtime::UInt32>(D);
                    m_dParameter->setAccessMode(runtime::Parameter::ACTIVATED_WRITE);
                    m_dParameter->setTitle("Pixel neigbourhood diameter");
                    parameters.push_back(m_dParameter);
                    
                    m_sigmaColorParameter = new runtime::NumericParameter<runtime::Double>(SIGMA_COLOR);
                    m_sigmaColorParameter->setAccessMode(runtime::Parameter::ACTIVATED_WRITE);
                    m_sigmaColorParameter->setTitle("Sigma color");
                    parameters.push_back(m_sigmaColorParameter);
                    
                    m_sigmaSpaceParameter = new runtime::NumericParameter<runtime::Double>(SIGMA_SPACE);
                    m_sigmaSpaceParameter->setAccessMode(runtime::Parameter::ACTIVATED_WRITE);
                    m_sigmaSpaceParameter->setTitle("Sigma space");
                    parameters.push_back(m_sigmaSpaceParameter);
                    
                }
                break;
            case(ALLOCATE):
                {
                    m_dParameter = new runtime::NumericParameter<runtime::UInt32>(D);
                    m_dParameter->setAccessMode(runtime::Parameter::ACTIVATED_WRITE);
                    m_dParameter->setTitle("Pixel neigbourhood diameter");
                    parameters.push_back(m_dParameter);
                    
                    m_sigmaColorParameter = new runtime::NumericParameter<runtime::Double>(SIGMA_COLOR);
                    m_sigmaColorParameter->setAccessMode(runtime::Parameter::ACTIVATED_WRITE);
                    m_sigmaColorParameter->setTitle("Sigma color");
                    parameters.push_back(m_sigmaColorParameter);
                    
                    m_sigmaSpaceParameter = new runtime::NumericParameter<runtime::Double>(SIGMA_SPACE);
                    m_sigmaSpaceParameter->setAccessMode(runtime::Parameter::ACTIVATED_WRITE);
                    m_sigmaSpaceParameter->setTitle("Sigma space");
                    parameters.push_back(m_sigmaSpaceParameter);
                    
                }
                break;
            }
            
            return parameters;
        }
        
        const std::vector<const runtime::Description*> BilateralFilter::setupInputs()
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
        
        const std::vector<const runtime::Description*> BilateralFilter::setupOutputs()
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
        
        void BilateralFilter::initialize()
        {
            runtime::OperatorKernel::initialize(setupInputs(), setupOutputs(), setupParameters());
        }
        
        void BilateralFilter::execute(runtime::DataProvider & provider)
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
                    
                    dstCastedData->initializeImage(srcCastedData->width(), srcCastedData->height(), srcCastedData->stride(), dstCastedData->data(), srcCastedData->pixelType());
                    
                    cv::Mat srcCvData = imgutil::getOpenCvMat(*srcCastedData);
                    cv::Mat dstCvData = imgutil::getOpenCvMat(*dstCastedData);
                    int dCvData = int(m_d);
                    double sigmaColorCvData = double(m_sigmaColor);
                    double sigmaSpaceCvData = double(m_sigmaSpace);
                    
                    cv::bilateralFilter(srcCvData, dstCvData, dCvData, sigmaColorCvData, sigmaSpaceCvData);
                    
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
                    int dCvData = int(m_d);
                    double sigmaColorCvData = double(m_sigmaColor);
                    double sigmaSpaceCvData = double(m_sigmaSpace);
                    
                    cv::bilateralFilter(srcCvData, dstCvData, dCvData, sigmaColorCvData, sigmaSpaceCvData);
                    
                    runtime::Image* dstCastedData = new imgutil::Image(dstCvData);
                    runtime::DataContainer outContainer = runtime::DataContainer(dstCastedData);
                    runtime::Id2DataPair outputMapper(DST, outContainer);
                    
                    dstCastedData->initializeImage(dstCastedData->width(), dstCastedData->height(), dstCastedData->stride(), dstCastedData->data(), srcCastedData->pixelType());
                    provider.sendOutputData(outputMapper);
                }
                break;
            }
        }
        
    }
}

