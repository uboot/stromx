#include "Id2DataPair.h"

#include "Exception.h"
#include <boost/assert.hpp>

namespace stream
{      
    const bool Id2DataPair::trySet(const Id2DataMap& id2DataMap) const
    {
        if(m_node)
            return m_node->trySetTraverse(id2DataMap);
        else
            return trySetTraverse(id2DataMap);      
    }
    
    const bool Id2DataPair::trySetTraverse(const Id2DataMap& id2DataMap) const
    {
        switch(m_type)
        {
        case AND:
            return m_lhs->trySetTraverse(id2DataMap) && m_rhs->trySetTraverse(id2DataMap);
        case OR:
            return m_lhs->trySetTraverse(id2DataMap) || m_rhs->trySetTraverse(id2DataMap);
        case LEAF:
            return id2DataMap[m_id] == 0;
        default:
            BOOST_ASSERT(false);
        }        
    }
    
    const bool Id2DataPair::tryGet(const Id2DataMap& id2DataMap) const
    {
        if(m_node)
            return m_node->tryGet(id2DataMap);
        
        switch(m_type)
        {
        case AND:
            return m_lhs->tryGet(id2DataMap) && m_rhs->tryGet(id2DataMap);
        case OR:
            return m_lhs->tryGet(id2DataMap) || m_rhs->tryGet(id2DataMap);
        case LEAF:
            return id2DataMap[m_id] != 0;
        default:
            BOOST_ASSERT(false);
        }   
        
    }
    
    void Id2DataPair::get(stream::Id2DataMap& id2DataMap)
    {
        if(m_node)
            return m_node->get(id2DataMap);
        
        switch(m_type)
        {
        case AND:
            m_lhs->get(id2DataMap);
            m_rhs->get(id2DataMap);
            break;
        case OR:
            if(m_lhs->tryGet(id2DataMap))
                m_lhs->get(id2DataMap);
            else
                m_rhs->get(id2DataMap);
            break;
        case LEAF:
            if(m_data != 0)
                throw InvalidStateException("Data has already been assigned to this ID-data pair");
                
            if(id2DataMap[m_id] == 0)
                throw InvalidStateException("The requested output contains 0");
            
            m_data = id2DataMap[m_id];
            id2DataMap[m_id] = 0;
            break;
        default:
            BOOST_ASSERT(false);
        }  
    }

    void Id2DataPair::set(stream::Id2DataMap& id2DataMap)
    {
        if(m_node)
            return m_node->set(id2DataMap);
        
        switch(m_type)
        {
        case AND:
            m_lhs->set(id2DataMap);
            m_rhs->set(id2DataMap);
            break;
        case OR:
            if(m_lhs->tryGet(id2DataMap))
                m_lhs->set(id2DataMap);
            else
                m_rhs->set(id2DataMap);
            break;
        case LEAF:
            if(m_data == 0)
                throw InvalidStateException("This ID-data pair contains no data");
            
            if(id2DataMap[m_id] != 0)
                throw InvalidStateException("Data has already been assigned to this input ID");
            
            id2DataMap[m_id] = m_data;
            m_data = 0;
            break;
        default:
            BOOST_ASSERT(false);
        }
        
    }
    
    Id2DataPair::~Id2DataPair()
    {
        if(m_node)
            delete m_node;
        
        if(m_type != LEAF)
        {
            if(m_rhs->m_type != LEAF)
                delete m_rhs;
            
            if(m_lhs->m_type != LEAF)
                delete m_lhs;
        }
    }
    
    Id2DataPair & Id2DataPair::createNode(Id2DataPair & lhs, Id2DataPair & rhs, const NodeType type)
    {
        Id2DataPair* unpackedLhs = lhs.m_node ? lhs.m_node : &lhs;
        Id2DataPair* unpackedRhs = rhs.m_node ? rhs.m_node : &rhs;
        
        lhs.m_node = new Id2DataPair(unpackedLhs, unpackedRhs, type);
        rhs.m_node = 0;
        
        return lhs;
    }
    
}
