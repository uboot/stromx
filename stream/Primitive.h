#ifndef STREAM_PRIMITIVE_H
#define STREAM_PRIMITIVE_H

#include "Description.h"
#include "Data.h"

#include <stdint.h>

namespace stream
{
    template <class repr_t, class val_t>
    class Primitive : public Data
    {
    public:
        explicit Primitive() : m_value(0) {}
        explicit Primitive(repr_t value) : m_value(value) {}
        
        virtual const DataType & type() const { return m_type; }
        
        operator repr_t() { return m_value; }
        
    private:
        val_t m_value;
        static const DataType m_type;
    };
    
    template <class repr_t, class val_t>
    const DataType Primitive<repr_t, val_t>::m_type;
        
    typedef Primitive<int, int32_t> Int32;
    typedef Primitive<unsigned int, uint32_t> UInt32;
}

#endif // STREAM_PRIMITIVE_H