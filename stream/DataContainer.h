#ifndef STREAM_DATACONTAINERIMPL_H
#define STREAM_DATACONTAINERIMPL_H

#include <typeinfo>
#include <boost/thread/mutex.hpp>

namespace stream
{
    class DataManager;
    class Data;
    
    class DataContainer
    {
    public:
        DataContainer(Data* const data, DataManager* const manager = 0);
        
        const std::type_info & type() { return typeid(DataContainer); }
        
        void reference();
        void dereference();

        const Data* const getReadAccess();
        Data* const getWriteAccess();
        void clearWriteAccess();
            
    private:
        Data* m_data;
        DataManager* m_manager;
        boost::mutex m_mutex;
        unsigned int m_refCount;
        bool m_hasWriteAccess;
    };     
}

#endif // STREAM_DATACONTAINERIMPL_H
