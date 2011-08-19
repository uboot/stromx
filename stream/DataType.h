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
        const static DataType DATA;
        const static DataType TRIGGER;
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
        const static DataType BGR_24_IMAGE;
        const static DataType BAYERBG_8_IMAGE;
        const static DataType BAYERGB_8_IMAGE;
        const static DataType MONO_16_IMAGE;
        const static DataType RGB_48_IMAGE;
        const static DataType BGR_48_IMAGE;
        
        explicit DataType(const unsigned int typeId, const std::string & description)
          : m_typeId(typeId),
            m_description(description)
        {}
        
        ~DataType() {}
        
        const std::string & description() const { return m_description; }
        
        virtual const bool is(const DataType& type) const;
       
    private: 
        enum Types
        {
            NONE_ID,
            DATA_ID,
            TRIGGER_ID,
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
            BGR_24_IMAGE_ID,
            BAYERBG_8_IMAGE_ID,
            BAYERGB_8_IMAGE_ID,
            MONO_16_IMAGE_ID,
            RGB_48_IMAGE_ID,
            BGR_48_IMAGE_ID
        };
        
        unsigned int m_typeId;
        std::string m_description;
    };
}

#endif // STREAM_DATATYPE_H