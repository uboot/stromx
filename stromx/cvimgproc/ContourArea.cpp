#include "stromx/cvimgproc/ContourArea.h"

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
        const std::string ContourArea::PACKAGE(STROMX_CVIMGPROC_PACKAGE_NAME);
        const runtime::Version ContourArea::VERSION(STROMX_CVIMGPROC_VERSION_MAJOR, STROMX_CVIMGPROC_VERSION_MINOR, STROMX_CVIMGPROC_VERSION_PATCH);
        const std::string ContourArea::TYPE("ContourArea");
        
        ContourArea::ContourArea()
          : runtime::OperatorKernel(TYPE, PACKAGE, VERSION, setupInitParameters()),
            m_dataFlow()
        {
        }
        
        const runtime::DataRef ContourArea::getParameter(unsigned int id) const
        {
            switch(id)
            {
            case DATA_FLOW:
                return m_dataFlow;
            default:
                throw runtime::WrongParameterId(id, *this);
            }
        }
        
        void ContourArea::setParameter(unsigned int id, const runtime::Data& value)
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
        
        const std::vector<const runtime::Parameter*> ContourArea::setupInitParameters()
        {
            std::vector<const runtime::Parameter*> parameters;
            
            return parameters;
        }
        
        const std::vector<const runtime::Parameter*> ContourArea::setupParameters()
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
        
        const std::vector<const runtime::Description*> ContourArea::setupInputs()
        {
            std::vector<const runtime::Description*> inputs;
            
            switch(int(m_dataFlow))
            {
            case(ALLOCATE):
                {
                    m_contourDescription = new runtime::MatrixDescription(CONTOUR, runtime::Variant::INT_32_MATRIX || runtime::Variant::FLOAT_32_MATRIX);
                    m_contourDescription->setTitle("Input points");
                    m_contourDescription->setVisualization(runtime::Variant::POLYGON);
                    m_contourDescription->setRows(0);
                    m_contourDescription->setCols(2);
                    inputs.push_back(m_contourDescription);
                    
                }
                break;
            }
            
            return inputs;
        }
        
        const std::vector<const runtime::Description*> ContourArea::setupOutputs()
        {
            std::vector<const runtime::Description*> outputs;
            
            switch(int(m_dataFlow))
            {
            case(ALLOCATE):
                {
                    runtime::Description* area = new runtime::Description(AREA, runtime::Variant::FLOAT_64);
                    area->setTitle(L_("Area"));
                    outputs.push_back(area);
                    
                }
                break;
            }
            
            return outputs;
        }
        
        void ContourArea::initialize()
        {
            runtime::OperatorKernel::initialize(setupInputs(), setupOutputs(), setupParameters());
        }
        
        void ContourArea::execute(runtime::DataProvider & provider)
        {
            switch(int(m_dataFlow))
            {
            case(ALLOCATE):
                {
                    runtime::Id2DataPair contourInMapper(CONTOUR);
                    
                    provider.receiveInputData(contourInMapper);
                    
                    const runtime::Data* contourData = 0;
                    
                    runtime::ReadAccess contourReadAccess;
                    
                    contourReadAccess = runtime::ReadAccess(contourInMapper.data());
                    contourData = &contourReadAccess.get();
                    
                    if(! contourData->variant().isVariant(m_contourDescription->variant()))
                    {
                        throw runtime::InputError(CONTOUR, *this, "Wrong input data variant.");
                    }
                    
                    const runtime::Matrix* contourCastedData = runtime::data_cast<runtime::Matrix>(contourData);
                    cvsupport::checkMatrixValue(*contourCastedData, m_contourDescription, *this);
                    
                    cv::Mat contourCvData = cvsupport::getOpenCvMat(*contourCastedData, 2);
                    double areaCvData;
                    
                    areaCvData = cv::contourArea(contourCvData);
                    
                    runtime::Float64* areaCastedData = new runtime::Float64(areaCvData);
                    runtime::DataContainer areaOutContainer = runtime::DataContainer(areaCastedData);
                    runtime::Id2DataPair areaOutMapper(AREA, areaOutContainer);
                    
                    provider.sendOutputData(areaOutMapper);
                }
                break;
            }
        }
        
    } // cvimgproc
} // stromx

