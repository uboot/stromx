#ifndef STREAM_DESCRIPTOR_H
#define STREAM_DESCRIPTOR_H

#include "DataType.h"
#include <string>

namespace stream
{
    class Description
    {
    public:
        Description(const unsigned int id)
          : m_id(id)
        {}
        
        const std::string & name() const { return m_name; }
        const unsigned int id() const { return m_id; }
        const DataType & type() const { return m_type; }
        
        void setName(const std::string & name) { m_name = name; }
        void setType(const DataType& type) { m_type = type; }
        
    private:
        std::string m_name;
        unsigned int m_id;
        DataType m_type;
    };
}

#endif // STREAM_DESCRIPTOR_H