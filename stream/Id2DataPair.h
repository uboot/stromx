#ifndef STREAM_ID2DATAPAIR_H
#define STREAM_ID2DATAPAIR_H

#include "Id2DataMapper.h"
#include "DataContainer.h"

namespace stream
{  
    class Id2DataMap;
    
    class Id2DataPair : public Id2DataMapper
    {
    public:
        Id2DataPair(const unsigned int id, DataContainer data)
          : m_id(id),
            m_data(new DataContainer())
        {
            *m_data = data;
        }
        
        Id2DataPair(const unsigned int id)
          : m_id(id),
            m_data(new DataContainer())
        {}
        
        ~Id2DataPair()
        {
            delete m_data;
        }
        
        const unsigned int id() const { return m_id; }
        DataContainer & data() const { return *m_data; }
        
        virtual const bool trySet(const Id2DataMap& id2DataMap) const;
        virtual const bool tryGet(const Id2DataMap& id2DataMap) const;
        virtual void get(Id2DataMap& id2DataMap) const; 
        virtual void set(Id2DataMap& id2DataMap) const;     
    
    private:
        unsigned int m_id;
        DataContainer* m_data;
    }; 
    
   
}

#endif // STREAM_ID2DATAPAIR_H
