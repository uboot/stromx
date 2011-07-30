#include "Id2DataComposite.h"

#include <boost/assert.hpp>

namespace stream
{
    const bool Id2DataComposite::trySet(const impl::Id2DataMap& id2DataMap) const
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
    
    const bool Id2DataComposite::tryGet(const impl::Id2DataMap& id2DataMap) const
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
    
    void Id2DataComposite::get(impl::Id2DataMap& id2DataMap) const
    {
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
    
    void Id2DataComposite::set(impl::Id2DataMap& id2DataMap) const
    {
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
    
    const Id2DataComposite operator&&(const Id2DataMapper & lhs, const Id2DataMapper & rhs)
    {
        return Id2DataComposite(lhs, rhs, Id2DataComposite::AND);
    }
    
    const Id2DataComposite operator||(const Id2DataMapper & lhs, const Id2DataMapper & rhs)  
    {
        return Id2DataComposite(lhs, rhs, Id2DataComposite::OR);
    }
}
