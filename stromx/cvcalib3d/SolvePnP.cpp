#include "stromx/cvcalib3d/SolvePnP.h"

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
        const std::string SolvePnP::PACKAGE(STROMX_CVCALIB3D_PACKAGE_NAME);
        const runtime::Version SolvePnP::VERSION(STROMX_CVCALIB3D_VERSION_MAJOR, STROMX_CVCALIB3D_VERSION_MINOR, STROMX_CVCALIB3D_VERSION_PATCH);
        const std::string SolvePnP::TYPE("SolvePnP");
        
        SolvePnP::SolvePnP()
          : runtime::OperatorKernel(TYPE, PACKAGE, VERSION, setupInitParameters()),
            m_cameraMatrix(cvsupport::Matrix::zeros(3, 3, runtime::Matrix::FLOAT_32)),
            m_distCoeffs(cvsupport::Matrix::zeros(1, 5, runtime::Matrix::FLOAT_32)),
            m_dataFlow()
        {
        }
        
        const runtime::DataRef SolvePnP::getParameter(unsigned int id) const
        {
            switch(id)
            {
            case CAMERA_MATRIX:
                return m_cameraMatrix;
            case DIST_COEFFS:
                return m_distCoeffs;
            case DATA_FLOW:
                return m_dataFlow;
            default:
                throw runtime::WrongParameterId(id, *this);
            }
        }
        
        void SolvePnP::setParameter(unsigned int id, const runtime::Data& value)
        {
            try
            {
                switch(id)
                {
                case CAMERA_MATRIX:
                    {
                        const runtime::Matrix & castedValue = runtime::data_cast<runtime::Matrix>(value);
                        if(! castedValue.variant().isVariant(runtime::Variant::FLOAT_MATRIX))
                        {
                            throw runtime::WrongParameterType(parameter(id), *this);
                        }
                        cvsupport::checkMatrixValue(castedValue, m_cameraMatrixParameter, *this);
                        m_cameraMatrix = castedValue;
                    }
                    break;
                case DIST_COEFFS:
                    {
                        const runtime::Matrix & castedValue = runtime::data_cast<runtime::Matrix>(value);
                        if(! castedValue.variant().isVariant(runtime::Variant::FLOAT_MATRIX))
                        {
                            throw runtime::WrongParameterType(parameter(id), *this);
                        }
                        cvsupport::checkMatrixValue(castedValue, m_distCoeffsParameter, *this);
                        m_distCoeffs = castedValue;
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
        
        const std::vector<const runtime::Parameter*> SolvePnP::setupInitParameters()
        {
            std::vector<const runtime::Parameter*> parameters;
            
            return parameters;
        }
        
        const std::vector<const runtime::Parameter*> SolvePnP::setupParameters()
        {
            std::vector<const runtime::Parameter*> parameters;
            
            switch(int(m_dataFlow))
            {
            case(ALLOCATE):
                {
                    m_cameraMatrixParameter = new runtime::MatrixParameter(CAMERA_MATRIX, runtime::Variant::FLOAT_MATRIX);
                    m_cameraMatrixParameter->setAccessMode(runtime::Parameter::ACTIVATED_WRITE);
                    m_cameraMatrixParameter->setTitle(L_("Camera matrix"));
                    m_cameraMatrixParameter->setRows(3);
                    m_cameraMatrixParameter->setCols(3);
                    parameters.push_back(m_cameraMatrixParameter);
                    
                    m_distCoeffsParameter = new runtime::MatrixParameter(DIST_COEFFS, runtime::Variant::FLOAT_MATRIX);
                    m_distCoeffsParameter->setAccessMode(runtime::Parameter::ACTIVATED_WRITE);
                    m_distCoeffsParameter->setTitle(L_("Distortion coefficients"));
                    m_distCoeffsParameter->setRows(1);
                    m_distCoeffsParameter->setCols(5);
                    parameters.push_back(m_distCoeffsParameter);
                    
                }
                break;
            }
            
            return parameters;
        }
        
        const std::vector<const runtime::Description*> SolvePnP::setupInputs()
        {
            std::vector<const runtime::Description*> inputs;
            
            switch(int(m_dataFlow))
            {
            case(ALLOCATE):
                {
                    m_objectPointsDescription = new runtime::MatrixDescription(OBJECT_POINTS, runtime::Variant::FLOAT_32_MATRIX);
                    m_objectPointsDescription->setTitle("Object points");
                    m_objectPointsDescription->setRows(0);
                    m_objectPointsDescription->setCols(3);
                    inputs.push_back(m_objectPointsDescription);
                    
                    m_imagePointsDescription = new runtime::MatrixDescription(IMAGE_POINTS, runtime::Variant::FLOAT_32_MATRIX);
                    m_imagePointsDescription->setTitle("Image points");
                    m_imagePointsDescription->setRows(0);
                    m_imagePointsDescription->setCols(2);
                    inputs.push_back(m_imagePointsDescription);
                    
                }
                break;
            }
            
            return inputs;
        }
        
        const std::vector<const runtime::Description*> SolvePnP::setupOutputs()
        {
            std::vector<const runtime::Description*> outputs;
            
            switch(int(m_dataFlow))
            {
            case(ALLOCATE):
                {
                    runtime::MatrixDescription* rvec = new runtime::MatrixDescription(RVEC, runtime::Variant::FLOAT_64_MATRIX);
                    rvec->setTitle(L_("Rotation"));
                    rvec->setRows(3);
                    rvec->setCols(1);
                    outputs.push_back(rvec);
                    
                    runtime::MatrixDescription* tvec = new runtime::MatrixDescription(TVEC, runtime::Variant::FLOAT_64_MATRIX);
                    tvec->setTitle(L_("Translation"));
                    tvec->setRows(3);
                    tvec->setCols(1);
                    outputs.push_back(tvec);
                    
                }
                break;
            }
            
            return outputs;
        }
        
        void SolvePnP::initialize()
        {
            runtime::OperatorKernel::initialize(setupInputs(), setupOutputs(), setupParameters());
        }
        
        void SolvePnP::execute(runtime::DataProvider & provider)
        {
            switch(int(m_dataFlow))
            {
            case(ALLOCATE):
                {
                    runtime::Id2DataPair objectPointsInMapper(OBJECT_POINTS);
                    runtime::Id2DataPair imagePointsInMapper(IMAGE_POINTS);
                    
                    provider.receiveInputData(objectPointsInMapper && imagePointsInMapper);
                    
                    const runtime::Data* objectPointsData = 0;
                    const runtime::Data* imagePointsData = 0;
                    
                    runtime::ReadAccess<> objectPointsReadAccess;
                    runtime::ReadAccess<> imagePointsReadAccess;
                    
                    objectPointsReadAccess = runtime::ReadAccess<>(objectPointsInMapper.data());
                    objectPointsData = &objectPointsReadAccess();
                    imagePointsReadAccess = runtime::ReadAccess<>(imagePointsInMapper.data());
                    imagePointsData = &imagePointsReadAccess();
                    
                    if(! objectPointsData->variant().isVariant(m_objectPointsDescription->variant()))
                    {
                        throw runtime::InputError(OBJECT_POINTS, *this, "Wrong input data variant.");
                    }
                    if(! imagePointsData->variant().isVariant(m_imagePointsDescription->variant()))
                    {
                        throw runtime::InputError(IMAGE_POINTS, *this, "Wrong input data variant.");
                    }
                    
                    const runtime::Matrix* objectPointsCastedData = runtime::data_cast<runtime::Matrix>(objectPointsData);
                    const runtime::Matrix* imagePointsCastedData = runtime::data_cast<runtime::Matrix>(imagePointsData);
                    cvsupport::checkMatrixValue(*objectPointsCastedData, m_objectPointsDescription, *this);
                    cvsupport::checkMatrixValue(*imagePointsCastedData, m_imagePointsDescription, *this);
                    
                    if (objectPointsCastedData->rows() != imagePointsCastedData->rows())
                    {
                        throw runtime::InputError(OBJECT_POINTS, *this, "Object and image points must have the same number of rows.");
                    }
                    
                    cv::Mat objectPointsCvData = cvsupport::getOpenCvMat(*objectPointsCastedData, 3);
                    cv::Mat imagePointsCvData = cvsupport::getOpenCvMat(*imagePointsCastedData, 2);
                    cv::Mat cameraMatrixCvData = cvsupport::getOpenCvMat(m_cameraMatrix);
                    cv::Mat distCoeffsCvData = cvsupport::getOpenCvMat(m_distCoeffs);
                    cv::Mat rvecCvData;
                    cv::Mat tvecCvData;
                    
                    cv::solvePnP(objectPointsCvData, imagePointsCvData, cameraMatrixCvData, distCoeffsCvData, rvecCvData, tvecCvData);
                    
                    runtime::Matrix* rvecCastedData = new cvsupport::Matrix(rvecCvData);
                    runtime::DataContainer rvecOutContainer = runtime::DataContainer(rvecCastedData);
                    runtime::Id2DataPair rvecOutMapper(RVEC, rvecOutContainer);
                    runtime::Matrix* tvecCastedData = new cvsupport::Matrix(tvecCvData);
                    runtime::DataContainer tvecOutContainer = runtime::DataContainer(tvecCastedData);
                    runtime::Id2DataPair tvecOutMapper(TVEC, tvecOutContainer);
                    
                    provider.sendOutputData(rvecOutMapper && tvecOutMapper);
                }
                break;
            }
        }
        
    } // cvcalib3d
} // stromx

