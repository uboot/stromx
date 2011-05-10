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
        
        explicit Id2DataComposite(Id2DataMapper& lhs, Id2DataMapper& rhs, const Type type)
          : m_lhs(lhs),
            m_rhs(rhs),
            m_type(type)
        {}
        
        Id2DataComposite(const Id2DataComposite& composite)
          : m_lhs(composite.m_lhs),
            m_rhs(composite.m_rhs),
            m_type(composite.m_type)
        {}
        
        virtual const bool trySet(const Id2DataMap& id2DataMap) const;
        virtual const bool tryGet(const Id2DataMap& id2DataMap) const;
        
        virtual void set(Id2DataMap& id2DataMap) const;
        virtual void get(const Id2DataMap& id2DataMap); 
    
    private:
        Id2DataMapper & m_lhs;
        Id2DataMapper & m_rhs;
        Type m_type;
    };
     
    Id2DataComposite operator&&(Id2DataMapper & lhs, Id2DataMapper & rhs);
    Id2DataComposite operator||(Id2DataMapper & lhs, Id2DataMapper & rhs);    
   
}

#endif // STREAM_ID2DATACOMPOSITE_H
