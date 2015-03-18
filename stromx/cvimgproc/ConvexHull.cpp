#include "stromx/cvimgproc/ConvexHull.h"

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
        const std::string ConvexHull::PACKAGE(STROMX_CVIMGPROC_PACKAGE_NAME);
        const runtime::Version ConvexHull::VERSION(STROMX_CVIMGPROC_VERSION_MAJOR, STROMX_CVIMGPROC_VERSION_MINOR, STROMX_CVIMGPROC_VERSION_PATCH);
        const std::string ConvexHull::TYPE("ConvexHull");
        
        ConvexHull::ConvexHull()
          : runtime::OperatorKernel(TYPE, PACKAGE, VERSION, setupInitParameters()),
            m_clockwise(false),
            m_dataFlow()
        {
        }
        
        const runtime::DataRef ConvexHull::getParameter(unsigned int id) const
        {
            switch(id)
            {
            case CLOCKWISE:
                return m_clockwise;
            case DATA_FLOW:
                return m_dataFlow;
            default:
                throw runtime::WrongParameterId(id, *this);
            }
        }
        
        void ConvexHull::setParameter(unsigned int id, const runtime::Data& value)
        {
            try
            {
                switch(id)
                {
                case CLOCKWISE:
                    {
                        const runtime::Bool & castedValue = runtime::data_cast<runtime::Bool>(value);
                        if(! castedValue.variant().isVariant(runtime::Variant::BOOL))
                        {
                            throw runtime::WrongParameterType(parameter(id), *this);
                        }
                        m_clockwise = castedValue;
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
        
        const std::vector<const runtime::Parameter*> ConvexHull::setupInitParameters()
        {
            std::vector<const runtime::Parameter*> parameters;
            
            return parameters;
        }
        
        const std::vector<const runtime::Parameter*> ConvexHull::setupParameters()
        {
            std::vector<const runtime::Parameter*> parameters;
            
            switch(int(m_dataFlow))
            {
            case(ALLOCATE):
                {
                    m_clockwiseParameter = new runtime::Parameter(CLOCKWISE, runtime::Variant::BOOL);
                    m_clockwiseParameter->setAccessMode(runtime::Parameter::ACTIVATED_WRITE);
                    m_clockwiseParameter->setTitle(L_("Output orientation"));
                    parameters.push_back(m_clockwiseParameter);
                    
                }
                break;
            }
            
            return parameters;
        }
        
        const std::vector<const runtime::Description*> ConvexHull::setupInputs()
        {
            std::vector<const runtime::Description*> inputs;
            
            switch(int(m_dataFlow))
            {
            case(ALLOCATE):
                {
                    m_curveDescription = new runtime::MatrixDescription(CURVE, runtime::Variant::INT_32_MATRIX || runtime::Variant::FLOAT_32_MATRIX);
                    m_curveDescription->setTitle("Input points");
                    m_curveDescription->setRows(0);
                    m_curveDescription->setCols(2);
                    inputs.push_back(m_curveDescription);
                    
                }
                break;
            }
            
            return inputs;
        }
        
        const std::vector<const runtime::Description*> ConvexHull::setupOutputs()
        {
            std::vector<const runtime::Description*> outputs;
            
            switch(int(m_dataFlow))
            {
            case(ALLOCATE):
                {
                    runtime::MatrixDescription* outCurve = new runtime::MatrixDescription(OUT_CURVE, runtime::Variant::INT_32_MATRIX || runtime::Variant::FLOAT_32_MATRIX);
                    outCurve->setTitle(L_("Convex hull"));
                    outCurve->setRows(0);
                    outCurve->setCols(2);
                    outputs.push_back(outCurve);
                    
                }
                break;
            }
            
            return outputs;
        }
        
        void ConvexHull::initialize()
        {
            runtime::OperatorKernel::initialize(setupInputs(), setupOutputs(), setupParameters());
        }
        
        void ConvexHull::execute(runtime::DataProvider & provider)
        {
            switch(int(m_dataFlow))
            {
            case(ALLOCATE):
                {
                    runtime::Id2DataPair curveInMapper(CURVE);
                    
                    provider.receiveInputData(curveInMapper);
                    
                    const runtime::Data* curveData = 0;
                    
                    runtime::ReadAccess<> curveReadAccess;
                    
                    curveReadAccess = runtime::ReadAccess<>(curveInMapper.data());
                    curveData = &curveReadAccess();
                    
                    if(! curveData->variant().isVariant(m_curveDescription->variant()))
                    {
                        throw runtime::InputError(CURVE, *this, "Wrong input data variant.");
                    }
                    
                    const runtime::Matrix* curveCastedData = runtime::data_cast<runtime::Matrix>(curveData);
                    cvsupport::checkMatrixValue(*curveCastedData, m_curveDescription, *this);
                    
                    cv::Mat curveCvData = cvsupport::getOpenCvMat(*curveCastedData, 2);
                    cv::Mat outCurveCvData;
                    bool clockwiseCvData = bool(m_clockwise);
                    
                    cv::convexHull(curveCvData, outCurveCvData, clockwiseCvData);
                    
                    runtime::Matrix* outCurveCastedData = new cvsupport::Matrix(outCurveCvData);
                    runtime::DataContainer outCurveOutContainer = runtime::DataContainer(outCurveCastedData);
                    runtime::Id2DataPair outCurveOutMapper(OUT_CURVE, outCurveOutContainer);
                    
                    provider.sendOutputData(outCurveOutMapper);
                }
                break;
            }
        }
        
    } // cvimgproc
} // stromx

