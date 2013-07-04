#ifndef STROMX_CVIMGPROC_BILATERALFILTER_H
#define STROMX_CVIMGPROC_BILATERALFILTER_H

#include "stromx/cvimgproc/Config.h"
#include <stromx/cvsupport/Matrix.h>
#include <stromx/runtime/Enum.h>
#include <stromx/runtime/EnumParameter.h>
#include <stromx/runtime/NumericParameter.h>
#include <stromx/runtime/OperatorException.h>
#include <stromx/runtime/OperatorKernel.h>
#include <stromx/runtime/Primitive.h>

namespace stromx
{
    namespace cvimgproc
    {
        class STROMX_CVIMGPROC_API BilateralFilter : public runtime::OperatorKernel
        {
        public:
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
                SIGMA_COLOR,
                D,
                DATA_FLOW,
                SIGMA_SPACE
            };
            BilateralFilter();
            virtual OperatorKernel* clone() const { return new BilateralFilter; }
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
            
            runtime::UInt32 m_d;
            runtime::Double m_sigmaColor;
            runtime::Double m_sigmaSpace;
            runtime::Enum m_dataFlow;
            runtime::NumericParameter<runtime::UInt32>* m_dParameter;
            runtime::NumericParameter<runtime::Double>* m_sigmaColorParameter;
            runtime::NumericParameter<runtime::Double>* m_sigmaSpaceParameter;
            runtime::EnumParameter* m_dataFlowParameter;
        };
    }
}

#endif // STROMX_CVIMGPROC_BILATERALFILTER_H
