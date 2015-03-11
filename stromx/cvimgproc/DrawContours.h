#ifndef STROMX_CVIMGPROC_DRAWCONTOURS_H
#define STROMX_CVIMGPROC_DRAWCONTOURS_H

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
        class STROMX_CVIMGPROC_API DrawContours : public runtime::OperatorKernel
        {
        public:
            enum DataFlowId
            {
                IN_PLACE
            };
            enum ConnectorId
            {
                CONTOURS,
                IMG
            };
            enum ParameterId
            {
                CH_1,
                CH_2,
                CH_3,
                DATA_FLOW,
                THICKNESS
            };
            DrawContours();
            virtual OperatorKernel* clone() const { return new DrawContours; }
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
            
            runtime::UInt8 m_ch1;
            runtime::UInt8 m_ch2;
            runtime::UInt8 m_ch3;
            runtime::Int32 m_thickness;
            runtime::Enum m_dataFlow;
            runtime::NumericParameter<runtime::UInt8>* m_ch1Parameter;
            runtime::NumericParameter<runtime::UInt8>* m_ch2Parameter;
            runtime::NumericParameter<runtime::UInt8>* m_ch3Parameter;
            runtime::Description* m_contoursDescription;
            runtime::Description* m_imgDescription;
            runtime::NumericParameter<runtime::Int32>* m_thicknessParameter;
            runtime::EnumParameter* m_dataFlowParameter;
        };
    } // cvimgproc
} // stromx

#endif // STROMX_CVIMGPROC_DRAWCONTOURS_H
