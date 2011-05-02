#ifndef STREAM_ID2DATAMAPPER_H
#define STREAM_ID2DATAMAPPER_H

#include <map>

namespace stream
{
    class DataContainer;
   
    typedef std::map<unsigned int, DataContainer*> Id2DataMap;
    typedef std::map<unsigned int, DataContainer* const> Id2ConstDataMap;
    
    class Id2DataMapper
    {
    public:
        virtual const bool trySet(const Id2DataMap& id2DataMap) const = 0;
        virtual const bool tryGet(const Id2ConstDataMap& id2DataMap) const = 0;
        
        virtual void set(Id2DataMap& id2dataMap) const = 0;
        virtual void get(const Id2ConstDataMap& id2dataMap) = 0; 
    };
    
   
}

#endif // STREAM_ID2DATAMAPPER_H