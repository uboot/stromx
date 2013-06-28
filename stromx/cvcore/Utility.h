#ifndef STROMX_CVCORE_UTILITY_H
#define STROMX_CVCORE_UTILITY_H

#include <stromx/runtime/Enum.h>
#include <stromx/runtime/OperatorKernel.h>
#include <stromx/runtime/OperatorException.h>
#include <stromx/runtime/NumericParameter.h>
#include <stromx/runtime/EnumParameter.h>

namespace stromx
{
    namespace cvcore
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
        
    }
}
#endif // STROMX_CVCORE_UTILITY_H
