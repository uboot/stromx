#include "stromx/cvimgproc/FloodFill.h"

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
        const std::string FloodFill::PACKAGE(STROMX_CVIMGPROC_PACKAGE_NAME);
        const runtime::Version FloodFill::VERSION(STROMX_CVIMGPROC_VERSION_MAJOR, STROMX_CVIMGPROC_VERSION_MINOR, STROMX_CVIMGPROC_VERSION_PATCH);
        const std::string FloodFill::TYPE("FloodFill");
        
        FloodFill::FloodFill()
          : runtime::OperatorKernel(TYPE, PACKAGE, VERSION, setupInitParameters()),
            m_newVal(),
            m_seedPointX(),
            m_seedPointY(),
            m_dataFlow()
        {
        }
        
        const runtime::DataRef FloodFill::getParameter(unsigned int id) const
        {
            switch(id)
            {
            case NEW_VAL:
                return m_newVal;
            case SEED_POINT_X:
                return m_seedPointX;
            case SEED_POINT_Y:
                return m_seedPointY;
            case DATA_FLOW:
                return m_dataFlow;
            default:
                throw runtime::WrongParameterId(id, *this);
            }
        }
        
        void FloodFill::setParameter(unsigned int id, const runtime::Data& value)
        {
            try
            {
                switch(id)
                {
                case NEW_VAL:
                    {
                        const runtime::Float64 & castedValue = runtime::data_cast<runtime::Float64>(value);
                        if(! castedValue.variant().isVariant(runtime::Variant::FLOAT_64))
                        {
                            throw runtime::WrongParameterType(parameter(id), *this);
                        }
                        cvsupport::checkNumericValue(castedValue, m_newValParameter, *this);
                        m_newVal = castedValue;
                    }
                    break;
                case SEED_POINT_X:
                    {
                        const runtime::UInt32 & castedValue = runtime::data_cast<runtime::UInt32>(value);
                        if(! castedValue.variant().isVariant(runtime::Variant::UINT_32))
                        {
                            throw runtime::WrongParameterType(parameter(id), *this);
                        }
                        cvsupport::checkNumericValue(castedValue, m_seedPointXParameter, *this);
                        m_seedPointX = castedValue;
                    }
                    break;
                case SEED_POINT_Y:
                    {
                        const runtime::UInt32 & castedValue = runtime::data_cast<runtime::UInt32>(value);
                        if(! castedValue.variant().isVariant(runtime::Variant::UINT_32))
                        {
                            throw runtime::WrongParameterType(parameter(id), *this);
                        }
                        cvsupport::checkNumericValue(castedValue, m_seedPointYParameter, *this);
                        m_seedPointY = castedValue;
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
        
        const std::vector<const runtime::Parameter*> FloodFill::setupInitParameters()
        {
            std::vector<const runtime::Parameter*> parameters;
            
            return parameters;
        }
        
        const std::vector<const runtime::Parameter*> FloodFill::setupParameters()
        {
            std::vector<const runtime::Parameter*> parameters;
            
            switch(int(m_dataFlow))
            {
            case(IN_PLACE):
                {
                    m_seedPointXParameter = new runtime::NumericParameter<runtime::UInt32>(SEED_POINT_X);
                    m_seedPointXParameter->setAccessMode(runtime::Parameter::ACTIVATED_WRITE);
                    m_seedPointXParameter->setTitle(L_("Seed point X"));
                    parameters.push_back(m_seedPointXParameter);
                    
                    m_seedPointYParameter = new runtime::NumericParameter<runtime::UInt32>(SEED_POINT_Y);
                    m_seedPointYParameter->setAccessMode(runtime::Parameter::ACTIVATED_WRITE);
                    m_seedPointYParameter->setTitle(L_("Seed point Y"));
                    parameters.push_back(m_seedPointYParameter);
                    
                    m_newValParameter = new runtime::NumericParameter<runtime::Float64>(NEW_VAL);
                    m_newValParameter->setAccessMode(runtime::Parameter::ACTIVATED_WRITE);
                    m_newValParameter->setTitle(L_("New value"));
                    parameters.push_back(m_newValParameter);
                    
                }
                break;
            }
            
            return parameters;
        }
        
        const std::vector<const runtime::Description*> FloodFill::setupInputs()
        {
            std::vector<const runtime::Description*> inputs;
            
            switch(int(m_dataFlow))
            {
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
        
        const std::vector<const runtime::Description*> FloodFill::setupOutputs()
        {
            std::vector<const runtime::Description*> outputs;
            
            switch(int(m_dataFlow))
            {
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
        
        void FloodFill::initialize()
        {
            runtime::OperatorKernel::initialize(setupInputs(), setupOutputs(), setupParameters());
        }
        
        void FloodFill::execute(runtime::DataProvider & provider)
        {
            switch(int(m_dataFlow))
            {
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
                    int seedPointXCvData = int(m_seedPointX);
                    int seedPointYCvData = int(m_seedPointY);
                    double newValCvData = double(m_newVal);
                    
                    cv::floodFill(srcCvData, cv::Point(seedPointXCvData, seedPointYCvData), newValCvData);
                    
                    runtime::DataContainer srcOutContainer = inContainer;
                    runtime::Id2DataPair srcOutMapper(SRC, srcOutContainer);
                    
                    provider.sendOutputData(srcOutMapper);
                }
                break;
            }
        }
        
    } // cvimgproc
} // stromx

