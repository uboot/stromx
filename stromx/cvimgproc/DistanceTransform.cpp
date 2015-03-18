#include "stromx/cvimgproc/DistanceTransform.h"

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
        const std::string DistanceTransform::PACKAGE(STROMX_CVIMGPROC_PACKAGE_NAME);
        const runtime::Version DistanceTransform::VERSION(STROMX_CVIMGPROC_VERSION_MAJOR, STROMX_CVIMGPROC_VERSION_MINOR, STROMX_CVIMGPROC_VERSION_PATCH);
        const std::string DistanceTransform::TYPE("DistanceTransform");
        
        DistanceTransform::DistanceTransform()
          : runtime::OperatorKernel(TYPE, PACKAGE, VERSION, setupInitParameters()),
            m_distanceType(0),
            m_maskSize(0),
            m_dataFlow()
        {
        }
        
        const runtime::DataRef DistanceTransform::getParameter(unsigned int id) const
        {
            switch(id)
            {
            case DISTANCE_TYPE:
                return m_distanceType;
            case MASK_SIZE:
                return m_maskSize;
            case DATA_FLOW:
                return m_dataFlow;
            default:
                throw runtime::WrongParameterId(id, *this);
            }
        }
        
        void DistanceTransform::setParameter(unsigned int id, const runtime::Data& value)
        {
            try
            {
                switch(id)
                {
                case DISTANCE_TYPE:
                    {
                        const runtime::Enum & castedValue = runtime::data_cast<runtime::Enum>(value);
                        if(! castedValue.variant().isVariant(runtime::Variant::ENUM))
                        {
                            throw runtime::WrongParameterType(parameter(id), *this);
                        }
                        cvsupport::checkEnumValue(castedValue, m_distanceTypeParameter, *this);
                        m_distanceType = castedValue;
                    }
                    break;
                case MASK_SIZE:
                    {
                        const runtime::Enum & castedValue = runtime::data_cast<runtime::Enum>(value);
                        if(! castedValue.variant().isVariant(runtime::Variant::ENUM))
                        {
                            throw runtime::WrongParameterType(parameter(id), *this);
                        }
                        cvsupport::checkEnumValue(castedValue, m_maskSizeParameter, *this);
                        m_maskSize = castedValue;
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
        
        const std::vector<const runtime::Parameter*> DistanceTransform::setupInitParameters()
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
        
        const std::vector<const runtime::Parameter*> DistanceTransform::setupParameters()
        {
            std::vector<const runtime::Parameter*> parameters;
            
            switch(int(m_dataFlow))
            {
            case(MANUAL):
                {
                    m_distanceTypeParameter = new runtime::EnumParameter(DISTANCE_TYPE);
                    m_distanceTypeParameter->setAccessMode(runtime::Parameter::ACTIVATED_WRITE);
                    m_distanceTypeParameter->setTitle(L_("Distance type"));
                    m_distanceTypeParameter->add(runtime::EnumDescription(runtime::Enum(DIST_L1), L_("L1 distance")));
                    m_distanceTypeParameter->add(runtime::EnumDescription(runtime::Enum(DIST_L2), L_("L2 distance")));
                    m_distanceTypeParameter->add(runtime::EnumDescription(runtime::Enum(DIST_C), L_("C")));
                    parameters.push_back(m_distanceTypeParameter);
                    
                    m_maskSizeParameter = new runtime::EnumParameter(MASK_SIZE);
                    m_maskSizeParameter->setAccessMode(runtime::Parameter::ACTIVATED_WRITE);
                    m_maskSizeParameter->setTitle(L_("Mask size"));
                    m_maskSizeParameter->add(runtime::EnumDescription(runtime::Enum(SIZE_3), L_("3")));
                    m_maskSizeParameter->add(runtime::EnumDescription(runtime::Enum(SIZE_5), L_("5")));
                    m_maskSizeParameter->add(runtime::EnumDescription(runtime::Enum(SIZE_PRECISE), L_("Precise")));
                    parameters.push_back(m_maskSizeParameter);
                    
                }
                break;
            case(ALLOCATE):
                {
                    m_distanceTypeParameter = new runtime::EnumParameter(DISTANCE_TYPE);
                    m_distanceTypeParameter->setAccessMode(runtime::Parameter::ACTIVATED_WRITE);
                    m_distanceTypeParameter->setTitle(L_("Distance type"));
                    m_distanceTypeParameter->add(runtime::EnumDescription(runtime::Enum(DIST_L1), L_("L1 distance")));
                    m_distanceTypeParameter->add(runtime::EnumDescription(runtime::Enum(DIST_L2), L_("L2 distance")));
                    m_distanceTypeParameter->add(runtime::EnumDescription(runtime::Enum(DIST_C), L_("C")));
                    parameters.push_back(m_distanceTypeParameter);
                    
                    m_maskSizeParameter = new runtime::EnumParameter(MASK_SIZE);
                    m_maskSizeParameter->setAccessMode(runtime::Parameter::ACTIVATED_WRITE);
                    m_maskSizeParameter->setTitle(L_("Mask size"));
                    m_maskSizeParameter->add(runtime::EnumDescription(runtime::Enum(SIZE_3), L_("3")));
                    m_maskSizeParameter->add(runtime::EnumDescription(runtime::Enum(SIZE_5), L_("5")));
                    m_maskSizeParameter->add(runtime::EnumDescription(runtime::Enum(SIZE_PRECISE), L_("Precise")));
                    parameters.push_back(m_maskSizeParameter);
                    
                }
                break;
            }
            
            return parameters;
        }
        
        const std::vector<const runtime::Description*> DistanceTransform::setupInputs()
        {
            std::vector<const runtime::Description*> inputs;
            
            switch(int(m_dataFlow))
            {
            case(MANUAL):
                {
                    m_srcDescription = new runtime::Description(SRC, runtime::Variant::MONO_IMAGE);
                    m_srcDescription->setTitle(L_("Source"));
                    inputs.push_back(m_srcDescription);
                    
                    m_dstDescription = new runtime::Description(DST, runtime::Variant::MATRIX);
                    m_dstDescription->setTitle(L_("Destination"));
                    inputs.push_back(m_dstDescription);
                    
                }
                break;
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
        
        const std::vector<const runtime::Description*> DistanceTransform::setupOutputs()
        {
            std::vector<const runtime::Description*> outputs;
            
            switch(int(m_dataFlow))
            {
            case(MANUAL):
                {
                    runtime::Description* dst = new runtime::Description(DST, runtime::Variant::FLOAT_32_MATRIX);
                    dst->setTitle(L_("Destination"));
                    outputs.push_back(dst);
                    
                }
                break;
            case(ALLOCATE):
                {
                    runtime::Description* dst = new runtime::Description(DST, runtime::Variant::FLOAT_32_MATRIX);
                    dst->setTitle(L_("Destination"));
                    outputs.push_back(dst);
                    
                }
                break;
            }
            
            return outputs;
        }
        
        void DistanceTransform::initialize()
        {
            runtime::OperatorKernel::initialize(setupInputs(), setupOutputs(), setupParameters());
        }
        
        void DistanceTransform::execute(runtime::DataProvider & provider)
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
                    runtime::Matrix * dstCastedData = runtime::data_cast<runtime::Matrix>(dstData);
                    
                    unsigned int stride = srcCastedData->cols() * runtime::Matrix::valueSize(runtime::Matrix::FLOAT_32);
                    dstCastedData->initializeMatrix(srcCastedData->rows(), srcCastedData->cols(), stride, dstCastedData->data(), runtime::Matrix::FLOAT_32);
                    
                    cv::Mat srcCvData = cvsupport::getOpenCvMat(*srcCastedData);
                    cv::Mat dstCvData = cvsupport::getOpenCvMat(*dstCastedData);
                    int distanceTypeCvData = convertDistanceType(m_distanceType);
                    int maskSizeCvData = convertMaskSize(m_maskSize);
                    
                    cv::distanceTransform(srcCvData, dstCvData, distanceTypeCvData, maskSizeCvData);
                    
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
                    int distanceTypeCvData = convertDistanceType(m_distanceType);
                    int maskSizeCvData = convertMaskSize(m_maskSize);
                    
                    cv::distanceTransform(srcCvData, dstCvData, distanceTypeCvData, maskSizeCvData);
                    
                    runtime::Matrix* dstCastedData = new cvsupport::Matrix(dstCvData);
                    runtime::DataContainer dstOutContainer = runtime::DataContainer(dstCastedData);
                    runtime::Id2DataPair dstOutMapper(DST, dstOutContainer);
                    
                    provider.sendOutputData(dstOutMapper);
                }
                break;
            }
        }
        
        int DistanceTransform::convertDistanceType(const runtime::Enum & value)
        {
            switch(int(value))
            {
            case DIST_L1:
                return CV_DIST_L1;
            case DIST_L2:
                return CV_DIST_L2;
            case DIST_C:
                return CV_DIST_C;
            default:
                throw runtime::WrongParameterValue(parameter(DISTANCE_TYPE), *this);
            }
        }
        
        int DistanceTransform::convertMaskSize(const runtime::Enum & value)
        {
            switch(int(value))
            {
            case SIZE_3:
                return 3;
            case SIZE_5:
                return 5;
            case SIZE_PRECISE:
                return CV_DIST_MASK_PRECISE;
            default:
                throw runtime::WrongParameterValue(parameter(MASK_SIZE), *this);
            }
        }
        
    } // cvimgproc
} // stromx

