#ifndef STROMX_IMGPROC_IMGPROC_H
#define STROMX_IMGPROC_IMGPROC_H

#include "stromx/imgproc/Config.h"

namespace stromx
{
    namespace runtime
    {
        class Registry;
    }
}

extern "C"
{
    STROMX_IMGPROC_API void stromxRegisterImgproc(stromx::runtime::Registry& registry);
}

#endif // STROMX_IMGPROC_IMGPROC_H
