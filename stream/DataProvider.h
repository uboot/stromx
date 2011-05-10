#ifndef STREAM_DATAPROVIDER_H
#define STREAM_DATAPROVIDER_H



namespace stream
{
    class DataContainer;
    
    class DataProvider
    {
    public:
        virtual ~DataProvider() {}
        
        virtual const bool isStopped() = 0;
        
        virtual void receiveInputData() = 0;
        virtual void sendOutputData() = 0;
    };
}

#endif // STREAM_DATAPROVIDER_H