#ifndef STROMX_CVIMGPROC_GOODFEATURESTOTRACK_H
#define STROMX_CVIMGPROC_GOODFEATURESTOTRACK_H

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
        class STROMX_CVIMGPROC_API GoodFeaturesToTrack : public runtime::OperatorKernel
        {
        public:
            enum DataFlowId
            {
                ALLOCATE
            };
            enum ConnectorId
            {
                SRC,
                POINT_MATRIX
            };
            enum ParameterId
            {
                MAX_CORNERS,
                K,
                DATA_FLOW,
                QUALITY_LEVEL,
                BLOCK_SIZE,
                USE_HARRIS_DETECTOR,
                MIN_DISTANCE
            };
            GoodFeaturesToTrack();
            virtual OperatorKernel* clone() const { return new GoodFeaturesToTrack; }
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
            
            runtime::UInt32 m_blockSize;
            runtime::Float64 m_k;
            runtime::UInt32 m_maxCorners;
            runtime::Float64 m_minDistance;
            runtime::Float64 m_qualityLevel;
            runtime::Bool m_useHarrisDetector;
            runtime::Enum m_dataFlow;
            runtime::NumericParameter<runtime::UInt32>* m_blockSizeParameter;
            runtime::NumericParameter<runtime::Float64>* m_kParameter;
            runtime::NumericParameter<runtime::UInt32>* m_maxCornersParameter;
            runtime::NumericParameter<runtime::Float64>* m_minDistanceParameter;
            runtime::MatrixDescription* m_pointMatrixDescription;
            runtime::NumericParameter<runtime::Float64>* m_qualityLevelParameter;
            runtime::Description* m_srcDescription;
            runtime::Parameter* m_useHarrisDetectorParameter;
            runtime::EnumParameter* m_dataFlowParameter;
        };
    }
}

#endif // STROMX_CVIMGPROC_GOODFEATURESTOTRACK_H