#ifndef STROMX_IMGPROC_GAUSSIANBLUR_H
#define STROMX_IMGPROC_GAUSSIANBLUR_H

#include "stromx/imgproc/Config.h"
#include <stromx/runtime/Enum.h>
#include <stromx/runtime/EnumParameter.h>
#include <stromx/runtime/NumericParameter.h>
#include <stromx/runtime/OperatorException.h>
#include <stromx/runtime/OperatorKernel.h>
#include <stromx/runtime/Primitive.h>

namespace stromx
{
    namespace imgproc
    {
        class STROMX_IMGPROC_API GaussianBlur : public runtime::OperatorKernel
        {
        public:
            enum DataFlowId
            {
                MANUAL,
                ALLOCATE,
                IN_PLACE
            };
            enum ConnectorId
            {
                SRC,
                DST
            };
            enum ParameterId
            {
                SIGMA_Y,
                SIGMA_X,
                DATA_FLOW,
                KSIZEX,
                KSIZEY
            };
            GaussianBlur();
            virtual OperatorKernel* clone() const { return new GaussianBlur; }
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
            
            runtime::Double m_sigmaY;
            runtime::UInt32 m_ksizey;
            runtime::UInt32 m_ksizex;
            runtime::Double m_sigmaX;
            runtime::Enum m_dataFlow;
            runtime::NumericParameter<runtime::Double>* m_sigmaYParameter;
            runtime::NumericParameter<runtime::UInt32>* m_ksizeyParameter;
            runtime::NumericParameter<runtime::UInt32>* m_ksizexParameter;
            runtime::NumericParameter<runtime::Double>* m_sigmaXParameter;
            runtime::EnumParameter* m_dataFlowParameter;
        };
    }
}

#endif // STROMX_IMGPROC_GAUSSIANBLUR_H
