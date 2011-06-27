#ifndef STREAM_NONE_H
#define STREAM_NONE_H

#include "Data.h"

namespace stream
{
    class None : public Data
    {
    public:
        None() {}
        
        virtual const DataType & type() const { return DataType::NONE; }
    };
}

#endif // STREAM_NONE_H
