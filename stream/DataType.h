#ifndef STREAM_DATATYPE_H
#define STREAM_DATATYPE_H

#include "Version.h"

#include <string>

namespace stream
{
    class DataType
    {
    public:
        const static DataType NONE;
        const static DataType BOOL;
        const static DataType ENUM;
        const static DataType INT;
        const static DataType UINT;
        const static DataType INT_8;
        const static DataType UINT_8;
        const static DataType INT_16;
        const static DataType UINT_16;
        const static DataType INT_32;
        const static DataType UINT_32;
        const static DataType FLOAT;
        const static DataType DOUBLE;
        const static DataType IMAGE;
        const static DataType MONO_IMAGE;
        const static DataType RGB_IMAGE;
        const static DataType MONO_8_IMAGE;
        const static DataType RGB_24_IMAGE;
        const static DataType MONO_16_IMAGE;
        const static DataType RGB_48_IMAGE;
        
        explicit DataType(const unsigned int typeId, const Version& version, const std::string & package, const std::string & name)
          : m_typeId(typeId),
            m_version(version),
            m_package(package),
            m_name(name)
        {}
        
        ~DataType() {}
        
        virtual const bool is(const DataType& type) const;
       
    private: 
        enum Types
        {
            NONE_ID,
            BOOL_ID,
            ENUM_ID,
            INT_ID,
            UINT_ID,
            INT_8_ID,
            UINT_8_ID,
            INT_16_ID,
            UINT_16_ID,
            INT_32_ID,
            UINT_32_ID,
            FLOAT_ID,
            DOUBLE_ID,
            IMAGE_ID,
            MONO_IMAGE_ID,
            RGB_IMAGE_ID,
            MONO_8_IMAGE_ID,
            RGB_24_IMAGE_ID,
            MONO_16_IMAGE_ID,
            RGB_48_IMAGE_ID
        };
        
        unsigned int m_typeId;
        std::string m_name;
        std::string m_package;
        Version m_version;
    };
}

#endif // STREAM_DATATYPE_H