#include "Utility.h"

#include <stromx/runtime/OperatorException.h>

namespace stromx
{
    namespace core
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
        
    }
}
