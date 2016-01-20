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
            default:
                throw runtime::WrongParameterId(id, *this);
            }
        }
        
        void CascadeClassifier::setParameter(unsigned int id, const runtime::Data& value)
        {
            try
            {
                switch(id)
                {
                case CLASSIFIER:
                    m_classifier = stromx::runtime::data_cast<runtime::File>(value);
                    if (! m_classifier.path().empty())
                        m_cvClassifier->load(m_classifier.path().c_str());
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
        
        const std::vector<const runtime::Parameter*> CascadeClassifier::setupParameters()
        {
            std::vector<const runtime::Parameter*> parameters;
            
            runtime::Parameter* classifier = new runtime::Parameter(CLASSIFIER, runtime::Variant::FILE);
            classifier->setTitle("Classifier");
            classifier->setAccessMode(runtime::Parameter::ACTIVATED_WRITE);
            parameters.push_back(classifier);
            
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
            
            m_cvClassifier->detectMultiScale(srcCvData, dstCvData);
            
            runtime::List* dstCastedData = new runtime::TypedList<cvsupport::Matrix>(dstCvData);
            runtime::DataContainer dstOutContainer = runtime::DataContainer(dstCastedData);
            runtime::Id2DataPair dstOutMapper(DST, dstOutContainer);
            
            provider.sendOutputData(dstOutMapper);
        }
        
    } // cvobjdetect
} // stromx

