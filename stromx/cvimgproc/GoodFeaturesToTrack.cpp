#include "stromx/cvimgproc/GoodFeaturesToTrack.h"

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
        const std::string GoodFeaturesToTrack::PACKAGE(STROMX_CVIMGPROC_PACKAGE_NAME);
        const runtime::Version GoodFeaturesToTrack::VERSION(STROMX_CVIMGPROC_VERSION_MAJOR, STROMX_CVIMGPROC_VERSION_MINOR, STROMX_CVIMGPROC_VERSION_PATCH);
        const std::string GoodFeaturesToTrack::TYPE("GoodFeaturesToTrack");
        
        GoodFeaturesToTrack::GoodFeaturesToTrack()
          : runtime::OperatorKernel(TYPE, PACKAGE, VERSION, setupInitParameters()),
            m_blockSize(3),
            m_k(1),
            m_maxCorners(10),
            m_minDistance(1.0),
            m_qualityLevel(0.01),
            m_useHarrisDetector(false),
            m_dataFlow()
        {
        }
        
        const runtime::DataRef GoodFeaturesToTrack::getParameter(unsigned int id) const
        {
            switch(id)
            {
            case BLOCK_SIZE:
                return m_blockSize;
            case K:
                return m_k;
            case MAX_CORNERS:
                return m_maxCorners;
            case MIN_DISTANCE:
                return m_minDistance;
            case QUALITY_LEVEL:
                return m_qualityLevel;
            case USE_HARRIS_DETECTOR:
                return m_useHarrisDetector;
            case DATA_FLOW:
                return m_dataFlow;
            default:
                throw runtime::WrongParameterId(id, *this);
            }
        }
        
        void GoodFeaturesToTrack::setParameter(unsigned int id, const runtime::Data& value)
        {
            try
            {
                switch(id)
                {
                case BLOCK_SIZE:
                    {
                        const runtime::UInt32 & castedValue = runtime::data_cast<runtime::UInt32>(value);
                        if(! castedValue.variant().isVariant(runtime::Variant::UINT_32))
                        {
                            throw runtime::WrongParameterType(parameter(id), *this);
                        }
                        cvsupport::checkNumericValue(castedValue, m_blockSizeParameter, *this);
                        if(int(castedValue) % 2 == 0)
                            throw runtime::WrongParameterValue(*m_blockSizeParameter, *this, "Only odd values are allowed");
                        m_blockSize = castedValue;
                    }
                    break;
                case K:
                    {
                        const runtime::Float64 & castedValue = runtime::data_cast<runtime::Float64>(value);
                        if(! castedValue.variant().isVariant(runtime::Variant::FLOAT_64))
                        {
                            throw runtime::WrongParameterType(parameter(id), *this);
                        }
                        cvsupport::checkNumericValue(castedValue, m_kParameter, *this);
                        m_k = castedValue;
                    }
                    break;
                case MAX_CORNERS:
                    {
                        const runtime::UInt32 & castedValue = runtime::data_cast<runtime::UInt32>(value);
                        if(! castedValue.variant().isVariant(runtime::Variant::UINT_32))
                        {
                            throw runtime::WrongParameterType(parameter(id), *this);
                        }
                        cvsupport::checkNumericValue(castedValue, m_maxCornersParameter, *this);
                        m_maxCorners = castedValue;
                    }
                    break;
                case MIN_DISTANCE:
                    {
                        const runtime::Float64 & castedValue = runtime::data_cast<runtime::Float64>(value);
                        if(! castedValue.variant().isVariant(runtime::Variant::FLOAT_64))
                        {
                            throw runtime::WrongParameterType(parameter(id), *this);
                        }
                        cvsupport::checkNumericValue(castedValue, m_minDistanceParameter, *this);
                        m_minDistance = castedValue;
                    }
                    break;
                case QUALITY_LEVEL:
                    {
                        const runtime::Float64 & castedValue = runtime::data_cast<runtime::Float64>(value);
                        if(! castedValue.variant().isVariant(runtime::Variant::FLOAT_64))
                        {
                            throw runtime::WrongParameterType(parameter(id), *this);
                        }
                        cvsupport::checkNumericValue(castedValue, m_qualityLevelParameter, *this);
                        m_qualityLevel = castedValue;
                    }
                    break;
                case USE_HARRIS_DETECTOR:
                    {
                        const runtime::Bool & castedValue = runtime::data_cast<runtime::Bool>(value);
                        if(! castedValue.variant().isVariant(runtime::Variant::BOOL))
                        {
                            throw runtime::WrongParameterType(parameter(id), *this);
                        }
                        m_useHarrisDetector = castedValue;
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
        
        const std::vector<const runtime::Parameter*> GoodFeaturesToTrack::setupInitParameters()
        {
            std::vector<const runtime::Parameter*> parameters;
            
            return parameters;
        }
        
        const std::vector<const runtime::Parameter*> GoodFeaturesToTrack::setupParameters()
        {
            std::vector<const runtime::Parameter*> parameters;
            
            switch(int(m_dataFlow))
            {
            case(ALLOCATE):
                {
                    m_maxCornersParameter = new runtime::NumericParameter<runtime::UInt32>(MAX_CORNERS);
                    m_maxCornersParameter->setAccessMode(runtime::Parameter::ACTIVATED_WRITE);
                    m_maxCornersParameter->setTitle(L_("Maximum number of corners"));
                    parameters.push_back(m_maxCornersParameter);
                    
                    m_qualityLevelParameter = new runtime::NumericParameter<runtime::Float64>(QUALITY_LEVEL);
                    m_qualityLevelParameter->setAccessMode(runtime::Parameter::ACTIVATED_WRITE);
                    m_qualityLevelParameter->setTitle(L_("Minimal accepted quality"));
                    parameters.push_back(m_qualityLevelParameter);
                    
                    m_minDistanceParameter = new runtime::NumericParameter<runtime::Float64>(MIN_DISTANCE);
                    m_minDistanceParameter->setAccessMode(runtime::Parameter::ACTIVATED_WRITE);
                    m_minDistanceParameter->setTitle(L_("Minimal distance between corners"));
                    parameters.push_back(m_minDistanceParameter);
                    
                    m_blockSizeParameter = new runtime::NumericParameter<runtime::UInt32>(BLOCK_SIZE);
                    m_blockSizeParameter->setAccessMode(runtime::Parameter::ACTIVATED_WRITE);
                    m_blockSizeParameter->setTitle(L_("Block size"));
                    m_blockSizeParameter->setMin(runtime::UInt32(1));
                    parameters.push_back(m_blockSizeParameter);
                    
                    m_useHarrisDetectorParameter = new runtime::Parameter(USE_HARRIS_DETECTOR, runtime::Variant::BOOL);
                    m_useHarrisDetectorParameter->setAccessMode(runtime::Parameter::ACTIVATED_WRITE);
                    m_useHarrisDetectorParameter->setTitle(L_("Use Harris detector"));
                    parameters.push_back(m_useHarrisDetectorParameter);
                    
                    m_kParameter = new runtime::NumericParameter<runtime::Float64>(K);
                    m_kParameter->setAccessMode(runtime::Parameter::ACTIVATED_WRITE);
                    m_kParameter->setTitle(L_("Harris parameter"));
                    parameters.push_back(m_kParameter);
                    
                }
                break;
            }
            
            return parameters;
        }
        
        const std::vector<const runtime::Description*> GoodFeaturesToTrack::setupInputs()
        {
            std::vector<const runtime::Description*> inputs;
            
            switch(int(m_dataFlow))
            {
            case(ALLOCATE):
                {
                    m_srcDescription = new runtime::Description(SRC, runtime::Variant::MONO_IMAGE);
                    m_srcDescription->setTitle(L_("Source"));
                    inputs.push_back(m_srcDescription);
                    
                }
                break;
            }
            
            return inputs;
        }
        
        const std::vector<const runtime::Description*> GoodFeaturesToTrack::setupOutputs()
        {
            std::vector<const runtime::Description*> outputs;
            
            switch(int(m_dataFlow))
            {
            case(ALLOCATE):
                {
                    runtime::MatrixDescription* pointMatrix = new runtime::MatrixDescription(POINT_MATRIX, runtime::Variant::FLOAT_32_MATRIX);
                    pointMatrix->setTitle(L_("Point coordinates"));
                    pointMatrix->setRows(0);
                    pointMatrix->setCols(2);
                    outputs.push_back(pointMatrix);
                    
                }
                break;
            }
            
            return outputs;
        }
        
        void GoodFeaturesToTrack::initialize()
        {
            runtime::OperatorKernel::initialize(setupInputs(), setupOutputs(), setupParameters());
        }
        
        void GoodFeaturesToTrack::execute(runtime::DataProvider & provider)
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
                    cv::Mat pointMatrixCvData;
                    int maxCornersCvData = int(m_maxCorners);
                    double qualityLevelCvData = double(m_qualityLevel);
                    double minDistanceCvData = double(m_minDistance);
                    int blockSizeCvData = int(m_blockSize);
                    bool useHarrisDetectorCvData = bool(m_useHarrisDetector);
                    double kCvData = double(m_k);
                    
                    cv::goodFeaturesToTrack(srcCvData, pointMatrixCvData, maxCornersCvData, qualityLevelCvData, minDistanceCvData, cv::noArray(), blockSizeCvData, useHarrisDetectorCvData, kCvData);
                    
                    runtime::Matrix* pointMatrixCastedData = new cvsupport::Matrix(pointMatrixCvData);
                    runtime::DataContainer pointMatrixOutContainer = runtime::DataContainer(pointMatrixCastedData);
                    runtime::Id2DataPair pointMatrixOutMapper(POINT_MATRIX, pointMatrixOutContainer);
                    
                    provider.sendOutputData(pointMatrixOutMapper);
                }
                break;
            }
        }
        
    } // cvimgproc
} // stromx

