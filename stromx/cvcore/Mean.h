#ifndef STROMX_CVCORE_MEAN_H
#define STROMX_CVCORE_MEAN_H

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
        class STROMX_CVCORE_API Mean : public runtime::OperatorKernel
        {
        public:
            enum DataFlowId
            {
                ALLOCATE
            };
            enum ConnectorId
            {
                SRC,
                VALUE
            };
            enum ParameterId
            {
                DATA_FLOW
            };
            Mean();
            virtual OperatorKernel* clone() const { return new Mean; }
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
            
            runtime::Enum m_dataFlow;
            runtime::Description* m_srcDescription;
            runtime::Description* m_valueDescription;
            runtime::EnumParameter* m_dataFlowParameter;
        };
    } // cvcore
} // stromx

#endif // STROMX_CVCORE_MEAN_H
