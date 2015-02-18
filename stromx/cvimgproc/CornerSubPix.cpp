#include "stromx/cvimgproc/CornerSubPix.h"

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
#include <stromx/runtime/WriteAccess.h>
#include <opencv2/imgproc/imgproc.hpp>

namespace stromx
{
    namespace cvimgproc
    {
        const std::string CornerSubPix::PACKAGE(STROMX_CVIMGPROC_PACKAGE_NAME);
        const runtime::Version CornerSubPix::VERSION(STROMX_CVIMGPROC_VERSION_MAJOR, STROMX_CVIMGPROC_VERSION_MINOR, STROMX_CVIMGPROC_VERSION_PATCH);
        const std::string CornerSubPix::TYPE("CornerSubPix");
        
        CornerSubPix::CornerSubPix()
          : runtime::OperatorKernel(TYPE, PACKAGE, VERSION, setupInitParameters()),
            m_winSizeX(5),
            m_winSizeY(5),
            m_dataFlow()
        {
        }
        
        const runtime::DataRef CornerSubPix::getParameter(unsigned int id) const
        {
            switch(id)
            {
            case WIN_SIZE_X:
                return m_winSizeX;
            case WIN_SIZE_Y:
                return m_winSizeY;
            case DATA_FLOW:
                return m_dataFlow;
            default:
                throw runtime::WrongParameterId(id, *this);
            }
        }
        
        void CornerSubPix::setParameter(unsigned int id, const runtime::Data& value)
        {
            try
            {
                switch(id)
                {
                case WIN_SIZE_X:
                    {
                        const runtime::UInt32 & castedValue = runtime::data_cast<runtime::UInt32>(value);
                        if(! castedValue.variant().isVariant(runtime::DataVariant::UINT_32))
                        {
                            throw runtime::WrongParameterType(parameter(id), *this);
                        }
                        checkNumericValue(castedValue, m_winSizeXParameter, *this);
                        m_winSizeX = castedValue;
                    }
                    break;
                case WIN_SIZE_Y:
                    {
                        const runtime::UInt32 & castedValue = runtime::data_cast<runtime::UInt32>(value);
                        if(! castedValue.variant().isVariant(runtime::DataVariant::UINT_32))
                        {
                            throw runtime::WrongParameterType(parameter(id), *this);
                        }
                        checkNumericValue(castedValue, m_winSizeYParameter, *this);
                        m_winSizeY = castedValue;
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
        
        const std::vector<const runtime::Parameter*> CornerSubPix::setupInitParameters()
        {
            std::vector<const runtime::Parameter*> parameters;
            
            return parameters;
        }
        
        const std::vector<const runtime::Parameter*> CornerSubPix::setupParameters()
        {
            std::vector<const runtime::Parameter*> parameters;
            
            switch(int(m_dataFlow))
            {
            case(IN_PLACE):
                {
                    m_winSizeXParameter = new runtime::NumericParameter<runtime::UInt32>(WIN_SIZE_X);
                    m_winSizeXParameter->setAccessMode(runtime::Parameter::ACTIVATED_WRITE);
                    m_winSizeXParameter->setTitle(L_("Width of search window"));
                    parameters.push_back(m_winSizeXParameter);
                    
                    m_winSizeYParameter = new runtime::NumericParameter<runtime::UInt32>(WIN_SIZE_Y);
                    m_winSizeYParameter->setAccessMode(runtime::Parameter::ACTIVATED_WRITE);
                    m_winSizeYParameter->setTitle(L_("Height of search window"));
                    parameters.push_back(m_winSizeYParameter);
                    
                }
                break;
            }
            
            return parameters;
        }
        
        const std::vector<const runtime::Description*> CornerSubPix::setupInputs()
        {
            std::vector<const runtime::Description*> inputs;
            
            switch(int(m_dataFlow))
            {
            case(IN_PLACE):
                {
                    m_srcDescription = new runtime::Description(SRC, runtime::DataVariant::MONO_IMAGE);
                    m_srcDescription->setTitle(L_("Source"));
                    inputs.push_back(m_srcDescription);
                    
                    m_pointMatrixDescription = new runtime::Description(POINT_MATRIX, runtime::DataVariant::FLOAT_32_MATRIX);
                    m_pointMatrixDescription->setTitle(L_("Point coordinates"));
                    inputs.push_back(m_pointMatrixDescription);
                    
                }
                break;
            }
            
            return inputs;
        }
        
        const std::vector<const runtime::Description*> CornerSubPix::setupOutputs()
        {
            std::vector<const runtime::Description*> outputs;
            
            switch(int(m_dataFlow))
            {
            case(IN_PLACE):
                {
                    runtime::Description* pointMatrix = new runtime::Description(POINT_MATRIX, runtime::DataVariant::FLOAT_32_MATRIX);
                    pointMatrix->setTitle(L_("Point coordinates"));
                    outputs.push_back(pointMatrix);
                    
                }
                break;
            }
            
            return outputs;
        }
        
        void CornerSubPix::initialize()
        {
            runtime::OperatorKernel::initialize(setupInputs(), setupOutputs(), setupParameters());
        }
        
        void CornerSubPix::execute(runtime::DataProvider & provider)
        {
            switch(int(m_dataFlow))
            {
            case(IN_PLACE):
                {
                    runtime::Id2DataPair srcInMapper(SRC);
                    runtime::Id2DataPair pointMatrixInMapper(POINT_MATRIX);
                    
                    provider.receiveInputData(srcInMapper && pointMatrixInMapper);
                    
                    const runtime::Data* srcData = 0;
                    runtime::Data* pointMatrixData = 0;
                    
                    runtime::ReadAccess<> srcReadAccess;
                    runtime::DataContainer inContainer = pointMatrixInMapper.data();
                    runtime::WriteAccess<> writeAccess(inContainer);
                    pointMatrixData = &writeAccess();
                    
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
                    if(! pointMatrixData->variant().isVariant(m_pointMatrixDescription->variant()))
                    {
                        throw runtime::InputError(POINT_MATRIX, *this, "Wrong input data variant.");
                    }
                    
                    const runtime::Image* srcCastedData = runtime::data_cast<runtime::Image>(srcData);
                    runtime::Matrix * pointMatrixCastedData = runtime::data_cast<runtime::Matrix>(pointMatrixData);
                    
                    cv::Mat srcCvData = cvsupport::getOpenCvMat(*srcCastedData);
                    cv::Mat pointMatrixCvData = cvsupport::getOpenCvMat(*pointMatrixCastedData);
                    int winSizeXCvData = int(m_winSizeX);
                    int winSizeYCvData = int(m_winSizeY);
                    
                    cv::cornerSubPix(srcCvData, pointMatrixCvData, cv::Size(winSizeXCvData, winSizeYCvData), cv::Size(-1, -1), cv::TermCriteria(cv::TermCriteria::COUNT + cv::TermCriteria::EPS, -1, -1));
                    
                    runtime::DataContainer outContainer = inContainer;
                    runtime::Id2DataPair outputMapper(POINT_MATRIX, outContainer);
                    
                    provider.sendOutputData(outputMapper);
                }
                break;
            }
        }
        
    }
}

