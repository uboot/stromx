#include "TestData.h"

#include "Config.h"

namespace stream
{
    bool TestData::wasDestructed = false;
    
    const std::string TestData::NAME = "TestData";
    const std::string TestData::PACKAGE = "TestPackage";
    const Version TestData::VERSION = Version(0, 1);
}