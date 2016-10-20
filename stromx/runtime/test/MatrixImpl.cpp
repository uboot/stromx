#include "stromx/runtime/test/MatrixImpl.h"

#include "stromx/runtime/Version.h"

namespace stromx
{
    namespace runtime
    {
        const std::string MatrixImpl::TYPE = "MatrixImpl";
        const std::string MatrixImpl::PACKAGE = "Test";
        const Version MatrixImpl::VERSION = Version(STROMX_RUNTIME_VERSION_MAJOR, STROMX_RUNTIME_VERSION_MINOR, STROMX_RUNTIME_VERSION_PATCH);
    }
}
