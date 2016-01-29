#ifndef STROMX_CVHIGHGUI_CVHIGHGUI_H
#define STROMX_CVHIGHGUI_CVHIGHGUI_H

#include "stromx/cvhighgui/Config.h"

namespace stromx
{
    namespace runtime
    {
        class Registry;
    }
}

extern "C"
{
    STROMX_CVHIGHGUI_API int stromxRegisterCvhighgui(stromx::runtime::Registry* registry);
}

#endif // STROMX_CVHIGHGUI_CVHIGHGUI_H
