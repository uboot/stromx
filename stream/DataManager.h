#ifndef STREAM_DATAMANAGER_H
#define STREAM_DATAMANAGER_H


namespace stream
{
    class DataContainerImpl;
    
    class DataManager
    {
    public:
        virtual void release(DataContainer* const data) = 0;
    };
}

#endif // STREAM_DATAMANAGER_H
