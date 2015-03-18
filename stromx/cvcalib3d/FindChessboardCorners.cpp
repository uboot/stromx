#include "stromx/cvcalib3d/FindChessboardCorners.h"

#include "stromx/cvcalib3d/Locale.h"
#include "stromx/cvcalib3d/Utility.h"
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
#include <opencv2/calib3d/calib3d.hpp>

namespace stromx
{
    namespace cvcalib3d
    {
        const std::string FindChessboardCorners::PACKAGE(STROMX_CVCALIB3D_PACKAGE_NAME);
        const runtime::Version FindChessboardCorners::VERSION(STROMX_CVCALIB3D_VERSION_MAJOR, STROMX_CVCALIB3D_VERSION_MINOR, STROMX_CVCALIB3D_VERSION_PATCH);
        const std::string FindChessboardCorners::TYPE("FindChessboardCorners");
        
        FindChessboardCorners::FindChessboardCorners()
          : runtime::OperatorKernel(TYPE, PACKAGE, VERSION, setupInitParameters()),
            m_patternSizeX(7),
            m_patternSizeY(5),
            m_dataFlow()
        {
        }
        
        const runtime::DataRef FindChessboardCorners::getParameter(unsigned int id) const
        {
            switch(id)
            {
            case PATTERN_SIZE_X:
                return m_patternSizeX;
            case PATTERN_SIZE_Y:
                return m_patternSizeY;
            case DATA_FLOW:
                return m_dataFlow;
            default:
                throw runtime::WrongParameterId(id, *this);
            }
        }
        
        void FindChessboardCorners::setParameter(unsigned int id, const runtime::Data& value)
        {
            try
            {
                switch(id)
                {
                case PATTERN_SIZE_X:
                    {
                        const runtime::UInt32 & castedValue = runtime::data_cast<runtime::UInt32>(value);
                        if(! castedValue.variant().isVariant(runtime::Variant::UINT_32))
                        {
                            throw runtime::WrongParameterType(parameter(id), *this);
                        }
                        cvsupport::checkNumericValue(castedValue, m_patternSizeXParameter, *this);
                        m_patternSizeX = castedValue;
                    }
                    break;
                case PATTERN_SIZE_Y:
                    {
                        const runtime::UInt32 & castedValue = runtime::data_cast<runtime::UInt32>(value);
                        if(! castedValue.variant().isVariant(runtime::Variant::UINT_32))
                        {
                            throw runtime::WrongParameterType(parameter(id), *this);
                        }
                        cvsupport::checkNumericValue(castedValue, m_patternSizeYParameter, *this);
                        m_patternSizeY = castedValue;
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
        
        const std::vector<const runtime::Parameter*> FindChessboardCorners::setupInitParameters()
        {
            std::vector<const runtime::Parameter*> parameters;
            
            return parameters;
        }
        
        const std::vector<const runtime::Parameter*> FindChessboardCorners::setupParameters()
        {
            std::vector<const runtime::Parameter*> parameters;
            
            switch(int(m_dataFlow))
            {
            case(ALLOCATE):
                {
                    m_patternSizeXParameter = new runtime::NumericParameter<runtime::UInt32>(PATTERN_SIZE_X);
                    m_patternSizeXParameter->setAccessMode(runtime::Parameter::ACTIVATED_WRITE);
                    m_patternSizeXParameter->setTitle(L_("Pattern size X"));
                    m_patternSizeXParameter->setMin(runtime::UInt32(1));
                    parameters.push_back(m_patternSizeXParameter);
                    
                    m_patternSizeYParameter = new runtime::NumericParameter<runtime::UInt32>(PATTERN_SIZE_Y);
                    m_patternSizeYParameter->setAccessMode(runtime::Parameter::ACTIVATED_WRITE);
                    m_patternSizeYParameter->setTitle(L_("Pattern size Y"));
                    m_patternSizeYParameter->setMin(runtime::UInt32(1));
                    parameters.push_back(m_patternSizeYParameter);
                    
                }
                break;
            }
            
            return parameters;
        }
        
        const std::vector<const runtime::Description*> FindChessboardCorners::setupInputs()
        {
            std::vector<const runtime::Description*> inputs;
            
            switch(int(m_dataFlow))
            {
            case(ALLOCATE):
                {
                    m_imageDescription = new runtime::Description(IMAGE, runtime::Variant::IMAGE);
                    m_imageDescription->setTitle(L_("Image"));
                    inputs.push_back(m_imageDescription);
                    
                }
                break;
            }
            
            return inputs;
        }
        
        const std::vector<const runtime::Description*> FindChessboardCorners::setupOutputs()
        {
            std::vector<const runtime::Description*> outputs;
            
            switch(int(m_dataFlow))
            {
            case(ALLOCATE):
                {
                    runtime::MatrixDescription* corners = new runtime::MatrixDescription(CORNERS, runtime::Variant::FLOAT_32_MATRIX);
                    corners->setTitle(L_("Corners"));
                    corners->setRows(0);
                    corners->setCols(2);
                    outputs.push_back(corners);
                    
                }
                break;
            }
            
            return outputs;
        }
        
        void FindChessboardCorners::initialize()
        {
            runtime::OperatorKernel::initialize(setupInputs(), setupOutputs(), setupParameters());
        }
        
        void FindChessboardCorners::execute(runtime::DataProvider & provider)
        {
            switch(int(m_dataFlow))
            {
            case(ALLOCATE):
                {
                    runtime::Id2DataPair imageInMapper(IMAGE);
                    
                    provider.receiveInputData(imageInMapper);
                    
                    const runtime::Data* imageData = 0;
                    
                    runtime::ReadAccess<> imageReadAccess;
                    
                    imageReadAccess = runtime::ReadAccess<>(imageInMapper.data());
                    imageData = &imageReadAccess();
                    
                    if(! imageData->variant().isVariant(m_imageDescription->variant()))
                    {
                        throw runtime::InputError(IMAGE, *this, "Wrong input data variant.");
                    }
                    
                    const runtime::Image* imageCastedData = runtime::data_cast<runtime::Image>(imageData);
                    
                    cv::Mat imageCvData = cvsupport::getOpenCvMat(*imageCastedData);
                    int patternSizeXCvData = int(m_patternSizeX);
                    int patternSizeYCvData = int(m_patternSizeY);
                    cv::Mat cornersCvData;
                    
                    cv::findChessboardCorners(imageCvData, cv::Size(patternSizeXCvData, patternSizeYCvData), cornersCvData);
                    
                    runtime::Matrix* cornersCastedData = new cvsupport::Matrix(cornersCvData);
                    runtime::DataContainer cornersOutContainer = runtime::DataContainer(cornersCastedData);
                    runtime::Id2DataPair cornersOutMapper(CORNERS, cornersOutContainer);
                    
                    provider.sendOutputData(cornersOutMapper);
                }
                break;
            }
        }
        
    } // cvcalib3d
} // stromx

