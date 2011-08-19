#include "Primitive.h"

#include <float.h>

namespace stream
{
    template<>
    const DataType & Bool::type() const { return DataType::BOOL; }
    template<>
    const std::string Bool::NAME = "Bool";
    template<>
    const std::string Bool::PACKAGE = PACKAGE_NAME;
    template<>
    const Version Bool::VERSION = Version(STREAM_VERSION_MAJOR, STREAM_VERSION_MINOR);
    
    template<>
    const DataType & Int8::type() const { return DataType::INT_8; }
    template<>
    const std::string Int8::NAME = "Int8";
    template<>
    const std::string Int8::PACKAGE = PACKAGE_NAME;
    template<>
    const Version Int8::VERSION = Version(STREAM_VERSION_MAJOR, STREAM_VERSION_MINOR);
    
    template<>
    const DataType & UInt8::type() const { return DataType::UINT_8; }
    template<>
    const std::string UInt8::NAME = "UInt8";
    template<>
    const std::string UInt8::PACKAGE = PACKAGE_NAME;
    template<>
    const Version UInt8::VERSION = Version(STREAM_VERSION_MAJOR, STREAM_VERSION_MINOR);
    
    template<>
    const DataType & Int16::type() const { return DataType::INT_16; }
    template<>
    const std::string Int16::NAME = "Int16";
    template<>
    const std::string Int16::PACKAGE = PACKAGE_NAME;
    template<>
    const Version Int16::VERSION = Version(STREAM_VERSION_MAJOR, STREAM_VERSION_MINOR);
    
    template<>
    const DataType & UInt16::type() const { return DataType::UINT_16; }
    template<>
    const std::string UInt16::NAME = "UInt16";
    template<>
    const std::string UInt16::PACKAGE = PACKAGE_NAME;
    template<>
    const Version UInt16::VERSION = Version(STREAM_VERSION_MAJOR, STREAM_VERSION_MINOR);
    
    template<>
    const DataType & Int32::type() const { return DataType::INT_32; }
    template<>
    const std::string Int32::NAME = "Int32";
    template<>
    const Int32 Int32::MIN = Int32(INT32_MIN);
    template<>
    const Int32 Int32::MAX = Int32(INT32_MAX);
    template<>
    const std::string Int32::PACKAGE = PACKAGE_NAME;
    template<>
    const Version Int32::VERSION = Version(STREAM_VERSION_MAJOR, STREAM_VERSION_MINOR);
    
    template<>
    const DataType & UInt32::type() const { return DataType::UINT_32; }
    template<>
    const std::string UInt32::NAME = "UInt32";
    template<>
    const UInt32 UInt32::MIN = UInt32(0);
    template<>
    const UInt32 UInt32::MAX = UInt32(UINT32_MAX);
    template<>
    const std::string UInt32::PACKAGE = PACKAGE_NAME;
    template<>
    const Version UInt32::VERSION = Version(STREAM_VERSION_MAJOR, STREAM_VERSION_MINOR);
    
    template<>
    const DataType & Float::type() const { return DataType::FLOAT; }
    template<>
    const std::string Float::NAME = "Float";
    template<>
    const Float Float::MIN = Float(- FLT_MAX);
    template<>
    const Float Float::MAX = Float(FLT_MAX);
    template<>
    const std::string Float::PACKAGE = PACKAGE_NAME;
    template<>
    const Version Float::VERSION = Version(STREAM_VERSION_MAJOR, STREAM_VERSION_MINOR);
    
    template<>
    const DataType & Double::type() const { return DataType::DOUBLE; }
    template<>
    const std::string Double::NAME = "Double";
    template<>
    const Double Double::MIN = Double(- DBL_MAX);
    template<>
    const Double Double::MAX = Double(DBL_MAX);
    template<>
    const std::string Double::PACKAGE = PACKAGE_NAME;
    template<>
    const Version Double::VERSION = Version(STREAM_VERSION_MAJOR, STREAM_VERSION_MINOR);
}
