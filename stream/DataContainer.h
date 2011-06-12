#ifndef STREAM_DATACONTAINERIMPL_H
#define STREAM_DATACONTAINERIMPL_H

#include <typeinfo>
#include <boost/thread/mutex.hpp>
#include <boost/thread/condition_variable.hpp>

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
        typedef boost::lock_guard<boost::mutex> lock_t;
        typedef boost::unique_lock<boost::mutex> unique_lock_t;
        
        boost::mutex m_mutex;
        boost::condition_variable_any m_cond;
        Data* m_data;
        DataOwner* m_owner;
        unsigned int m_refCount;
        bool m_hasWriteAccess;
    };     
}

#endif // STREAM_DATACONTAINERIMPL_H
