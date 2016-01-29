#ifndef STROMX_CVIMGPROC_CVIMGPROC_H
#define STROMX_CVIMGPROC_CVIMGPROC_H

#include "stromx/cvimgproc/Config.h"

namespace stromx
{
    namespace runtime
    {
        class Registry;
    }
}

extern "C"
{
    STROMX_CVIMGPROC_API int stromxRegisterCvimgproc(stromx::runtime::Registry* registry);
}

#endif // STROMX_CVIMGPROC_CVIMGPROC_H
