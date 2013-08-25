#include "stromx/cvimgproc/UndistortPoints.h"

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
        const std::string UndistortPoints::PACKAGE(STROMX_CVIMGPROC_PACKAGE_NAME);
        const runtime::Version UndistortPoints::VERSION(STROMX_CVIMGPROC_VERSION_MAJOR, STROMX_CVIMGPROC_VERSION_MINOR, STROMX_CVIMGPROC_VERSION_PATCH);
        const std::string UndistortPoints::TYPE("UndistortPoints");
        
        UndistortPoints::UndistortPoints()
          : runtime::OperatorKernel(TYPE, PACKAGE, VERSION, setupInitParameters()),
            m_cameraMatrix(cvsupport::Matrix::eye(3, 3, runtime::Matrix::FLOAT_32)),
            m_distCoeffs(cvsupport::Matrix::zeros(4, 1, runtime::Matrix::FLOAT_32)),
            m_dataFlow()
        {
        }
        
        const runtime::DataRef UndistortPoints::getParameter(unsigned int id) const
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
        
        void UndistortPoints::setParameter(unsigned int id, const runtime::Data& value)
        {
            try
            {
                switch(id)
                {
                case CAMERA_MATRIX:
                    {
                        const runtime::Matrix & castedValue = runtime::data_cast<runtime::Matrix>(value);
                        if(! castedValue.variant().isVariant(runtime::DataVariant::FLOAT_MATRIX))
                        {
                            throw runtime::WrongParameterType(parameter(id), *this);
                        }
                        checkMatrixValue(castedValue, m_cameraMatrixParameter, *this);
                        m_cameraMatrix = castedValue;
                    }
                    break;
                case DIST_COEFFS:
                    {
                        const runtime::Matrix & castedValue = runtime::data_cast<runtime::Matrix>(value);
                        if(! castedValue.variant().isVariant(runtime::DataVariant::FLOAT_MATRIX))
                        {
                            throw runtime::WrongParameterType(parameter(id), *this);
                        }
                        checkMatrixValue(castedValue, m_distCoeffsParameter, *this);
                        m_distCoeffs = castedValue;
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
        
        const std::vector<const runtime::Parameter*> UndistortPoints::setupInitParameters()
        {
            std::vector<const runtime::Parameter*> parameters;
            
            m_dataFlowParameter = new runtime::EnumParameter(DATA_FLOW);
            m_dataFlowParameter->setAccessMode(runtime::Parameter::NONE_WRITE);
            m_dataFlowParameter->setTitle("Data flow");
            m_dataFlowParameter->add(runtime::EnumDescription(runtime::Enum(ALLOCATE), "Allocate"));
            parameters.push_back(m_dataFlowParameter);
            
            return parameters;
        }
        
        const std::vector<const runtime::Parameter*> UndistortPoints::setupParameters()
        {
            std::vector<const runtime::Parameter*> parameters;
            
            switch(int(m_dataFlow))
            {
            case(ALLOCATE):
                {
                    m_cameraMatrixParameter = new runtime::MatrixParameter(CAMERA_MATRIX, runtime::DataVariant::FLOAT_MATRIX);
                    m_cameraMatrixParameter->setAccessMode(runtime::Parameter::ACTIVATED_WRITE);
                    m_cameraMatrixParameter->setTitle("3x3 camera matrix");
                    m_cameraMatrixParameter->setRows(3);
                    m_cameraMatrixParameter->setCols(3);
                    parameters.push_back(m_cameraMatrixParameter);
                    
                    m_distCoeffsParameter = new runtime::MatrixParameter(DIST_COEFFS, runtime::DataVariant::FLOAT_MATRIX);
                    m_distCoeffsParameter->setAccessMode(runtime::Parameter::ACTIVATED_WRITE);
                    m_distCoeffsParameter->setTitle("4x1 distortion coefficients");
                    m_distCoeffsParameter->setRows(4);
                    m_distCoeffsParameter->setCols(1);
                    parameters.push_back(m_distCoeffsParameter);
                    
                }
                break;
            }
            
            return parameters;
        }
        
        const std::vector<const runtime::Description*> UndistortPoints::setupInputs()
        {
            std::vector<const runtime::Description*> inputs;
            
            switch(int(m_dataFlow))
            {
            case(ALLOCATE):
                {
                    m_srcDescription = new runtime::MatrixDescription(SRC, runtime::DataVariant::FLOAT_MATRIX);
                    m_srcDescription->setTitle("Source");
                    m_srcDescription->setRows(0);
                    m_srcDescription->setCols(2);
                    inputs.push_back(m_srcDescription);
                    
                }
                break;
            }
            
            return inputs;
        }
        
        const std::vector<const runtime::Description*> UndistortPoints::setupOutputs()
        {
            std::vector<const runtime::Description*> outputs;
            
            switch(int(m_dataFlow))
            {
            case(ALLOCATE):
                {
                    runtime::Description* dst = new runtime::Description(DST, runtime::DataVariant::FLOAT_MATRIX);
                    dst->setTitle("Destination");
                    outputs.push_back(dst);
                    
                }
                break;
            }
            
            return outputs;
        }
        
        void UndistortPoints::initialize()
        {
            runtime::OperatorKernel::initialize(setupInputs(), setupOutputs(), setupParameters());
        }
        
        void UndistortPoints::execute(runtime::DataProvider & provider)
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
                    
                    if(! srcData->variant().isVariant(runtime::DataVariant::FLOAT_MATRIX))
                    {
                        throw runtime::InputError(SRC, *this, "Wrong input data variant.");
                    }
                    
                    const runtime::Matrix* srcCastedData = runtime::data_cast<runtime::Matrix>(srcData);
                    checkMatrixData(*srcCastedData, m_srcDescription, *this);
                    
                    cv::Mat srcCvData = cvsupport::getOpenCvMat(*srcCastedData, 2);
                    cv::Mat dstCvData;
                    cv::Mat cameraMatrixCvData = cvsupport::getOpenCvMat(m_cameraMatrix);
                    cv::Mat distCoeffsCvData = cvsupport::getOpenCvMat(m_distCoeffs);
                    
                    cv::undistortPoints(srcCvData, dstCvData, cameraMatrixCvData, distCoeffsCvData);
                    
                    runtime::Matrix* dstCastedData = new cvsupport::Matrix(dstCvData);
                    runtime::DataContainer outContainer = runtime::DataContainer(dstCastedData);
                    runtime::Id2DataPair outputMapper(DST, outContainer);
                    
                    provider.sendOutputData(outputMapper);
                }
                break;
            }
        }
        
    }
}

