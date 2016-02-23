#include "stromx/cvobjdetect/CascadeClassifier.h"

#include "stromx/cvobjdetect/Locale.h"
#include "stromx/cvobjdetect/Utility.h"
#include <stromx/cvsupport/Image.h>
#include <stromx/cvsupport/Matrix.h>
#include <stromx/cvsupport/Utilities.h>
#include <stromx/runtime/DataContainer.h>
#include <stromx/runtime/DataProvider.h>
#include <stromx/runtime/Id2DataComposite.h>
#include <stromx/runtime/Id2DataPair.h>
#include <stromx/runtime/List.h>
#include <stromx/runtime/NumericParameter.h>
#include <stromx/runtime/ReadAccess.h>

#include <opencv2/objdetect/objdetect.hpp>

namespace stromx
{
    namespace cvobjdetect
    {
        const std::string CascadeClassifier::PACKAGE(STROMX_CVOBJDETECT_PACKAGE_NAME);
        const runtime::Version CascadeClassifier::VERSION(STROMX_CVOBJDETECT_VERSION_MAJOR, STROMX_CVOBJDETECT_VERSION_MINOR, STROMX_CVOBJDETECT_VERSION_PATCH);
        const std::string CascadeClassifier::TYPE("CascadeClassifier");
        
        CascadeClassifier::CascadeClassifier()
          : runtime::OperatorKernel(TYPE, PACKAGE, VERSION, setupInputs(), setupOutputs(), setupParameters())
          , m_cvClassifier(new cv::CascadeClassifier())
        {
        }
        
        CascadeClassifier::~CascadeClassifier()
        {
            delete m_cvClassifier;
        }
            
        const runtime::DataRef CascadeClassifier::getParameter(unsigned int id) const
        {
            switch(id)
            {
            case CLASSIFIER:
                return m_classifier;
            case MIN_SIZE_X:
                return m_minSizeX;
            case MIN_SIZE_Y:
                return m_minSizeY;
            case MAX_SIZE_X:
                return m_maxSizeX;
            case MAX_SIZE_Y:
                return m_maxSizeY;
            default:
                throw runtime::WrongParameterId(id, *this);
            }
        }
        
        void CascadeClassifier::setParameter(unsigned int id, const runtime::Data& value)
        {
            using namespace runtime;
            
            try
            {
                switch(id)
                {
                case CLASSIFIER:
                    m_classifier = data_cast<File>(value);
                    if (! m_classifier.path().empty())
                        m_cvClassifier->load(m_classifier.path().c_str());
                    break;
                case MIN_SIZE_X:
                    m_minSizeX = data_cast<UInt32>(value);
                    break;
                case MIN_SIZE_Y:
                    m_minSizeY = data_cast<UInt32>(value);
                    break;
                case MAX_SIZE_X:
                    m_maxSizeX = data_cast<UInt32>(value);
                    break;
                case MAX_SIZE_Y:
                    m_maxSizeY = data_cast<UInt32>(value);
                    break;
                default:
                    throw WrongParameterId(id, *this);
                }
            }
            catch(BadCast&)
            {
                throw WrongParameterType(parameter(id), *this);
            }
        }
        
        const std::vector<const runtime::Parameter*> CascadeClassifier::setupParameters()
        {
            using namespace runtime;
            std::vector<const Parameter*> parameters;
            
            Parameter* classifier = new Parameter(CLASSIFIER, Variant::TEXT_FILE);
            classifier->setTitle("Classifier");
            classifier->setAccessMode(Parameter::ACTIVATED_WRITE);
            parameters.push_back(classifier);
            
            NumericParameter<UInt32>* minSizeX = new NumericParameter<runtime::UInt32>(MIN_SIZE_X);
            minSizeX->setAccessMode(Parameter::ACTIVATED_WRITE);
            minSizeX->setTitle(L_("Minimum object size X"));
            parameters.push_back(minSizeX);
            
            NumericParameter<UInt32>* minSizeY = new NumericParameter<runtime::UInt32>(MIN_SIZE_Y);
            minSizeY->setAccessMode(Parameter::ACTIVATED_WRITE);
            minSizeY->setTitle(L_("Minimum object size Y"));
            parameters.push_back(minSizeY);
            
            NumericParameter<UInt32>* maxSizeX = new NumericParameter<runtime::UInt32>(MAX_SIZE_X);
            maxSizeX->setAccessMode(Parameter::ACTIVATED_WRITE);
            maxSizeX->setTitle(L_("Maximum object size X"));
            parameters.push_back(maxSizeX);
            
            NumericParameter<UInt32>* maxSizeY = new NumericParameter<runtime::UInt32>(MAX_SIZE_Y);
            maxSizeY->setAccessMode(Parameter::ACTIVATED_WRITE);
            maxSizeY->setTitle(L_("Maximum object size Y"));
            parameters.push_back(maxSizeY);
            
            return parameters;
        }
        
        const std::vector<const runtime::Description*> CascadeClassifier::setupInputs()
        {
            std::vector<const runtime::Description*> inputs;
            
            m_srcDescription = new runtime::Description(SRC, runtime::Variant::MONO_8_IMAGE);
            m_srcDescription->setTitle(L_("Source image"));
            inputs.push_back(m_srcDescription);
            
            return inputs;
        }
        
        const std::vector<const runtime::Description*> CascadeClassifier::setupOutputs()
        {
            std::vector<const runtime::Description*> outputs;
            
            runtime::Description* dst = new runtime::Description(DST, runtime::Variant::LIST);
            dst->setTitle(L_("Detected rectangles"));
            dst->setVisualization(runtime::Variant::RECTANGLE);
            outputs.push_back(dst);
            
            return outputs;
        }
        
        void CascadeClassifier::execute(runtime::DataProvider & provider)
        {
            runtime::Id2DataPair srcInMapper(SRC);
            
            provider.receiveInputData(srcInMapper);
            
            const runtime::Data* srcData = 0;
            
            runtime::ReadAccess srcReadAccess;
            
            srcReadAccess = runtime::ReadAccess(srcInMapper.data());
            srcData = &srcReadAccess.get();
            
            if(! srcData->variant().isVariant(m_srcDescription->variant()))
            {
                throw runtime::InputError(SRC, *this, "Wrong input data variant.");
            }
            
            const runtime::Image* srcCastedData = runtime::data_cast<runtime::Image>(srcData);
            
            cv::Mat srcCvData = cvsupport::getOpenCvMat(*srcCastedData);
            std::vector<cv::Rect> dstCvData;
            
            m_cvClassifier->detectMultiScale(srcCvData, dstCvData, 1.1, 3, 0, 
                                             cv::Size(m_minSizeX, m_minSizeY), 
                                             cv::Size(m_maxSizeX, m_maxSizeY));
            
            runtime::List* dstCastedData = new runtime::TypedList<cvsupport::Matrix>(dstCvData);
            runtime::DataContainer dstOutContainer = runtime::DataContainer(dstCastedData);
            runtime::Id2DataPair dstOutMapper(DST, dstOutContainer);
            
            provider.sendOutputData(dstOutMapper);
        }
        
    } // cvobjdetect
} // stromx

