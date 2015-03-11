#ifndef STROMX_CVIMGPROC_CORNERSUBPIX_H
#define STROMX_CVIMGPROC_CORNERSUBPIX_H

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
        class STROMX_CVIMGPROC_API CornerSubPix : public runtime::OperatorKernel
        {
        public:
            enum DataFlowId
            {
                IN_PLACE
            };
            enum ConnectorId
            {
                SRC,
                POINT_MATRIX
            };
            enum ParameterId
            {
                WIN_SIZE_Y,
                WIN_SIZE_X,
                DATA_FLOW
            };
            CornerSubPix();
            virtual OperatorKernel* clone() const { return new CornerSubPix; }
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
            
            runtime::UInt32 m_winSizeX;
            runtime::UInt32 m_winSizeY;
            runtime::Enum m_dataFlow;
            runtime::MatrixDescription* m_pointMatrixDescription;
            runtime::Description* m_srcDescription;
            runtime::NumericParameter<runtime::UInt32>* m_winSizeXParameter;
            runtime::NumericParameter<runtime::UInt32>* m_winSizeYParameter;
            runtime::EnumParameter* m_dataFlowParameter;
        };
    } // cvimgproc
} // stromx

#endif // STROMX_CVIMGPROC_CORNERSUBPIX_H
