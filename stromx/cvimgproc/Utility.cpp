#include "Utility.h"

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
        
    }
}
