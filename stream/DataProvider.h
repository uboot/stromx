#ifndef STREAM_DATAPROVIDER_H
#define STREAM_DATAPROVIDER_H

namespace stream
{
    class DataContainer;
    class Id2DataMapper;
    class Data;
    
    class DataProvider
    {
    public:
        virtual ~DataProvider() {}
        
        virtual void testForInterrupt() = 0;
        
        virtual void receiveInputData(const Id2DataMapper& mapper) = 0;
        virtual void sendOutputData(const Id2DataMapper& mapper) = 0;
    };
}

#endif // STREAM_DATAPROVIDER_H