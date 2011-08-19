#ifndef STREAM_DATA_H
#define STREAM_DATA_H

#include "DataType.h"

namespace stream
{
    class Version;
    
    class Data
    {
    public:
        virtual ~Data() {}
        
        virtual const Version & version() const = 0;
        virtual const std::string & name() const = 0;
        virtual const std::string & package() const = 0;
        
        virtual const DataType & type() const = 0;
        
        virtual const std::string serialize(const std::string & name, const std::string & path) const;
        virtual void deserialize(const std::string & data,
                                 const std::string & name, const std::string & path);
        
        bool is(const DataType & t) const { return type().is(t); }
        
    protected:
        Data() {} 
    };
}

#endif // STREAM_DATA_H