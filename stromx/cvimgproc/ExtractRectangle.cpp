#include "stromx/cvimgproc/ExtractRectangle.h"

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
        const std::string ExtractRectangle::PACKAGE(STROMX_CVIMGPROC_PACKAGE_NAME);
        const runtime::Version ExtractRectangle::VERSION(STROMX_CVIMGPROC_VERSION_MAJOR, STROMX_CVIMGPROC_VERSION_MINOR, STROMX_CVIMGPROC_VERSION_PATCH);
        const std::string ExtractRectangle::TYPE("ExtractRectangle");
        
        ExtractRectangle::ExtractRectangle()
          : runtime::OperatorKernel(TYPE, PACKAGE, VERSION, setupInitParameters()),
            m_dataFlow()
        {
        }
        
        const runtime::DataRef ExtractRectangle::getParameter(unsigned int id) const
        {
            switch(id)
            {
            case DATA_FLOW:
                return m_dataFlow;
            default:
                throw runtime::WrongParameterId(id, *this);
            }
        }
        
        void ExtractRectangle::setParameter(unsigned int id, const runtime::Data& value)
        {
            try
            {
                switch(id)
                {
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
        
        const std::vector<const runtime::Parameter*> ExtractRectangle::setupInitParameters()
        {
            std::vector<const runtime::Parameter*> parameters;
            
            return parameters;
        }
        
        const std::vector<const runtime::Parameter*> ExtractRectangle::setupParameters()
        {
            std::vector<const runtime::Parameter*> parameters;
            
            switch(int(m_dataFlow))
            {
            case(ALLOCATE):
                {
                }
                break;
            }
            
            return parameters;
        }
        
        const std::vector<const runtime::Description*> ExtractRectangle::setupInputs()
        {
            std::vector<const runtime::Description*> inputs;
            
            switch(int(m_dataFlow))
            {
            case(ALLOCATE):
                {
                    m_srcDescription = new runtime::Description(SRC, runtime::Variant::IMAGE);
                    m_srcDescription->setTitle(L_("Source"));
                    inputs.push_back(m_srcDescription);
                    
                    m_rectDescription = new runtime::MatrixDescription(RECT, runtime::Variant::FLOAT_32_MATRIX);
                    m_rectDescription->setTitle("Rectangle");
                    m_rectDescription->setVisualization(runtime::Variant::ROTATED_RECTANGLE);
                    m_rectDescription->setRows(1);
                    m_rectDescription->setCols(5);
                    inputs.push_back(m_rectDescription);
                    
                }
                break;
            }
            
            return inputs;
        }
        
        const std::vector<const runtime::Description*> ExtractRectangle::setupOutputs()
        {
            std::vector<const runtime::Description*> outputs;
            
            switch(int(m_dataFlow))
            {
            case(ALLOCATE):
                {
                    runtime::Description* dst = new runtime::Description(DST, runtime::Variant::IMAGE);
                    dst->setTitle(L_("Destination"));
                    outputs.push_back(dst);
                    
                }
                break;
            }
            
            return outputs;
        }
        
        void ExtractRectangle::initialize()
        {
            runtime::OperatorKernel::initialize(setupInputs(), setupOutputs(), setupParameters());
        }
        
        void ExtractRectangle::execute(runtime::DataProvider & provider)
        {
            switch(int(m_dataFlow))
            {
            case(ALLOCATE):
                {
                    runtime::Id2DataPair srcInMapper(SRC);
                    runtime::Id2DataPair rectInMapper(RECT);
                    
                    provider.receiveInputData(srcInMapper && rectInMapper);
                    
                    const runtime::Data* srcData = 0;
                    const runtime::Data* rectData = 0;
                    
                    runtime::ReadAccess srcReadAccess;
                    runtime::ReadAccess rectReadAccess;
                    
                    srcReadAccess = runtime::ReadAccess(srcInMapper.data());
                    srcData = &srcReadAccess.get();
                    rectReadAccess = runtime::ReadAccess(rectInMapper.data());
                    rectData = &rectReadAccess.get();
                    
                    if(! srcData->variant().isVariant(m_srcDescription->variant()))
                    {
                        throw runtime::InputError(SRC, *this, "Wrong input data variant.");
                    }
                    if(! rectData->variant().isVariant(m_rectDescription->variant()))
                    {
                        throw runtime::InputError(RECT, *this, "Wrong input data variant.");
                    }
                    
                    const runtime::Image* srcCastedData = runtime::data_cast<runtime::Image>(srcData);
                    const runtime::Matrix* rectCastedData = runtime::data_cast<runtime::Matrix>(rectData);
                    cvsupport::checkMatrixValue(*rectCastedData, m_rectDescription, *this);
                    
                    cv::Mat srcCvData = cvsupport::getOpenCvMat(*srcCastedData);
                    cv::RotatedRect rectCvData = cvsupport::getOpenCvRotatedRect(*rectCastedData);
                    cv::Mat dstCvData;
                    
                    extractRectangle(srcCvData, rectCvData, dstCvData);
                    
                    runtime::Image* dstCastedData = new cvsupport::Image(dstCvData);
                    runtime::DataContainer dstOutContainer = runtime::DataContainer(dstCastedData);
                    runtime::Id2DataPair dstOutMapper(DST, dstOutContainer);
                    
                    dstCastedData->initializeImage(dstCastedData->width(), dstCastedData->height(), dstCastedData->stride(), dstCastedData->data(), srcCastedData->pixelType());
                    provider.sendOutputData(dstOutMapper);
                }
                break;
            }
        }
        
    } // cvimgproc
} // stromx

