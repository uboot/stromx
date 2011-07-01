#ifndef STREAM_TESTDATA_H
#define STREAM_TESTDATA_H

#include <stream/Data.h>

namespace stream
{
    class TestData : public Data
    {
    public:
        TestData() { wasDestructed = false; }
        ~TestData() { wasDestructed = true; }
        
        virtual const DataType & type() const { return DataType::NONE; }
        
        static bool wasDestructed;
    };
}

#endif // STREAM_TESTDATA_H
