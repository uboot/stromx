#include "Utility.h"

#include <opencv2/imgproc/imgproc.hpp>
#include <sstream>
#include <stromx/runtime/OperatorException.h>

namespace stromx
{
    namespace cvimgproc
    {
        void checkEnumValue(const stromx::runtime::Enum & value, const stromx::runtime::EnumParameter* param, const stromx::runtime::OperatorKernel& op)
        {
            using namespace runtime;
            
            for(std::vector<EnumDescription>::const_iterator iter = param->descriptions().begin(); iter != param->descriptions().end(); ++iter)
            {
                 if(value == iter->value())
                return;
            }
            throw stromx::runtime::WrongParameterValue(*param, op);
        }
        
        void checkMatrixData(const stromx::runtime::Matrix & value,
                             const stromx::runtime::MatrixDescription* desc,
                             const stromx::runtime::OperatorKernel& op)
        {
            if(desc->rows() && value.rows() != desc->rows())
            {
                std::ostringstream str;
                str << desc->rows();
                throw runtime::InputError(desc->id(), op, "Number of matrix rows must be " + str.str() + " .");
            }
            
            if(desc->cols() && value.cols() != desc->cols())
            {
                std::ostringstream str;
                str << desc->cols();
                throw runtime::InputError(desc->id(), op, "Number of matrix columns must be " + str.str() + " .");
            }
        }
        
        void checkMatrixValue(const stromx::runtime::Matrix & value,
                              const stromx::runtime::MatrixParameter* param,
                              const stromx::runtime::OperatorKernel& op)
        {
            if(param->rows() && value.rows() != param->rows())
            {
                std::ostringstream str;
                str << param->rows();
                throw runtime::WrongParameterValue(*param, op, "Number of matrix rows must be " + str.str() + " .");
            }
            
            if(param->cols() && value.cols() != param->cols())
            {
                std::ostringstream str;
                str << param->cols();
                throw runtime::WrongParameterValue(*param, op, "Number of matrix columns must be " + str.str() + " .");
            }
        }
        
        void calcHist1D(const cv::Mat & input, cv::Mat & result, const float min, const float max, int size)
        {
            int channels[] = {0};
            float range[] = {min, max};
            const float* ranges[] = {range};
            
            cv::calcHist(&input, 1, channels, cv::Mat(), result, 1, &size, ranges);
        }
        
    }
}
