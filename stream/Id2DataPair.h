#ifndef STREAM_ID2DATAPAIR_H
#define STREAM_ID2DATAPAIR_H

#include "Id2DataMapper.h"

namespace stream
{  
    class Id2DataPair : public Id2DataMapper
    {
    public:
        
        Id2DataPair(const unsigned int id, DataContainer* const data);
        
        Id2DataPair(const unsigned int id)
          : m_id(id),
            m_data(0)
        {}
        
        virtual const bool trySet(const Id2DataMap& id2DataMap) const;
        virtual const bool tryGet(const Id2DataMap& id2DataMap) const;
        
        virtual void set(Id2DataMap& id2DataMap) const;
        virtual void get(const Id2DataMap& id2DataMap); 
    
    private:
        unsigned int m_id;
        DataContainer* m_data;
    };
    
   
}

#endif // STREAM_ID2DATAPAIR_H
