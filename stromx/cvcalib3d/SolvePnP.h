#ifndef STROMX_CVCALIB3D_SOLVEPNP_H
#define STROMX_CVCALIB3D_SOLVEPNP_H

#include "stromx/cvcalib3d/Config.h"
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
    namespace cvcalib3d
    {
        class STROMX_CVCALIB3D_API SolvePnP : public runtime::OperatorKernel
        {
        public:
            enum DataFlowId
            {
                ALLOCATE
            };
            enum ConnectorId
            {
                OBJECT_POINTS,
                RVEC,
                TVEC,
                IMAGE_POINTS
            };
            enum ParameterId
            {
                DIST_COEFFS,
                DATA_FLOW,
                CAMERA_MATRIX
            };
            SolvePnP();
            virtual OperatorKernel* clone() const { return new SolvePnP; }
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
            
            cvsupport::Matrix m_cameraMatrix;
            cvsupport::Matrix m_distCoeffs;
            runtime::Enum m_dataFlow;
            runtime::MatrixParameter* m_cameraMatrixParameter;
            runtime::MatrixParameter* m_distCoeffsParameter;
            runtime::MatrixDescription* m_imagePointsDescription;
            runtime::MatrixDescription* m_objectPointsDescription;
            runtime::MatrixDescription* m_rvecDescription;
            runtime::MatrixDescription* m_tvecDescription;
            runtime::EnumParameter* m_dataFlowParameter;
        };
    } // cvcalib3d
} // stromx

#endif // STROMX_CVCALIB3D_SOLVEPNP_H
