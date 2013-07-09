#ifndef STROMX_CVIMGPROC_LAPLACIAN_H
#define STROMX_CVIMGPROC_LAPLACIAN_H

#include "stromx/cvimgproc/Config.h"
#include <stromx/cvsupport/Matrix.h>
#include <stromx/runtime/Enum.h>
#include <stromx/runtime/EnumParameter.h>
#include <stromx/runtime/MatrixParameter.h>
#include <stromx/runtime/NumericParameter.h>
#include <stromx/runtime/OperatorException.h>
#include <stromx/runtime/OperatorKernel.h>
#include <stromx/runtime/Primitive.h>

namespace stromx
{
    namespace cvimgproc
    {
        class STROMX_CVIMGPROC_API Laplacian : public runtime::OperatorKernel
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
                DATA_FLOW,
                SCALE,
                DDEPTH,
                KSIZE,
                DELTA
            };
            Laplacian();
            virtual OperatorKernel* clone() const { return new Laplacian; }
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
            runtime::Double m_delta;
            runtime::UInt32 m_ksize;
            runtime::Double m_scale;
            runtime::Enum m_dataFlow;
            runtime::EnumParameter* m_ddepthParameter;
            runtime::NumericParameter<runtime::Double>* m_deltaParameter;
            runtime::NumericParameter<runtime::UInt32>* m_ksizeParameter;
            runtime::NumericParameter<runtime::Double>* m_scaleParameter;
            runtime::EnumParameter* m_dataFlowParameter;
        };
    }
}

#endif // STROMX_CVIMGPROC_LAPLACIAN_H
