#ifndef STREAM_DATAPROVIDER_H
#define STREAM_DATAPROVIDER_H



namespace stream
{
    class DataContainer;
    class Id2DataPair;
    
    class DataProvider
    {
    public:
        virtual ~DataProvider() {}
        
        virtual const bool isDeactivating() = 0;
        
        virtual void receiveInputData(Id2DataPair& mapper) = 0;
        virtual void sendOutputData(Id2DataPair& mapper) = 0;
    };
}

#endif // STREAM_DATAPROVIDER_H