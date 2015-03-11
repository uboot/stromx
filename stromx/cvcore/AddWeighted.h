#ifndef STROMX_CVCORE_ADDWEIGHTED_H
#define STROMX_CVCORE_ADDWEIGHTED_H

#include "stromx/cvcore/Config.h"
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
    namespace cvcore
    {
        class STROMX_CVCORE_API AddWeighted : public runtime::OperatorKernel
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
                DST,
                SRC_2,
                SRC_1
            };
            enum ParameterId
            {
                DATA_FLOW,
                ALPHA,
                BETA,
                DDEPTH,
                GAMMA
            };
            AddWeighted();
            virtual OperatorKernel* clone() const { return new AddWeighted; }
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
            
            runtime::Float64 m_alpha;
            runtime::Float64 m_beta;
            runtime::Enum m_ddepth;
            runtime::Float64 m_gamma;
            runtime::Enum m_dataFlow;
            runtime::NumericParameter<runtime::Float64>* m_alphaParameter;
            runtime::NumericParameter<runtime::Float64>* m_betaParameter;
            runtime::EnumParameter* m_ddepthParameter;
            runtime::Description* m_dstDescription;
            runtime::NumericParameter<runtime::Float64>* m_gammaParameter;
            runtime::Description* m_src1Description;
            runtime::Description* m_src2Description;
            runtime::EnumParameter* m_dataFlowParameter;
        };
    } // cvcore
} // stromx

#endif // STROMX_CVCORE_ADDWEIGHTED_H
