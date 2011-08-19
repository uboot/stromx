#include "Data.h"

#include "Exception.h"

namespace stream
{
    const std::string Data::serialize(const std::string& name, const std::string& path) const
    {
        throw SerializationError(*this, name, path);
    }

    void Data::deserialize(const std::string& data, const std::string& path)
    {
        throw DeserializationError(*this, data, path);
    }
}
