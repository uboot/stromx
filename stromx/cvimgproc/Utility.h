#ifndef STROMX_CVIMGPROC_UTILITY_H
#define STROMX_CVIMGPROC_UTILITY_H

#include <stromx/runtime/NumericParameter.h>
#include <stromx/runtime/Enum.h>
#include <stromx/runtime/EnumParameter.h>
#include <stromx/runtime/Matrix.h>
#include <stromx/runtime/OperatorException.h>
#include <opencv2/core/core.hpp>
#include <stromx/runtime/MatrixParameter.h>
#include <stromx/runtime/MatrixDescription.h>
#include <stromx/runtime/OperatorKernel.h>

namespace stromx
{
    namespace cvimgproc
    {
        void checkEnumValue(const stromx::runtime::Enum & value, const stromx::runtime::EnumParameter* param, const stromx::runtime::OperatorKernel& op);
        
        template<class T>
        void checkNumericValue(const T & value, const runtime::NumericParameter<T>* param, const stromx::runtime::OperatorKernel& op)
        {
            if(value < runtime::data_cast<T>(param->min()))
                throw runtime::WrongParameterValue(*param, op);
            if(value > runtime::data_cast<T>(param->max()))
                throw runtime::WrongParameterValue(*param, op);
        }
        
        void checkMatrixData(const stromx::runtime::Matrix & value,
                             const stromx::runtime::MatrixDescription* desc,
                             const stromx::runtime::OperatorKernel& op);
        
        void checkMatrixValue(const stromx::runtime::Matrix & value,
                              const stromx::runtime::MatrixParameter* param,
                              const stromx::runtime::OperatorKernel& op);
        
        void calcHist1D(const cv::Mat & input, cv::Mat & result, const float min, const float max, int size);
        
    }
}
#endif // STROMX_CVIMGPROC_UTILITY_H
