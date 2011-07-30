#ifndef STREAM_ID2DATACOMPOSITE_H
#define STREAM_ID2DATACOMPOSITE_H

#include "Id2DataMapper.h"

namespace stream
{
    class Id2DataComposite : public Id2DataMapper
    {
    private:
        enum NodeType
        {
            AND,
            OR
        };
        
    public:
        friend const Id2DataComposite operator&&(const Id2DataMapper & lhs, const Id2DataMapper & rhs);
        friend const Id2DataComposite operator||(const Id2DataMapper & lhs, const Id2DataMapper & rhs);
        
        Id2DataComposite(const Id2DataMapper& lhs, const Id2DataMapper& rhs, const NodeType type)
          : m_lhs(lhs),
            m_rhs(rhs),
            m_type(type)
        {}
        
        virtual const bool trySet(const impl::Id2DataMap& id2DataMap) const;
        virtual const bool tryGet(const impl::Id2DataMap& id2DataMap) const;
        virtual void get(impl::Id2DataMap& id2DataMap) const; 
        virtual void set(impl::Id2DataMap& id2DataMap) const;
        
    private:
        const Id2DataMapper& m_lhs;
        const Id2DataMapper& m_rhs;
        NodeType m_type;
    };
    
    const Id2DataComposite operator&&(const Id2DataMapper & lhs, const Id2DataMapper & rhs);
    const Id2DataComposite operator||(const Id2DataMapper & lhs, const Id2DataMapper & rhs);

}

#endif // STREAM_ID2DATACOMPOSITE_H