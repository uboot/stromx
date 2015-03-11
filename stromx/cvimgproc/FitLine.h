#ifndef STROMX_CVIMGPROC_FITLINE_H
#define STROMX_CVIMGPROC_FITLINE_H

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
        class STROMX_CVIMGPROC_API FitLine : public runtime::OperatorKernel
        {
        public:
            enum DistTypeId
            {
                DIST_L2,
                DIST_L1,
                DIST_L12,
                DIST_FAIR,
                DIST_WELSCH,
                DIST_HUBER
            };
            enum DataFlowId
            {
                ALLOCATE
            };
            enum ConnectorId
            {
                LINE,
                POINTS
            };
            enum ParameterId
            {
                AEPS,
                REPS,
                DATA_FLOW,
                DIST_TYPE
            };
            FitLine();
            virtual OperatorKernel* clone() const { return new FitLine; }
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
            
            int convertDistType(const runtime::Enum & value);
            
            runtime::Float64 m_aeps;
            runtime::Enum m_distType;
            runtime::Float64 m_reps;
            runtime::Enum m_dataFlow;
            runtime::NumericParameter<runtime::Float64>* m_aepsParameter;
            runtime::EnumParameter* m_distTypeParameter;
            runtime::MatrixDescription* m_lineDescription;
            runtime::MatrixDescription* m_pointsDescription;
            runtime::NumericParameter<runtime::Float64>* m_repsParameter;
            runtime::EnumParameter* m_dataFlowParameter;
        };
    } // cvimgproc
} // stromx

#endif // STROMX_CVIMGPROC_FITLINE_H
