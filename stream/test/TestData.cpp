#include "TestData.h"

#include "Config.h"

namespace stream
{
    bool TestData::wasDestructed = false;
    
    const std::string TestData::NAME = "TestData";
    const std::string TestData::PACKAGE = PACKAGE_NAME;
    const Version TestData::VERSION = Version(STREAM_VERSION_MAJOR, STREAM_VERSION_MINOR);
}