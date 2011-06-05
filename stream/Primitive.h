#ifndef STREAM_PRIMITIVE_H
#define STREAM_PRIMITIVE_H

#include "Description.h"
#include "Data.h"

#include <stdint.h>
#include <float.h>

namespace stream
{
    template <class repr_t, class val_t>
    class Primitive : public Data
    {
    public:
        explicit Primitive() : m_value(0) {}
        explicit Primitive(repr_t value) : m_value(value) {}
        
        virtual const DataType & type() const { return m_type; }
        
        operator repr_t() const { return m_value; }
        
    private:
        val_t m_value;
        static const DataType m_type;
    };
    
    typedef Primitive<bool, bool> Bool;
    
    typedef Primitive<unsigned int, uint32_t> Enum;
        
    typedef Primitive<int, int8_t> Int8;
    typedef Primitive<unsigned int, uint8_t> UInt8;
        
    typedef Primitive<int, int16_t> Int16;
    typedef Primitive<unsigned int, uint16_t> UInt16;
    
    typedef Primitive<int, int32_t> Int32;
    typedef Primitive<unsigned int, uint32_t> UInt32;
    
    typedef Primitive<double, float> Float;
    typedef Primitive<double, double> Double;
}

#endif // STREAM_PRIMITIVE_H