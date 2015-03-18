#include "stromx/cvimgproc/FindContours.h"

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
        const std::string FindContours::PACKAGE(STROMX_CVIMGPROC_PACKAGE_NAME);
        const runtime::Version FindContours::VERSION(STROMX_CVIMGPROC_VERSION_MAJOR, STROMX_CVIMGPROC_VERSION_MINOR, STROMX_CVIMGPROC_VERSION_PATCH);
        const std::string FindContours::TYPE("FindContours");
        
        FindContours::FindContours()
          : runtime::OperatorKernel(TYPE, PACKAGE, VERSION, setupInitParameters()),
            m_method(0),
            m_mode(0),
            m_dataFlow()
        {
        }
        
        const runtime::DataRef FindContours::getParameter(unsigned int id) const
        {
            switch(id)
            {
            case METHOD:
                return m_method;
            case MODE:
                return m_mode;
            case DATA_FLOW:
                return m_dataFlow;
            default:
                throw runtime::WrongParameterId(id, *this);
            }
        }
        
        void FindContours::setParameter(unsigned int id, const runtime::Data& value)
        {
            try
            {
                switch(id)
                {
                case METHOD:
                    {
                        const runtime::Enum & castedValue = runtime::data_cast<runtime::Enum>(value);
                        if(! castedValue.variant().isVariant(runtime::Variant::ENUM))
                        {
                            throw runtime::WrongParameterType(parameter(id), *this);
                        }
                        cvsupport::checkEnumValue(castedValue, m_methodParameter, *this);
                        m_method = castedValue;
                    }
                    break;
                case MODE:
                    {
                        const runtime::Enum & castedValue = runtime::data_cast<runtime::Enum>(value);
                        if(! castedValue.variant().isVariant(runtime::Variant::ENUM))
                        {
                            throw runtime::WrongParameterType(parameter(id), *this);
                        }
                        cvsupport::checkEnumValue(castedValue, m_modeParameter, *this);
                        m_mode = castedValue;
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
        
        const std::vector<const runtime::Parameter*> FindContours::setupInitParameters()
        {
            std::vector<const runtime::Parameter*> parameters;
            
            return parameters;
        }
        
        const std::vector<const runtime::Parameter*> FindContours::setupParameters()
        {
            std::vector<const runtime::Parameter*> parameters;
            
            switch(int(m_dataFlow))
            {
            case(ALLOCATE):
                {
                    m_modeParameter = new runtime::EnumParameter(MODE);
                    m_modeParameter->setAccessMode(runtime::Parameter::ACTIVATED_WRITE);
                    m_modeParameter->setTitle(L_("Mode"));
                    m_modeParameter->add(runtime::EnumDescription(runtime::Enum(RETR_EXTERNAL), L_("Extreme outer contours")));
                    m_modeParameter->add(runtime::EnumDescription(runtime::Enum(RETR_LIST), L_("All contours")));
                    parameters.push_back(m_modeParameter);
                    
                    m_methodParameter = new runtime::EnumParameter(METHOD);
                    m_methodParameter->setAccessMode(runtime::Parameter::ACTIVATED_WRITE);
                    m_methodParameter->setTitle(L_("Mode"));
                    m_methodParameter->add(runtime::EnumDescription(runtime::Enum(CHAIN_APPROX_NONE), L_("Store all points")));
                    m_methodParameter->add(runtime::EnumDescription(runtime::Enum(CHAIN_APPROX_SIMPLE), L_("Compress straight segments")));
                    m_methodParameter->add(runtime::EnumDescription(runtime::Enum(CHAIN_APPROX_TC89_L1), L_("Teh-Chin L1")));
                    m_methodParameter->add(runtime::EnumDescription(runtime::Enum(CHAIN_APPROX_TC89_KCOS), L_("Teh-Chin Kcos")));
                    parameters.push_back(m_methodParameter);
                    
                }
                break;
            }
            
            return parameters;
        }
        
        const std::vector<const runtime::Description*> FindContours::setupInputs()
        {
            std::vector<const runtime::Description*> inputs;
            
            switch(int(m_dataFlow))
            {
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
        
        const std::vector<const runtime::Description*> FindContours::setupOutputs()
        {
            std::vector<const runtime::Description*> outputs;
            
            switch(int(m_dataFlow))
            {
            case(ALLOCATE):
                {
                    runtime::Description* dst = new runtime::Description(DST, runtime::Variant::LIST);
                    dst->setTitle(L_("Destination"));
                    outputs.push_back(dst);
                    
                }
                break;
            }
            
            return outputs;
        }
        
        void FindContours::initialize()
        {
            runtime::OperatorKernel::initialize(setupInputs(), setupOutputs(), setupParameters());
        }
        
        void FindContours::execute(runtime::DataProvider & provider)
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
                    std::vector<cv::Mat> dstCvData;
                    int modeCvData = convertMode(m_mode);
                    int methodCvData = convertMethod(m_method);
                    
                    cv::findContours(srcCvData, dstCvData, modeCvData, methodCvData);
                    
                    runtime::List* dstCastedData = new runtime::TypedList<cvsupport::Matrix>(dstCvData);
                    runtime::DataContainer dstOutContainer = runtime::DataContainer(dstCastedData);
                    runtime::Id2DataPair dstOutMapper(DST, dstOutContainer);
                    
                    provider.sendOutputData(dstOutMapper);
                }
                break;
            }
        }
        
        int FindContours::convertMethod(const runtime::Enum & value)
        {
            switch(int(value))
            {
            case CHAIN_APPROX_NONE:
                return CV_CHAIN_APPROX_NONE;
            case CHAIN_APPROX_SIMPLE:
                return CV_CHAIN_APPROX_SIMPLE;
            case CHAIN_APPROX_TC89_L1:
                return CV_CHAIN_APPROX_TC89_L1;
            case CHAIN_APPROX_TC89_KCOS:
                return CV_CHAIN_APPROX_TC89_KCOS;
            default:
                throw runtime::WrongParameterValue(parameter(METHOD), *this);
            }
        }
        
        int FindContours::convertMode(const runtime::Enum & value)
        {
            switch(int(value))
            {
            case RETR_EXTERNAL:
                return CV_RETR_EXTERNAL;
            case RETR_LIST:
                return CV_RETR_LIST;
            default:
                throw runtime::WrongParameterValue(parameter(MODE), *this);
            }
        }
        
    } // cvimgproc
} // stromx

