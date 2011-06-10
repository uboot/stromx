#ifndef STREAM_DATACONTAINERIMPL_H
#define STREAM_DATACONTAINERIMPL_H

#include <typeinfo>
#include <boost/thread/mutex.hpp>

#include "Data.h"

namespace stream
{
    class DataOwner;
    class DataType;
    
    class DataContainer
    {
    public:
        DataContainer(Data* const data, DataOwner* const owner = 0);
        ~DataContainer();
        
        const DataType & type() { return m_data->type(); }
        
        void reference();
        void dereference();

        const Data* const getReadAccess();
        Data* const getWriteAccess();
        void clearWriteAccess();
            
    private:
        Data* m_data;
        DataOwner* m_owner;
        boost::mutex m_mutex;
        unsigned int m_refCount;
        bool m_hasWriteAccess;
    };     
}

#endif // STREAM_DATACONTAINERIMPL_H
