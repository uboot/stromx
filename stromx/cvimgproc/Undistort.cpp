#include "stromx/cvimgproc/Undistort.h"

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
        const std::string Undistort::PACKAGE(STROMX_CVIMGPROC_PACKAGE_NAME);
        const runtime::Version Undistort::VERSION(STROMX_CVIMGPROC_VERSION_MAJOR, STROMX_CVIMGPROC_VERSION_MINOR, STROMX_CVIMGPROC_VERSION_PATCH);
        const std::string Undistort::TYPE("Undistort");
        
        Undistort::Undistort()
          : runtime::OperatorKernel(TYPE, PACKAGE, VERSION, setupInitParameters()),
            m_cameraMatrix(cvsupport::Matrix::eye(3, 3, runtime::Matrix::FLOAT_32)),
            m_distCoeffs(cvsupport::Matrix::zeros(1, 5, runtime::Matrix::FLOAT_32)),
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
        
        const std::vector<const runtime::Parameter*> Undistort::setupInitParameters()
        {
            std::vector<const runtime::Parameter*> parameters;
            
            m_dataFlowParameter = new runtime::EnumParameter(DATA_FLOW);
            m_dataFlowParameter->setAccessMode(runtime::Parameter::NONE_WRITE);
            m_dataFlowParameter->setTitle(L_("Data flow"));
            m_dataFlowParameter->add(runtime::EnumDescription(runtime::Enum(MANUAL), L_("Manual")));
            m_dataFlowParameter->add(runtime::EnumDescription(runtime::Enum(ALLOCATE), L_("Allocate")));
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
        
        const std::vector<const runtime::Description*> Undistort::setupInputs()
        {
            std::vector<const runtime::Description*> inputs;
            
            switch(int(m_dataFlow))
            {
            case(MANUAL):
                {
                    m_srcDescription = new runtime::Description(SRC, runtime::Variant::IMAGE);
                    m_srcDescription->setTitle(L_("Source"));
                    inputs.push_back(m_srcDescription);
                    
                    m_dstDescription = new runtime::Description(DST, runtime::Variant::IMAGE);
                    m_dstDescription->setTitle(L_("Destination"));
                    inputs.push_back(m_dstDescription);
                    
                }
                break;
            case(ALLOCATE):
                {
                    m_srcDescription = new runtime::Description(SRC, runtime::Variant::IMAGE);
                    m_srcDescription->setTitle(L_("Source"));
                    inputs.push_back(m_srcDescription);
                    
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
                    runtime::Description* dst = new runtime::Description(DST, runtime::Variant::IMAGE);
                    dst->setTitle(L_("Destination"));
                    outputs.push_back(dst);
                    
                }
                break;
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
                    
                    if(! srcData->variant().isVariant(m_srcDescription->variant()))
                    {
                        throw runtime::InputError(SRC, *this, "Wrong input data variant.");
                    }
                    if(! dstData->variant().isVariant(m_dstDescription->variant()))
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
                    
                    runtime::DataContainer dstOutContainer = inContainer;
                    runtime::Id2DataPair dstOutMapper(DST, dstOutContainer);
                    
                    provider.sendOutputData(dstOutMapper);
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
                    
                    if(! srcData->variant().isVariant(m_srcDescription->variant()))
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

