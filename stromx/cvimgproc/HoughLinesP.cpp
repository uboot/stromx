#include "stromx/cvimgproc/HoughLinesP.h"

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
        const std::string HoughLinesP::PACKAGE(STROMX_CVIMGPROC_PACKAGE_NAME);
        const runtime::Version HoughLinesP::VERSION(STROMX_CVIMGPROC_VERSION_MAJOR, STROMX_CVIMGPROC_VERSION_MINOR, STROMX_CVIMGPROC_VERSION_PATCH);
        const std::string HoughLinesP::TYPE("HoughLinesP");
        
        HoughLinesP::HoughLinesP()
          : runtime::OperatorKernel(TYPE, PACKAGE, VERSION, setupInitParameters()),
            m_maxLineGap(5),
            m_minLineLength(50),
            m_rho(1.0),
            m_theta(0.0174532925199),
            m_threshold(100),
            m_dataFlow()
        {
        }
        
        const runtime::DataRef HoughLinesP::getParameter(unsigned int id) const
        {
            switch(id)
            {
            case MAX_LINE_GAP:
                return m_maxLineGap;
            case MIN_LINE_LENGTH:
                return m_minLineLength;
            case RHO:
                return m_rho;
            case THETA:
                return m_theta;
            case THRESHOLD:
                return m_threshold;
            case DATA_FLOW:
                return m_dataFlow;
            default:
                throw runtime::WrongParameterId(id, *this);
            }
        }
        
        void HoughLinesP::setParameter(unsigned int id, const runtime::Data& value)
        {
            try
            {
                switch(id)
                {
                case MAX_LINE_GAP:
                    {
                        const runtime::Float64 & castedValue = runtime::data_cast<runtime::Float64>(value);
                        if(! castedValue.variant().isVariant(runtime::Variant::FLOAT_64))
                        {
                            throw runtime::WrongParameterType(parameter(id), *this);
                        }
                        cvsupport::checkNumericValue(castedValue, m_maxLineGapParameter, *this);
                        m_maxLineGap = castedValue;
                    }
                    break;
                case MIN_LINE_LENGTH:
                    {
                        const runtime::Float64 & castedValue = runtime::data_cast<runtime::Float64>(value);
                        if(! castedValue.variant().isVariant(runtime::Variant::FLOAT_64))
                        {
                            throw runtime::WrongParameterType(parameter(id), *this);
                        }
                        cvsupport::checkNumericValue(castedValue, m_minLineLengthParameter, *this);
                        m_minLineLength = castedValue;
                    }
                    break;
                case RHO:
                    {
                        const runtime::Float64 & castedValue = runtime::data_cast<runtime::Float64>(value);
                        if(! castedValue.variant().isVariant(runtime::Variant::FLOAT_64))
                        {
                            throw runtime::WrongParameterType(parameter(id), *this);
                        }
                        cvsupport::checkNumericValue(castedValue, m_rhoParameter, *this);
                        m_rho = castedValue;
                    }
                    break;
                case THETA:
                    {
                        const runtime::Float64 & castedValue = runtime::data_cast<runtime::Float64>(value);
                        if(! castedValue.variant().isVariant(runtime::Variant::FLOAT_64))
                        {
                            throw runtime::WrongParameterType(parameter(id), *this);
                        }
                        cvsupport::checkNumericValue(castedValue, m_thetaParameter, *this);
                        m_theta = castedValue;
                    }
                    break;
                case THRESHOLD:
                    {
                        const runtime::UInt32 & castedValue = runtime::data_cast<runtime::UInt32>(value);
                        if(! castedValue.variant().isVariant(runtime::Variant::UINT_32))
                        {
                            throw runtime::WrongParameterType(parameter(id), *this);
                        }
                        cvsupport::checkNumericValue(castedValue, m_thresholdParameter, *this);
                        m_threshold = castedValue;
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
        
        const std::vector<const runtime::Parameter*> HoughLinesP::setupInitParameters()
        {
            std::vector<const runtime::Parameter*> parameters;
            
            return parameters;
        }
        
        const std::vector<const runtime::Parameter*> HoughLinesP::setupParameters()
        {
            std::vector<const runtime::Parameter*> parameters;
            
            switch(int(m_dataFlow))
            {
            case(ALLOCATE):
                {
                    m_rhoParameter = new runtime::NumericParameter<runtime::Float64>(RHO);
                    m_rhoParameter->setAccessMode(runtime::Parameter::ACTIVATED_WRITE);
                    m_rhoParameter->setTitle(L_("Distance resolution"));
                    parameters.push_back(m_rhoParameter);
                    
                    m_thetaParameter = new runtime::NumericParameter<runtime::Float64>(THETA);
                    m_thetaParameter->setAccessMode(runtime::Parameter::ACTIVATED_WRITE);
                    m_thetaParameter->setTitle(L_("Angle resolution"));
                    parameters.push_back(m_thetaParameter);
                    
                    m_thresholdParameter = new runtime::NumericParameter<runtime::UInt32>(THRESHOLD);
                    m_thresholdParameter->setAccessMode(runtime::Parameter::ACTIVATED_WRITE);
                    m_thresholdParameter->setTitle(L_("Accumulator threshold"));
                    parameters.push_back(m_thresholdParameter);
                    
                    m_minLineLengthParameter = new runtime::NumericParameter<runtime::Float64>(MIN_LINE_LENGTH);
                    m_minLineLengthParameter->setAccessMode(runtime::Parameter::ACTIVATED_WRITE);
                    m_minLineLengthParameter->setTitle(L_("Minimum line length"));
                    parameters.push_back(m_minLineLengthParameter);
                    
                    m_maxLineGapParameter = new runtime::NumericParameter<runtime::Float64>(MAX_LINE_GAP);
                    m_maxLineGapParameter->setAccessMode(runtime::Parameter::ACTIVATED_WRITE);
                    m_maxLineGapParameter->setTitle(L_("Maximum allowed gap"));
                    parameters.push_back(m_maxLineGapParameter);
                    
                }
                break;
            }
            
            return parameters;
        }
        
        const std::vector<const runtime::Description*> HoughLinesP::setupInputs()
        {
            std::vector<const runtime::Description*> inputs;
            
            switch(int(m_dataFlow))
            {
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
        
        const std::vector<const runtime::Description*> HoughLinesP::setupOutputs()
        {
            std::vector<const runtime::Description*> outputs;
            
            switch(int(m_dataFlow))
            {
            case(ALLOCATE):
                {
                    runtime::MatrixDescription* dst = new runtime::MatrixDescription(DST, runtime::Variant::MATRIX);
                    dst->setTitle(L_("Destination"));
                    dst->setRows(0);
                    dst->setCols(4);
                    outputs.push_back(dst);
                    
                }
                break;
            }
            
            return outputs;
        }
        
        void HoughLinesP::initialize()
        {
            runtime::OperatorKernel::initialize(setupInputs(), setupOutputs(), setupParameters());
        }
        
        void HoughLinesP::execute(runtime::DataProvider & provider)
        {
            switch(int(m_dataFlow))
            {
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
                    double rhoCvData = double(m_rho);
                    double thetaCvData = double(m_theta);
                    int thresholdCvData = int(m_threshold);
                    double minLineLengthCvData = double(m_minLineLength);
                    double maxLineGapCvData = double(m_maxLineGap);
                    
                    cv::HoughLinesP(srcCvData, dstCvData, rhoCvData, thetaCvData, thresholdCvData, minLineLengthCvData, maxLineGapCvData);
                    dstCvData = dstCvData.reshape(1, dstCvData.cols);
                    
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

