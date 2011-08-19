#ifndef STREAM_NONE_H
#define STREAM_NONE_H

#include "Data.h"

namespace stream
{
    class None : public Data
    {
    public:
        None() {}
        
        virtual const Version & version() const { return VERSION; }
        virtual const std::string & name() const { return NAME; }
        virtual const std::string & package() const { return PACKAGE; }
        
        virtual const DataType & type() const { return DataType::NONE; }
        
        virtual Data* const clone() const { return new None; }
        
    private:
        static const std::string NAME;
        static const std::string PACKAGE;
        static const Version VERSION;
    };
}

#endif // STREAM_NONE_H
