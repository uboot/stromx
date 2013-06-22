#ifndef STROMX_CORE_CORE_H
#define STROMX_CORE_CORE_H

#include "stromx/core/Config.h"

namespace stromx
{
    namespace runtime
    {
        class Registry;
    }
}

extern "C"
{
    STROMX_CORE_API void stromxRegisterCore(stromx::runtime::Registry& registry);
}

#endif // STROMX_CORE_CORE_H
