#include "Id2DataComposite.h"
#include "Exception.h"

#include <boost/assert.hpp>

namespace stream
{
    const bool Id2DataComposite::tryGet(const stream::Id2DataMap& id2DataMap) const
    {
        switch(m_type)
        {
        case AND:
            return m_lhs.tryGet(id2DataMap) && m_rhs.tryGet(id2DataMap);
        case OR:
            return m_lhs.tryGet(id2DataMap) || m_rhs.tryGet(id2DataMap);
        default:
            BOOST_ASSERT(false);
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
            BOOST_ASSERT(false);
        }
    }
    
    void Id2DataComposite::get(const stream::Id2DataMap& id2DataMap)
    {
        BOOST_ASSERT(tryGet(id2DataMap));
        
        switch(m_type)
        {
        case AND:
            m_lhs.get(id2DataMap);
            m_rhs.get(id2DataMap);
            break;
        case OR:
            if(m_lhs.tryGet(id2DataMap))
                m_lhs.get(id2DataMap);
            else
                m_rhs.get(id2DataMap);
            break;
        default:
            BOOST_ASSERT(false);
        }
    }

    void Id2DataComposite::set(Id2DataMap& id2DataMap) const
    {
        
        BOOST_ASSERT(trySet(id2DataMap));
        
        switch(m_type)
        {
        case AND:
            m_lhs.set(id2DataMap);
            m_rhs.set(id2DataMap);
            break;
        case OR:
            if(m_lhs.trySet(id2DataMap))
                m_lhs.set(id2DataMap);
            else
                m_rhs.set(id2DataMap);
            break;
        default:
            BOOST_ASSERT(false);
        }
    }
    

    Id2DataComposite operator&&(Id2DataMapper & lhs, Id2DataMapper & rhs)
    {
        return Id2DataComposite(lhs, rhs, Id2DataComposite::AND);
    }
    
    Id2DataComposite operator||(Id2DataMapper & lhs, Id2DataMapper & rhs)
    {
        return Id2DataComposite(lhs, rhs, Id2DataComposite::OR);
    }

}