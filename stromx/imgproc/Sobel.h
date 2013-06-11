#ifndef STROMX_IMGPROC_SOBEL_H
#define STROMX_IMGPROC_SOBEL_H

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
        class STROMX_IMGPROC_API Sobel : public runtime::OperatorKernel
        {
        public:
            enum DdepthId
            {
                SAME,
                DEPTH_8_BIT,
                DEPTH_16_BIT
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
                SCALE,
                DY,
                DDEPTH,
                KSIZE,
                DX,
                DELTA,
                DATA_FLOW
            };
            Sobel();
            virtual OperatorKernel* clone() const { return new Sobel; }
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
            
            int convertDdepth(const runtime::Enum & value);
            
            runtime::Enum m_ddepth;
            runtime::UInt32 m_ksize;
            runtime::UInt32 m_dx;
            runtime::Double m_delta;
            runtime::UInt32 m_dy;
            runtime::Double m_scale;
            runtime::Enum m_dataFlow;
            runtime::EnumParameter* m_ddepthParameter;
            runtime::NumericParameter<runtime::UInt32>* m_ksizeParameter;
            runtime::NumericParameter<runtime::UInt32>* m_dxParameter;
            runtime::NumericParameter<runtime::Double>* m_deltaParameter;
            runtime::NumericParameter<runtime::UInt32>* m_dyParameter;
            runtime::NumericParameter<runtime::Double>* m_scaleParameter;
            runtime::EnumParameter* m_dataFlowParameter;
        };
    }
}

#endif // STROMX_IMGPROC_SOBEL_H
