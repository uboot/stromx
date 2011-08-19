#include "None.h"

#include "Config.h"

namespace stream
{
    const std::string None::NAME = "None";
    const std::string None::PACKAGE = PACKAGE_NAME;
    const Version None::VERSION = Version(STREAM_VERSION_MAJOR, STREAM_VERSION_MINOR);
}