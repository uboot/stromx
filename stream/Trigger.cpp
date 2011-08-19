#include "Trigger.h"

#include "Config.h"

namespace stream
{
    const std::string Trigger::NAME = "Trigger";
    const std::string Trigger::PACKAGE = PACKAGE_NAME;
    const Version Trigger::VERSION = Version(STREAM_VERSION_MAJOR, STREAM_VERSION_MINOR);
}