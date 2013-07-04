#include "stromx/cvimgproc/Erode.h"

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
        const std::string Erode::PACKAGE(STROMX_CVIMGPROC_PACKAGE_NAME);
        const runtime::Version Erode::VERSION(STROMX_CVIMGPROC_VERSION_MAJOR, STROMX_CVIMGPROC_VERSION_MINOR, STROMX_CVIMGPROC_VERSION_PATCH);
        const std::string Erode::TYPE("Erode");
        
        Erode::Erode()
          : runtime::OperatorKernel(TYPE, PACKAGE, VERSION, setupInitParameters()),
            m_iterations(1),
            m_ksizex(3),
            m_ksizey(3),
            m_shape(0),
            m_dataFlow()
        {
        }
        
        const runtime::DataRef Erode::getParameter(unsigned int id) const
        {
            switch(id)
            {
            case ITERATIONS:
                return m_iterations;
            case KSIZEX:
                return m_ksizex;
            case KSIZEY:
                return m_ksizey;
            case SHAPE:
                return m_shape;
            case DATA_FLOW:
                return m_dataFlow;
            default:
                throw runtime::WrongParameterId(id, *this);
            }
        }
        
        void Erode::setParameter(unsigned int id, const runtime::Data& value)
        {
            try
            {
                switch(id)
                {
                case ITERATIONS:
                    {
                        const runtime::UInt32 & castedValue = runtime::data_cast<runtime::UInt32>(value);
                        checkNumericValue(castedValue, m_iterationsParameter, *this);
                        m_iterations = castedValue;
                    }
                    break;
                case KSIZEX:
                    {
                        const runtime::UInt32 & castedValue = runtime::data_cast<runtime::UInt32>(value);
                        checkNumericValue(castedValue, m_ksizexParameter, *this);
                        m_ksizex = castedValue;
                    }
                    break;
                case KSIZEY:
                    {
                        const runtime::UInt32 & castedValue = runtime::data_cast<runtime::UInt32>(value);
                        checkNumericValue(castedValue, m_ksizeyParameter, *this);
                        m_ksizey = castedValue;
                    }
                    break;
                case SHAPE:
                    {
                        const runtime::Enum & castedValue = runtime::data_cast<runtime::Enum>(value);
                        checkEnumValue(castedValue, m_shapeParameter, *this);
                        m_shape = castedValue;
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
        
        const std::vector<const runtime::Parameter*> Erode::setupInitParameters()
        {
            std::vector<const runtime::Parameter*> parameters;
            
            m_dataFlowParameter = new runtime::EnumParameter(DATA_FLOW);
            m_dataFlowParameter->setAccessMode(runtime::Parameter::NONE_WRITE);
            m_dataFlowParameter->setTitle("Data flow");
            m_dataFlowParameter->add(runtime::EnumDescription(runtime::Enum(MANUAL), "Manual"));
            m_dataFlowParameter->add(runtime::EnumDescription(runtime::Enum(ALLOCATE), "Allocate"));
            m_dataFlowParameter->add(runtime::EnumDescription(runtime::Enum(IN_PLACE), "In place"));
            parameters.push_back(m_dataFlowParameter);
            
            return parameters;
        }
        
        const std::vector<const runtime::Parameter*> Erode::setupParameters()
        {
            std::vector<const runtime::Parameter*> parameters;
            
            switch(int(m_dataFlow))
            {
            case(MANUAL):
                {
                    m_ksizexParameter = new runtime::NumericParameter<runtime::UInt32>(KSIZEX);
                    m_ksizexParameter->setAccessMode(runtime::Parameter::ACTIVATED_WRITE);
                    m_ksizexParameter->setTitle("Kernel size X");
                    m_ksizexParameter->setMin(runtime::UInt32(1));
                    parameters.push_back(m_ksizexParameter);
                    
                    m_ksizeyParameter = new runtime::NumericParameter<runtime::UInt32>(KSIZEY);
                    m_ksizeyParameter->setAccessMode(runtime::Parameter::ACTIVATED_WRITE);
                    m_ksizeyParameter->setTitle("Kernel size Y");
                    m_ksizeyParameter->setMin(runtime::UInt32(1));
                    parameters.push_back(m_ksizeyParameter);
                    
                    m_shapeParameter = new runtime::EnumParameter(SHAPE);
                    m_shapeParameter->setAccessMode(runtime::Parameter::ACTIVATED_WRITE);
                    m_shapeParameter->setTitle("Kernel shape");
                    m_shapeParameter->add(runtime::EnumDescription(runtime::Enum(MORPH_RECT), "Rectangle"));
                    m_shapeParameter->add(runtime::EnumDescription(runtime::Enum(MORPH_ELLIPSE), "Ellipse"));
                    m_shapeParameter->add(runtime::EnumDescription(runtime::Enum(MORPH_CROSS), "Cross"));
                    parameters.push_back(m_shapeParameter);
                    
                    m_iterationsParameter = new runtime::NumericParameter<runtime::UInt32>(ITERATIONS);
                    m_iterationsParameter->setAccessMode(runtime::Parameter::ACTIVATED_WRITE);
                    m_iterationsParameter->setTitle("Number of iterations");
                    m_iterationsParameter->setMin(runtime::UInt32(1));
                    parameters.push_back(m_iterationsParameter);
                    
                }
                break;
            case(ALLOCATE):
                {
                    m_ksizexParameter = new runtime::NumericParameter<runtime::UInt32>(KSIZEX);
                    m_ksizexParameter->setAccessMode(runtime::Parameter::ACTIVATED_WRITE);
                    m_ksizexParameter->setTitle("Kernel size X");
                    m_ksizexParameter->setMin(runtime::UInt32(1));
                    parameters.push_back(m_ksizexParameter);
                    
                    m_ksizeyParameter = new runtime::NumericParameter<runtime::UInt32>(KSIZEY);
                    m_ksizeyParameter->setAccessMode(runtime::Parameter::ACTIVATED_WRITE);
                    m_ksizeyParameter->setTitle("Kernel size Y");
                    m_ksizeyParameter->setMin(runtime::UInt32(1));
                    parameters.push_back(m_ksizeyParameter);
                    
                    m_shapeParameter = new runtime::EnumParameter(SHAPE);
                    m_shapeParameter->setAccessMode(runtime::Parameter::ACTIVATED_WRITE);
                    m_shapeParameter->setTitle("Kernel shape");
                    m_shapeParameter->add(runtime::EnumDescription(runtime::Enum(MORPH_RECT), "Rectangle"));
                    m_shapeParameter->add(runtime::EnumDescription(runtime::Enum(MORPH_ELLIPSE), "Ellipse"));
                    m_shapeParameter->add(runtime::EnumDescription(runtime::Enum(MORPH_CROSS), "Cross"));
                    parameters.push_back(m_shapeParameter);
                    
                    m_iterationsParameter = new runtime::NumericParameter<runtime::UInt32>(ITERATIONS);
                    m_iterationsParameter->setAccessMode(runtime::Parameter::ACTIVATED_WRITE);
                    m_iterationsParameter->setTitle("Number of iterations");
                    m_iterationsParameter->setMin(runtime::UInt32(1));
                    parameters.push_back(m_iterationsParameter);
                    
                }
                break;
            case(IN_PLACE):
                {
                    m_ksizexParameter = new runtime::NumericParameter<runtime::UInt32>(KSIZEX);
                    m_ksizexParameter->setAccessMode(runtime::Parameter::ACTIVATED_WRITE);
                    m_ksizexParameter->setTitle("Kernel size X");
                    m_ksizexParameter->setMin(runtime::UInt32(1));
                    parameters.push_back(m_ksizexParameter);
                    
                    m_ksizeyParameter = new runtime::NumericParameter<runtime::UInt32>(KSIZEY);
                    m_ksizeyParameter->setAccessMode(runtime::Parameter::ACTIVATED_WRITE);
                    m_ksizeyParameter->setTitle("Kernel size Y");
                    m_ksizeyParameter->setMin(runtime::UInt32(1));
                    parameters.push_back(m_ksizeyParameter);
                    
                    m_shapeParameter = new runtime::EnumParameter(SHAPE);
                    m_shapeParameter->setAccessMode(runtime::Parameter::ACTIVATED_WRITE);
                    m_shapeParameter->setTitle("Kernel shape");
                    m_shapeParameter->add(runtime::EnumDescription(runtime::Enum(MORPH_RECT), "Rectangle"));
                    m_shapeParameter->add(runtime::EnumDescription(runtime::Enum(MORPH_ELLIPSE), "Ellipse"));
                    m_shapeParameter->add(runtime::EnumDescription(runtime::Enum(MORPH_CROSS), "Cross"));
                    parameters.push_back(m_shapeParameter);
                    
                    m_iterationsParameter = new runtime::NumericParameter<runtime::UInt32>(ITERATIONS);
                    m_iterationsParameter->setAccessMode(runtime::Parameter::ACTIVATED_WRITE);
                    m_iterationsParameter->setTitle("Number of iterations");
                    m_iterationsParameter->setMin(runtime::UInt32(1));
                    parameters.push_back(m_iterationsParameter);
                    
                }
                break;
            }
            
            return parameters;
        }
        
        const std::vector<const runtime::Description*> Erode::setupInputs()
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
            case(IN_PLACE):
                {
                    runtime::Description* src = new runtime::Description(SRC, runtime::DataVariant::IMAGE);
                    src->setTitle("Source");
                    inputs.push_back(src);
                    
                }
                break;
            }
            
            return inputs;
        }
        
        const std::vector<const runtime::Description*> Erode::setupOutputs()
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
            case(IN_PLACE):
                {
                    runtime::Description* src = new runtime::Description(SRC, runtime::DataVariant::IMAGE);
                    src->setTitle("Source");
                    outputs.push_back(src);
                    
                }
                break;
            }
            
            return outputs;
        }
        
        void Erode::initialize()
        {
            runtime::OperatorKernel::initialize(setupInputs(), setupOutputs(), setupParameters());
        }
        
        void Erode::execute(runtime::DataProvider & provider)
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
                        srcData = &writeAccess();
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
                    int ksizexCvData = int(m_ksizex);
                    int ksizeyCvData = int(m_ksizey);
                    int shapeCvData = convertShape(m_shape);
                    int iterationsCvData = int(m_iterations);
                    
                    cv::erode(srcCvData, dstCvData, getStructuringElement(shapeCvData, cv::Size(ksizexCvData, ksizeyCvData)), cv::Point(-1, -1), iterationsCvData);
                    
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
                    int ksizexCvData = int(m_ksizex);
                    int ksizeyCvData = int(m_ksizey);
                    int shapeCvData = convertShape(m_shape);
                    int iterationsCvData = int(m_iterations);
                    
                    cv::erode(srcCvData, dstCvData, getStructuringElement(shapeCvData, cv::Size(ksizexCvData, ksizeyCvData)), cv::Point(-1, -1), iterationsCvData);
                    
                    runtime::Image* dstCastedData = new cvsupport::Image(dstCvData);
                    runtime::DataContainer outContainer = runtime::DataContainer(dstCastedData);
                    runtime::Id2DataPair outputMapper(DST, outContainer);
                    
                    dstCastedData->initializeImage(dstCastedData->width(), dstCastedData->height(), dstCastedData->stride(), dstCastedData->data(), srcCastedData->pixelType());
                    provider.sendOutputData(outputMapper);
                }
                break;
            case(IN_PLACE):
                {
                    runtime::Id2DataPair srcInMapper(SRC);
                    
                    provider.receiveInputData(srcInMapper);
                    
                    runtime::Data* srcData = 0;
                    
                    runtime::DataContainer inContainer = srcInMapper.data();
                    runtime::WriteAccess<> writeAccess(inContainer);
                    srcData = &writeAccess();
                    
                    if(! srcData->variant().isVariant(runtime::DataVariant::IMAGE))
                    {
                        throw runtime::InputError(SRC, *this, "Wrong input data variant.");
                    }
                    
                    runtime::Image * srcCastedData = runtime::data_cast<runtime::Image>(srcData);
                    
                    cv::Mat srcCvData = cvsupport::getOpenCvMat(*srcCastedData);
                    cv::Mat dstCvData = srcCvData;
                    int ksizexCvData = int(m_ksizex);
                    int ksizeyCvData = int(m_ksizey);
                    int shapeCvData = convertShape(m_shape);
                    int iterationsCvData = int(m_iterations);
                    
                    cv::erode(srcCvData, dstCvData, getStructuringElement(shapeCvData, cv::Size(ksizexCvData, ksizeyCvData)), cv::Point(-1, -1), iterationsCvData);
                    
                    runtime::DataContainer outContainer = inContainer;
                    runtime::Id2DataPair outputMapper(SRC, outContainer);
                    
                    provider.sendOutputData(outputMapper);
                }
                break;
            }
        }
        
        int Erode::convertShape(const runtime::Enum & value)
        {
            switch(int(value))
            {
            case MORPH_RECT:
                return cv::MORPH_RECT;
            case MORPH_ELLIPSE:
                return cv::MORPH_ELLIPSE;
            case MORPH_CROSS:
                return cv::MORPH_CROSS;
            default:
                throw runtime::WrongParameterValue(parameter(SHAPE), *this);
            }
        }
        
    }
}

