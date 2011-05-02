#ifndef STREAM_ID2DATACOMPOSITE_H
#define STREAM_ID2DATACOMPOSITE_H

#include "Id2DataMapper.h"

namespace stream
{  
    class Id2DataComposite : public Id2DataMapper
    {
    public:
        enum Type
        {
            AND,
            OR
        };
        
        // TODO: must also (type safe) accept non-const arguments
        Id2DataComposite(const Id2DataMapper& lhs, const Id2DataMapper& rhs, const Type type)
          : m_lhs(lhs),
            m_rhs(rhs),
            m_type(type)
        {}
        
        virtual const bool trySet(const Id2DataMap& id2DataMap) const;
        virtual const bool tryGet(const Id2ConstDataMap& id2DataMap) const;
        
        virtual void set(Id2DataMap& id2DataMap) const;
        virtual void get(const Id2ConstDataMap& id2DataMap); 
    
    private:
        const Id2DataMapper & m_lhs;
        const Id2DataMapper & m_rhs;
        Type m_type;
    };
    
   
}

#endif // STREAM_ID2DATACOMPOSITE_H
