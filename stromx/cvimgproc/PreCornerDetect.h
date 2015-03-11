#ifndef STROMX_CVIMGPROC_PRECORNERDETECT_H
#define STROMX_CVIMGPROC_PRECORNERDETECT_H

#include "stromx/cvimgproc/Config.h"
#include <stromx/cvsupport/Matrix.h>
#include <stromx/runtime/Enum.h>
#include <stromx/runtime/EnumParameter.h>
#include <stromx/runtime/List.h>
#include <stromx/runtime/MatrixDescription.h>
#include <stromx/runtime/MatrixParameter.h>
#include <stromx/runtime/NumericParameter.h>
#include <stromx/runtime/OperatorException.h>
#include <stromx/runtime/OperatorKernel.h>
#include <stromx/runtime/Primitive.h>

namespace stromx
{
    namespace cvimgproc
    {
        class STROMX_CVIMGPROC_API PreCornerDetect : public runtime::OperatorKernel
        {
        public:
            enum BorderTypeId
            {
                BORDER_DEFAULT,
                BORDER_CONSTANT,
                BORDER_REFLECT,
                BORDER_REPLICATE
            };
            enum DataFlowId
            {
                MANUAL,
                ALLOCATE
            };
            enum ConnectorId
            {
                SRC,
                DST
            };
            enum ParameterId
            {
                BORDER_TYPE,
                KSIZE,
                DATA_FLOW
            };
            PreCornerDetect();
            virtual OperatorKernel* clone() const { return new PreCornerDetect; }
            virtual void setParameter(const unsigned int id, const runtime::Data& value);
            virtual const runtime::DataRef getParameter(const unsigned int id) const;
            void initialize();
            virtual void execute(runtime::DataProvider& provider);
            
        private:
            static const std::string PACKAGE;
            static const runtime::Version VERSION;
            static const std::string TYPE;
            
            const std::vector<const runtime::Parameter*> setupInitParameters();
            const std::vector<const runtime::Parameter*> setupParameters();
            const std::vector<const runtime::Description*> setupInputs();
            const std::vector<const runtime::Description*> setupOutputs();
            
            int convertBorderType(const runtime::Enum & value);
            
            runtime::Enum m_borderType;
            runtime::UInt32 m_ksize;
            runtime::Enum m_dataFlow;
            runtime::EnumParameter* m_borderTypeParameter;
            runtime::Description* m_dstDescription;
            runtime::NumericParameter<runtime::UInt32>* m_ksizeParameter;
            runtime::Description* m_srcDescription;
            runtime::EnumParameter* m_dataFlowParameter;
        };
    } // cvimgproc
} // stromx

#endif // STROMX_CVIMGPROC_PRECORNERDETECT_H
