#include "stromx/cvimgproc/DrawContours.h"

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
        const std::string DrawContours::PACKAGE(STROMX_CVIMGPROC_PACKAGE_NAME);
        const runtime::Version DrawContours::VERSION(STROMX_CVIMGPROC_VERSION_MAJOR, STROMX_CVIMGPROC_VERSION_MINOR, STROMX_CVIMGPROC_VERSION_PATCH);
        const std::string DrawContours::TYPE("DrawContours");
        
        DrawContours::DrawContours()
          : runtime::OperatorKernel(TYPE, PACKAGE, VERSION, setupInitParameters()),
            m_ch1(0),
            m_ch2(0),
            m_ch3(0),
            m_thickness(1),
            m_dataFlow()
        {
        }
        
        const runtime::DataRef DrawContours::getParameter(unsigned int id) const
        {
            switch(id)
            {
            case CH_1:
                return m_ch1;
            case CH_2:
                return m_ch2;
            case CH_3:
                return m_ch3;
            case THICKNESS:
                return m_thickness;
            case DATA_FLOW:
                return m_dataFlow;
            default:
                throw runtime::WrongParameterId(id, *this);
            }
        }
        
        void DrawContours::setParameter(unsigned int id, const runtime::Data& value)
        {
            try
            {
                switch(id)
                {
                case CH_1:
                    {
                        const runtime::UInt8 & castedValue = runtime::data_cast<runtime::UInt8>(value);
                        if(! castedValue.variant().isVariant(runtime::Variant::UINT_8))
                        {
                            throw runtime::WrongParameterType(parameter(id), *this);
                        }
                        cvsupport::checkNumericValue(castedValue, m_ch1Parameter, *this);
                        m_ch1 = castedValue;
                    }
                    break;
                case CH_2:
                    {
                        const runtime::UInt8 & castedValue = runtime::data_cast<runtime::UInt8>(value);
                        if(! castedValue.variant().isVariant(runtime::Variant::UINT_8))
                        {
                            throw runtime::WrongParameterType(parameter(id), *this);
                        }
                        cvsupport::checkNumericValue(castedValue, m_ch2Parameter, *this);
                        m_ch2 = castedValue;
                    }
                    break;
                case CH_3:
                    {
                        const runtime::UInt8 & castedValue = runtime::data_cast<runtime::UInt8>(value);
                        if(! castedValue.variant().isVariant(runtime::Variant::UINT_8))
                        {
                            throw runtime::WrongParameterType(parameter(id), *this);
                        }
                        cvsupport::checkNumericValue(castedValue, m_ch3Parameter, *this);
                        m_ch3 = castedValue;
                    }
                    break;
                case THICKNESS:
                    {
                        const runtime::Int32 & castedValue = runtime::data_cast<runtime::Int32>(value);
                        if(! castedValue.variant().isVariant(runtime::Variant::INT_32))
                        {
                            throw runtime::WrongParameterType(parameter(id), *this);
                        }
                        cvsupport::checkNumericValue(castedValue, m_thicknessParameter, *this);
                        m_thickness = castedValue;
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
        
        const std::vector<const runtime::Parameter*> DrawContours::setupInitParameters()
        {
            std::vector<const runtime::Parameter*> parameters;
            
            return parameters;
        }
        
        const std::vector<const runtime::Parameter*> DrawContours::setupParameters()
        {
            std::vector<const runtime::Parameter*> parameters;
            
            switch(int(m_dataFlow))
            {
            case(IN_PLACE):
                {
                    m_ch1Parameter = new runtime::NumericParameter<runtime::UInt8>(CH_1);
                    m_ch1Parameter->setAccessMode(runtime::Parameter::ACTIVATED_WRITE);
                    m_ch1Parameter->setTitle(L_("Channel 1"));
                    parameters.push_back(m_ch1Parameter);
                    
                    m_ch2Parameter = new runtime::NumericParameter<runtime::UInt8>(CH_2);
                    m_ch2Parameter->setAccessMode(runtime::Parameter::ACTIVATED_WRITE);
                    m_ch2Parameter->setTitle(L_("Channel 2"));
                    parameters.push_back(m_ch2Parameter);
                    
                    m_ch3Parameter = new runtime::NumericParameter<runtime::UInt8>(CH_3);
                    m_ch3Parameter->setAccessMode(runtime::Parameter::ACTIVATED_WRITE);
                    m_ch3Parameter->setTitle(L_("Channel 3"));
                    parameters.push_back(m_ch3Parameter);
                    
                    m_thicknessParameter = new runtime::NumericParameter<runtime::Int32>(THICKNESS);
                    m_thicknessParameter->setAccessMode(runtime::Parameter::ACTIVATED_WRITE);
                    m_thicknessParameter->setTitle(L_("Thickness"));
                    parameters.push_back(m_thicknessParameter);
                    
                }
                break;
            }
            
            return parameters;
        }
        
        const std::vector<const runtime::Description*> DrawContours::setupInputs()
        {
            std::vector<const runtime::Description*> inputs;
            
            switch(int(m_dataFlow))
            {
            case(IN_PLACE):
                {
                    m_imgDescription = new runtime::Description(IMG, runtime::Variant::IMAGE);
                    m_imgDescription->setTitle(L_("Image"));
                    inputs.push_back(m_imgDescription);
                    
                    m_contoursDescription = new runtime::Description(CONTOURS, runtime::Variant::LIST);
                    m_contoursDescription->setTitle(L_("Contours"));
                    inputs.push_back(m_contoursDescription);
                    
                }
                break;
            }
            
            return inputs;
        }
        
        const std::vector<const runtime::Description*> DrawContours::setupOutputs()
        {
            std::vector<const runtime::Description*> outputs;
            
            switch(int(m_dataFlow))
            {
            case(IN_PLACE):
                {
                    runtime::Description* img = new runtime::Description(IMG, runtime::Variant::IMAGE);
                    img->setTitle(L_("Image"));
                    outputs.push_back(img);
                    
                }
                break;
            }
            
            return outputs;
        }
        
        void DrawContours::initialize()
        {
            runtime::OperatorKernel::initialize(setupInputs(), setupOutputs(), setupParameters());
        }
        
        void DrawContours::execute(runtime::DataProvider & provider)
        {
            switch(int(m_dataFlow))
            {
            case(IN_PLACE):
                {
                    runtime::Id2DataPair imgInMapper(IMG);
                    runtime::Id2DataPair contoursInMapper(CONTOURS);
                    
                    provider.receiveInputData(imgInMapper && contoursInMapper);
                    
                    runtime::Data* imgData = 0;
                    const runtime::Data* contoursData = 0;
                    
                    runtime::DataContainer inContainer = imgInMapper.data();
                    runtime::WriteAccess<> writeAccess(inContainer);
                    imgData = &writeAccess();
                    runtime::ReadAccess<> contoursReadAccess;
                    
                    if(contoursInMapper.data() == inContainer)
                    {
                        throw runtime::InputError(CONTOURS, *this, "Can not operate in place.");
                    }
                    else
                    {
                        contoursReadAccess = runtime::ReadAccess<>(contoursInMapper.data());
                        contoursData = &contoursReadAccess();
                    }
                    
                    if(! imgData->variant().isVariant(m_imgDescription->variant()))
                    {
                        throw runtime::InputError(IMG, *this, "Wrong input data variant.");
                    }
                    if(! contoursData->variant().isVariant(m_contoursDescription->variant()))
                    {
                        throw runtime::InputError(CONTOURS, *this, "Wrong input data variant.");
                    }
                    
                    runtime::Image * imgCastedData = runtime::data_cast<runtime::Image>(imgData);
                    const runtime::List* contoursCastedData = runtime::data_cast<runtime::List>(contoursData);
                    
                    cv::Mat imgCvData = cvsupport::getOpenCvMat(*imgCastedData);
                    std::vector<cv::Mat> contoursCvData = cvsupport::getOpenCvMatVector(*contoursCastedData);
                    int ch1CvData = int(m_ch1);
                    int ch2CvData = int(m_ch2);
                    int ch3CvData = int(m_ch3);
                    int thicknessCvData = int(m_thickness);
                    
                    cv::drawContours(imgCvData, contoursCvData, -1, cv::Scalar(ch1CvData, ch2CvData, ch3CvData), thicknessCvData);
                    
                    runtime::DataContainer imgOutContainer = inContainer;
                    runtime::Id2DataPair imgOutMapper(IMG, imgOutContainer);
                    
                    provider.sendOutputData(imgOutMapper);
                }
                break;
            }
        }
        
    } // cvimgproc
} // stromx

