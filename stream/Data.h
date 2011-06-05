#ifndef STREAM_DATA_H
#define STREAM_DATA_H

#include "DataType.h"

namespace stream
{
    class Data
    {
    public:
        virtual const DataType & type() const = 0;
        virtual ~Data() {}
        
        bool is(const DataType & t) const { return type().is(t); }
        
    protected:
        Data() {} 
    };
}

#endif // STREAM_DATA_H