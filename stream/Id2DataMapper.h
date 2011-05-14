#ifndef STREAM_ID2DATAMAPPER_H
#define STREAM_ID2DATAMAPPER_H

namespace stream
{
    class Id2DataMap; 
    
    class Id2DataMapper
    { 
    public:
        friend bool trySet(const Id2DataMapper& mapper, const Id2DataMap& id2DataMap);
        friend bool tryGet(const Id2DataMapper& mapper, const Id2DataMap& id2DataMap);
        friend void get(const Id2DataMapper& mapper, Id2DataMap& id2DataMap); 
        friend void set(const Id2DataMapper& mapper, Id2DataMap& id2DataMap); 
        
        virtual ~Id2DataMapper() {}
        
        virtual const bool trySet(const Id2DataMap& id2DataMap) const = 0;
        virtual const bool tryGet(const Id2DataMap& id2DataMap) const = 0;
        virtual void get(Id2DataMap& id2DataMap) const = 0; 
        virtual void set(Id2DataMap& id2DataMap) const = 0; 
    };

}

#endif // STREAM_ID2DATAMAPPER_H