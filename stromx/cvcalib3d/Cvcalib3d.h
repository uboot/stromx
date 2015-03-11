#ifndef STROMX_CVCALIB3D_CVCALIB3D_H
#define STROMX_CVCALIB3D_CVCALIB3D_H

#include "stromx/cvcalib3d/Config.h"

namespace stromx
{
    namespace runtime
    {
        class Registry;
    }
}

extern "C"
{
    STROMX_CVCALIB3D_API void stromxCvcalib3dRegister(stromx::runtime::Registry& registry);
}

#endif // STROMX_CVCALIB3D_CVCALIB3D_H
