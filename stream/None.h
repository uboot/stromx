#ifndef STREAM_NONE_H
#define STREAM_NONE_H

#include "Data.h"

namespace stream
{
    class None : public Data
    {
    public:
        None() {}
        
        virtual const DataType & type() const { return m_type; }
        
    private:
        static const DataType m_type;
    };
}

#endif // STREAM_NONE_H
