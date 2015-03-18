#include "stromx/cvimgproc/MinEnclosingCircle.h"

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
        const std::string MinEnclosingCircle::PACKAGE(STROMX_CVIMGPROC_PACKAGE_NAME);
        const runtime::Version MinEnclosingCircle::VERSION(STROMX_CVIMGPROC_VERSION_MAJOR, STROMX_CVIMGPROC_VERSION_MINOR, STROMX_CVIMGPROC_VERSION_PATCH);
        const std::string MinEnclosingCircle::TYPE("MinEnclosingCircle");
        
        MinEnclosingCircle::MinEnclosingCircle()
          : runtime::OperatorKernel(TYPE, PACKAGE, VERSION, setupInitParameters()),
            m_dataFlow()
        {
        }
        
        const runtime::DataRef MinEnclosingCircle::getParameter(unsigned int id) const
        {
            switch(id)
            {
            case DATA_FLOW:
                return m_dataFlow;
            default:
                throw runtime::WrongParameterId(id, *this);
            }
        }
        
        void MinEnclosingCircle::setParameter(unsigned int id, const runtime::Data& value)
        {
            try
            {
                switch(id)
                {
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
        
        const std::vector<const runtime::Parameter*> MinEnclosingCircle::setupInitParameters()
        {
            std::vector<const runtime::Parameter*> parameters;
            
            return parameters;
        }
        
        const std::vector<const runtime::Parameter*> MinEnclosingCircle::setupParameters()
        {
            std::vector<const runtime::Parameter*> parameters;
            
            switch(int(m_dataFlow))
            {
            case(ALLOCATE):
                {
                }
                break;
            }
            
            return parameters;
        }
        
        const std::vector<const runtime::Description*> MinEnclosingCircle::setupInputs()
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
        
        const std::vector<const runtime::Description*> MinEnclosingCircle::setupOutputs()
        {
            std::vector<const runtime::Description*> outputs;
            
            switch(int(m_dataFlow))
            {
            case(ALLOCATE):
                {
                    runtime::MatrixDescription* circle = new runtime::MatrixDescription(CIRCLE, runtime::Variant::FLOAT_32_MATRIX);
                    circle->setTitle(L_("Circle"));
                    circle->setRows(1);
                    circle->setCols(3);
                    outputs.push_back(circle);
                    
                }
                break;
            }
            
            return outputs;
        }
        
        void MinEnclosingCircle::initialize()
        {
            runtime::OperatorKernel::initialize(setupInputs(), setupOutputs(), setupParameters());
        }
        
        void MinEnclosingCircle::execute(runtime::DataProvider & provider)
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
                    cv::Mat circleCvData;
                    
                    minEnclosingCircle(pointsCvData, circleCvData);
                    
                    runtime::Matrix* circleCastedData = new cvsupport::Matrix(circleCvData);
                    runtime::DataContainer circleOutContainer = runtime::DataContainer(circleCastedData);
                    runtime::Id2DataPair circleOutMapper(CIRCLE, circleOutContainer);
                    
                    provider.sendOutputData(circleOutMapper);
                }
                break;
            }
        }
        
    } // cvimgproc
} // stromx

