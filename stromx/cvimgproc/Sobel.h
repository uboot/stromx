#ifndef STROMX_CVIMGPROC_SOBEL_H
#define STROMX_CVIMGPROC_SOBEL_H

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
        class STROMX_CVIMGPROC_API Sobel : public runtime::OperatorKernel
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
                DELTA,
                DDEPTH,
                KSIZE,
                DX,
                DY,
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
            runtime::Float64 m_delta;
            runtime::UInt32 m_dx;
            runtime::UInt32 m_dy;
            runtime::UInt32 m_ksize;
            runtime::Float64 m_scale;
            runtime::Enum m_dataFlow;
            runtime::EnumParameter* m_ddepthParameter;
            runtime::NumericParameter<runtime::Float64>* m_deltaParameter;
            runtime::Description* m_dstDescription;
            runtime::NumericParameter<runtime::UInt32>* m_dxParameter;
            runtime::NumericParameter<runtime::UInt32>* m_dyParameter;
            runtime::NumericParameter<runtime::UInt32>* m_ksizeParameter;
            runtime::NumericParameter<runtime::Float64>* m_scaleParameter;
            runtime::Description* m_srcDescription;
            runtime::EnumParameter* m_dataFlowParameter;
        };
    } // cvimgproc
} // stromx

#endif // STROMX_CVIMGPROC_SOBEL_H
