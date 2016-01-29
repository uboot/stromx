#ifndef STROMX_CVOBJDETECT_CVOBJDETECT_H
#define STROMX_CVOBJDETECT_CVOBJDETECT_H

#include "stromx/cvobjdetect/Config.h"

namespace stromx
{
    namespace runtime
    {
        class Registry;
    }
}

extern "C"
{
    STROMX_CVOBJDETECT_API int stromxRegisterCvobjdetect(stromx::runtime::Registry* registry);
}

#endif // STROMX_CVOBJDETECT_CVOBJDETECT_H
