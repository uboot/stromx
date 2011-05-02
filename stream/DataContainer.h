#ifndef STREAM_DATACONTAINER_H
#define STREAM_DATACONTAINER_H

#include <typeinfo>
#include <boost/thread/mutex.hpp>

namespace stream
{
    class Data;
    class Operator;
    
    class DataContainer
    {
    public:
        DataContainer(const unsigned int id, const Operator* const owner, Data* const data);
        DataContainer(const unsigned int id, Data* const data);
        virtual ~DataContainer();
        
        const std::type_info & type() { return typeid(DataContainer); }
        const unsigned int id() { return m_id; }
        const Operator* const Owner() { return m_owner; }
        
        void reference();
        void dereference();
        const Data* const getReadAccess();
        Data* const obtainOwnership(const Operator* const owner);
        void releaseOwnership(const Operator* const owner);
            
    private:
        unsigned int m_id;
        const Operator* m_owner;
        Data* m_data;
        boost::mutex m_mutex;
        unsigned int m_refCount;
    };
    
   
}

#endif // STREAM_DATACONTAINER_H