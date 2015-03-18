#include "stromx/cvimgproc/FitLine.h"

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
        const std::string FitLine::PACKAGE(STROMX_CVIMGPROC_PACKAGE_NAME);
        const runtime::Version FitLine::VERSION(STROMX_CVIMGPROC_VERSION_MAJOR, STROMX_CVIMGPROC_VERSION_MINOR, STROMX_CVIMGPROC_VERSION_PATCH);
        const std::string FitLine::TYPE("FitLine");
        
        FitLine::FitLine()
          : runtime::OperatorKernel(TYPE, PACKAGE, VERSION, setupInitParameters()),
            m_aeps(0.01),
            m_distType(0),
            m_reps(0.01),
            m_dataFlow()
        {
        }
        
        const runtime::DataRef FitLine::getParameter(unsigned int id) const
        {
            switch(id)
            {
            case AEPS:
                return m_aeps;
            case DIST_TYPE:
                return m_distType;
            case REPS:
                return m_reps;
            case DATA_FLOW:
                return m_dataFlow;
            default:
                throw runtime::WrongParameterId(id, *this);
            }
        }
        
        void FitLine::setParameter(unsigned int id, const runtime::Data& value)
        {
            try
            {
                switch(id)
                {
                case AEPS:
                    {
                        const runtime::Float64 & castedValue = runtime::data_cast<runtime::Float64>(value);
                        if(! castedValue.variant().isVariant(runtime::Variant::FLOAT_64))
                        {
                            throw runtime::WrongParameterType(parameter(id), *this);
                        }
                        cvsupport::checkNumericValue(castedValue, m_aepsParameter, *this);
                        m_aeps = castedValue;
                    }
                    break;
                case DIST_TYPE:
                    {
                        const runtime::Enum & castedValue = runtime::data_cast<runtime::Enum>(value);
                        if(! castedValue.variant().isVariant(runtime::Variant::ENUM))
                        {
                            throw runtime::WrongParameterType(parameter(id), *this);
                        }
                        cvsupport::checkEnumValue(castedValue, m_distTypeParameter, *this);
                        m_distType = castedValue;
                    }
                    break;
                case REPS:
                    {
                        const runtime::Float64 & castedValue = runtime::data_cast<runtime::Float64>(value);
                        if(! castedValue.variant().isVariant(runtime::Variant::FLOAT_64))
                        {
                            throw runtime::WrongParameterType(parameter(id), *this);
                        }
                        cvsupport::checkNumericValue(castedValue, m_repsParameter, *this);
                        m_reps = castedValue;
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
        
        const std::vector<const runtime::Parameter*> FitLine::setupInitParameters()
        {
            std::vector<const runtime::Parameter*> parameters;
            
            return parameters;
        }
        
        const std::vector<const runtime::Parameter*> FitLine::setupParameters()
        {
            std::vector<const runtime::Parameter*> parameters;
            
            switch(int(m_dataFlow))
            {
            case(ALLOCATE):
                {
                    m_distTypeParameter = new runtime::EnumParameter(DIST_TYPE);
                    m_distTypeParameter->setAccessMode(runtime::Parameter::ACTIVATED_WRITE);
                    m_distTypeParameter->setTitle(L_("Distance type"));
                    m_distTypeParameter->add(runtime::EnumDescription(runtime::Enum(DIST_L2), L_("L2")));
                    m_distTypeParameter->add(runtime::EnumDescription(runtime::Enum(DIST_L1), L_("L1")));
                    m_distTypeParameter->add(runtime::EnumDescription(runtime::Enum(DIST_L12), L_("L12")));
                    m_distTypeParameter->add(runtime::EnumDescription(runtime::Enum(DIST_FAIR), L_("Fair")));
                    m_distTypeParameter->add(runtime::EnumDescription(runtime::Enum(DIST_WELSCH), L_("Welsch")));
                    m_distTypeParameter->add(runtime::EnumDescription(runtime::Enum(DIST_HUBER), L_("Huber")));
                    parameters.push_back(m_distTypeParameter);
                    
                    m_repsParameter = new runtime::NumericParameter<runtime::Float64>(REPS);
                    m_repsParameter->setAccessMode(runtime::Parameter::ACTIVATED_WRITE);
                    m_repsParameter->setTitle(L_("Accuracy of \u03C1"));
                    m_repsParameter->setMin(runtime::Float64(0.0));
                    parameters.push_back(m_repsParameter);
                    
                    m_aepsParameter = new runtime::NumericParameter<runtime::Float64>(AEPS);
                    m_aepsParameter->setAccessMode(runtime::Parameter::ACTIVATED_WRITE);
                    m_aepsParameter->setTitle(L_("Accuracy of \u03B8"));
                    m_aepsParameter->setMin(runtime::Float64(0.0));
                    parameters.push_back(m_aepsParameter);
                    
                }
                break;
            }
            
            return parameters;
        }
        
        const std::vector<const runtime::Description*> FitLine::setupInputs()
        {
            std::vector<const runtime::Description*> inputs;
            
            switch(int(m_dataFlow))
            {
            case(ALLOCATE):
                {
                    m_pointsDescription = new runtime::MatrixDescription(POINTS, runtime::Variant::INT_32_MATRIX || runtime::Variant::FLOAT_32_MATRIX);
                    m_pointsDescription->setTitle("Point set");
                    m_pointsDescription->setRows(0);
                    m_pointsDescription->setCols(2);
                    inputs.push_back(m_pointsDescription);
                    
                }
                break;
            }
            
            return inputs;
        }
        
        const std::vector<const runtime::Description*> FitLine::setupOutputs()
        {
            std::vector<const runtime::Description*> outputs;
            
            switch(int(m_dataFlow))
            {
            case(ALLOCATE):
                {
                    runtime::MatrixDescription* line = new runtime::MatrixDescription(LINE, runtime::Variant::FLOAT_32_MATRIX);
                    line->setTitle(L_("Line (\u03C1, \u03B8)"));
                    line->setRows(1);
                    line->setCols(3);
                    outputs.push_back(line);
                    
                }
                break;
            }
            
            return outputs;
        }
        
        void FitLine::initialize()
        {
            runtime::OperatorKernel::initialize(setupInputs(), setupOutputs(), setupParameters());
        }
        
        void FitLine::execute(runtime::DataProvider & provider)
        {
            switch(int(m_dataFlow))
            {
            case(ALLOCATE):
                {
                    runtime::Id2DataPair pointsInMapper(POINTS);
                    
                    provider.receiveInputData(pointsInMapper);
                    
                    const runtime::Data* pointsData = 0;
                    
                    runtime::ReadAccess<> pointsReadAccess;
                    
                    pointsReadAccess = runtime::ReadAccess<>(pointsInMapper.data());
                    pointsData = &pointsReadAccess();
                    
                    if(! pointsData->variant().isVariant(m_pointsDescription->variant()))
                    {
                        throw runtime::InputError(POINTS, *this, "Wrong input data variant.");
                    }
                    
                    const runtime::Matrix* pointsCastedData = runtime::data_cast<runtime::Matrix>(pointsData);
                    cvsupport::checkMatrixValue(*pointsCastedData, m_pointsDescription, *this);
                    
                    cv::Mat pointsCvData = cvsupport::getOpenCvMat(*pointsCastedData, 2);
                    cv::Mat lineCvData;
                    int distTypeCvData = convertDistType(m_distType);
                    double repsCvData = double(m_reps);
                    double aepsCvData = double(m_aeps);
                    
                    fitLine(pointsCvData, lineCvData, distTypeCvData, 0, repsCvData, aepsCvData);
                    
                    runtime::Matrix* lineCastedData = new cvsupport::Matrix(lineCvData);
                    runtime::DataContainer lineOutContainer = runtime::DataContainer(lineCastedData);
                    runtime::Id2DataPair lineOutMapper(LINE, lineOutContainer);
                    
                    provider.sendOutputData(lineOutMapper);
                }
                break;
            }
        }
        
        int FitLine::convertDistType(const runtime::Enum & value)
        {
            switch(int(value))
            {
            case DIST_L2:
                return CV_DIST_L2;
            case DIST_L1:
                return CV_DIST_L1;
            case DIST_L12:
                return CV_DIST_L12;
            case DIST_FAIR:
                return CV_DIST_FAIR;
            case DIST_WELSCH:
                return CV_DIST_WELSCH;
            case DIST_HUBER:
                return CV_DIST_HUBER;
            default:
                throw runtime::WrongParameterValue(parameter(DIST_TYPE), *this);
            }
        }
        
    } // cvimgproc
} // stromx

