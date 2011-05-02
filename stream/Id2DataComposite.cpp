#include "Id2DataComposite.h"
#include "Exception.h"

namespace stream
{
    const bool Id2DataComposite::tryGet(const stream::Id2ConstDataMap& id2DataMap) const
    {
        switch(m_type)
        {
        case AND:
            return m_lhs.tryGet(id2DataMap) && m_rhs.tryGet(id2DataMap);
        case OR:
            return m_lhs.tryGet(id2DataMap) || m_rhs.tryGet(id2DataMap);
        default:
            throw InternalException("Unhandled composite type");
        }
    }
    
    const bool Id2DataComposite::trySet(const stream::Id2DataMap& id2DataMap) const
    {
        switch(m_type)
        {
        case AND:
            return m_lhs.trySet(id2DataMap) && m_rhs.trySet(id2DataMap);
        case OR:
            return m_lhs.trySet(id2DataMap) || m_rhs.trySet(id2DataMap);
        default:
            throw InternalException("Unhandled composite type");
        }
    }
    
    void Id2DataComposite::get(const stream::Id2ConstDataMap& id2DataMap)
    {
        // TODO
    }

    void Id2DataComposite::set(Id2DataMap& id2DataMap) const
    {
        // TODO
    }



}