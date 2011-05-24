#ifndef STREAM_DESCRIPTOR_H
#define STREAM_DESCRIPTOR_H

#include "DataType.h"
#include <string>

namespace stream
{
    class Description
    {
    public:
        Description(const std::string & name, const unsigned int id, const DataType & type)
          : m_name(name),
            m_id(id),
            m_type(type)
        {}
        
        const std::string & name() const { return m_name; }
        const unsigned int id() const { return m_id; }
        const DataType & type() const { return m_type; }
        
    private:
        std::string m_name;
        unsigned int m_id;
        DataType m_type;
    };
}

#endif // STREAM_DESCRIPTOR_H