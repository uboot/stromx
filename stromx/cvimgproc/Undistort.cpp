#include "stromx/cvimgproc/Undistort.h"

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
        const std::string Undistort::PACKAGE(STROMX_CVIMGPROC_PACKAGE_NAME);
        const runtime::Version Undistort::VERSION(STROMX_CVIMGPROC_VERSION_MAJOR, STROMX_CVIMGPROC_VERSION_MINOR, STROMX_CVIMGPROC_VERSION_PATCH);
        const std::string Undistort::TYPE("Undistort");
        
        Undistort::Undistort()
          : runtime::OperatorKernel(TYPE, PACKAGE, VERSION, setupInitParameters()),
            m_cameraMatrix(cvsupport::Matrix::eye(3, 3, runtime::Matrix::FLOAT)),
            m_distCoeffs(cvsupport::Matrix::zeros(4, 1, runtime::Matrix::FLOAT)),
            m_dataFlow()
        {
        }
        
        const runtime::DataRef Undistort::getParameter(unsigned int id) const
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
        
        void Undistort::setParameter(unsigned int id, const runtime::Data& value)
        {
            try
            {
                switch(id)
                {
                case CAMERA_MATRIX:
                    {
                        const runtime::Matrix & castedValue = runtime::data_cast<runtime::Matrix>(value);
                        checkMatrixValue(castedValue, m_cameraMatrixParameter, *this);
                        m_cameraMatrix = castedValue;
                    }
                    break;
                case DIST_COEFFS:
                    {
                        const runtime::Matrix & castedValue = runtime::data_cast<runtime::Matrix>(value);
                        checkMatrixValue(castedValue, m_distCoeffsParameter, *this);
                        m_distCoeffs = castedValue;
                    }
                    break;
                case DATA_FLOW:
                    {
                        const runtime::Enum & castedValue = runtime::data_cast<runtime::Enum>(value);
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
        
        const std::vector<const runtime::Parameter*> Undistort::setupInitParameters()
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
        
        const std::vector<const runtime::Parameter*> Undistort::setupParameters()
        {
            std::vector<const runtime::Parameter*> parameters;
            
            switch(int(m_dataFlow))
            {
            case(MANUAL):
                {
                    m_cameraMatrixParameter = new runtime::MatrixParameter(CAMERA_MATRIX, runtime::DataVariant::MATRIX);
                    m_cameraMatrixParameter->setAccessMode(runtime::Parameter::ACTIVATED_WRITE);
                    m_cameraMatrixParameter->setTitle("3x3 camera matrix");
                    m_cameraMatrixParameter->setRows(3);
                    m_cameraMatrixParameter->setCols(3);
                    parameters.push_back(m_cameraMatrixParameter);
                    
                    m_distCoeffsParameter = new runtime::MatrixParameter(DIST_COEFFS, runtime::DataVariant::MATRIX);
                    m_distCoeffsParameter->setAccessMode(runtime::Parameter::ACTIVATED_WRITE);
                    m_distCoeffsParameter->setTitle("4x1 distortion coefficients");
                    m_distCoeffsParameter->setRows(4);
                    m_distCoeffsParameter->setCols(1);
                    parameters.push_back(m_distCoeffsParameter);
                    
                }
                break;
            case(ALLOCATE):
                {
                    m_cameraMatrixParameter = new runtime::MatrixParameter(CAMERA_MATRIX, runtime::DataVariant::MATRIX);
                    m_cameraMatrixParameter->setAccessMode(runtime::Parameter::ACTIVATED_WRITE);
                    m_cameraMatrixParameter->setTitle("3x3 camera matrix");
                    m_cameraMatrixParameter->setRows(3);
                    m_cameraMatrixParameter->setCols(3);
                    parameters.push_back(m_cameraMatrixParameter);
                    
                    m_distCoeffsParameter = new runtime::MatrixParameter(DIST_COEFFS, runtime::DataVariant::MATRIX);
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
        
        const std::vector<const runtime::Description*> Undistort::setupInputs()
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
        
        const std::vector<const runtime::Description*> Undistort::setupOutputs()
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
        
        void Undistort::initialize()
        {
            runtime::OperatorKernel::initialize(setupInputs(), setupOutputs(), setupParameters());
        }
        
        void Undistort::execute(runtime::DataProvider & provider)
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
                    
                    if(! srcData->variant().isVariant(runtime::DataVariant::IMAGE))
                    {
                        throw runtime::InputError(SRC, *this, "Wrong input data variant.");
                    }
                    if(! dstData->variant().isVariant(runtime::DataVariant::IMAGE))
                    {
                        throw runtime::InputError(DST, *this, "Wrong input data variant.");
                    }
                    
                    const runtime::Image* srcCastedData = runtime::data_cast<runtime::Image>(srcData);
                    runtime::Image * dstCastedData = runtime::data_cast<runtime::Image>(dstData);
                    
                    dstCastedData->initializeImage(srcCastedData->width(), srcCastedData->height(), srcCastedData->stride(), dstCastedData->data(), srcCastedData->pixelType());
                    
                    cv::Mat srcCvData = cvsupport::getOpenCvMat(*srcCastedData);
                    cv::Mat dstCvData = cvsupport::getOpenCvMat(*dstCastedData);
                    cv::Mat cameraMatrixCvData = cvsupport::getOpenCvMat(m_cameraMatrix);
                    cv::Mat distCoeffsCvData = cvsupport::getOpenCvMat(m_distCoeffs);
                    
                    cv::undistort(srcCvData, dstCvData, cameraMatrixCvData, distCoeffsCvData);
                    
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
                    
                    if(! srcData->variant().isVariant(runtime::DataVariant::IMAGE))
                    {
                        throw runtime::InputError(SRC, *this, "Wrong input data variant.");
                    }
                    
                    const runtime::Image* srcCastedData = runtime::data_cast<runtime::Image>(srcData);
                    
                    cv::Mat srcCvData = cvsupport::getOpenCvMat(*srcCastedData);
                    cv::Mat dstCvData;
                    cv::Mat cameraMatrixCvData = cvsupport::getOpenCvMat(m_cameraMatrix);
                    cv::Mat distCoeffsCvData = cvsupport::getOpenCvMat(m_distCoeffs);
                    
                    cv::undistort(srcCvData, dstCvData, cameraMatrixCvData, distCoeffsCvData);
                    
                    runtime::Image* dstCastedData = new cvsupport::Image(dstCvData);
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
