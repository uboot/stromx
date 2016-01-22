#ifndef STROMX_CVOBJDETECT_CASCADECLASSIFIER_H
#define STROMX_CVOBJDETECT_CASCADECLASSIFIER_H

#include "stromx/cvobjdetect/Config.h"

#include <stromx/runtime/Description.h>
#include <stromx/runtime/File.h>
#include <stromx/runtime/OperatorKernel.h>

namespace cv
{
    class CascadeClassifier;
}

namespace stromx
{
    namespace cvobjdetect
    {
        class STROMX_CVOBJDETECT_API CascadeClassifier : public runtime::OperatorKernel
        {
        public:
            enum ConnectorId
            {
                SRC,
                DST
            };
            
            enum ParameterId
            {
                CLASSIFIER,
                MIN_SIZE_X,
                MIN_SIZE_Y,
                MAX_SIZE_X,
                MAX_SIZE_Y
            };
            
            CascadeClassifier();
            ~CascadeClassifier();
            virtual OperatorKernel* clone() const { return new CascadeClassifier; }
            virtual void setParameter(const unsigned int id, const runtime::Data& value);
            virtual const runtime::DataRef getParameter(const unsigned int id) const;
            virtual void execute(runtime::DataProvider& provider);
            
        private:
            static const std::string PACKAGE;
            static const runtime::Version VERSION;
            static const std::string TYPE;
            
            const std::vector<const runtime::Parameter*> setupParameters();
            const std::vector<const runtime::Description*> setupInputs();
            const std::vector<const runtime::Description*> setupOutputs();
            
            runtime::File m_classifier;
            runtime::Description* m_srcDescription;
            cv::CascadeClassifier* m_cvClassifier;
            runtime::UInt32 m_minSizeX;
            runtime::UInt32 m_minSizeY;
            runtime::UInt32 m_maxSizeX;
            runtime::UInt32 m_maxSizeY;
        };
    } // cvobjdetect
} // stromx

#endif // STROMX_CVOBJDETECT_CASCADECLASSIFIER_H
