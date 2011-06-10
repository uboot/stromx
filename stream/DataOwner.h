#ifndef STREAM_DATAOWNER_H
#define STREAM_DATAOWNER_H


namespace stream
{
    class DataContainer;
    
    class DataOwner
    {
    public:
        virtual void release(DataContainer* const data) = 0;
    };
}

#endif // STREAM_DATAOWNER_H
