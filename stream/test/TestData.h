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
        
        virtual const Version & version() const { return VERSION; }
        virtual const std::string & name() const { return NAME; }
        virtual const std::string & package() const { return PACKAGE; }
        
        virtual const DataType & type() const { return DataType::NONE; }
        
        virtual Data* const clone() const { return new TestData; }
        
        static bool wasDestructed;
        
    private:
        static const std::string NAME;
        static const std::string PACKAGE;
        static const Version VERSION;
    };
}

#endif // STREAM_TESTDATA_H
