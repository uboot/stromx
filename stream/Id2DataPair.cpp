#include "Id2DataPair.h"

#include "Exception.h"
#include <boost/assert.hpp>

namespace stream
{      
    
    bool trySet(Id2DataPair& mapper, const Id2DataMap& id2DataMap)
    {
        bool value;
        
        if(mapper.m_node)
            value = mapper.m_node->trySetTraverse(id2DataMap);
        else
            value = mapper.trySetTraverse(id2DataMap);
        
        mapper.clean();
        
        return value;
    }  
    
    bool tryGet(Id2DataPair& mapper, const Id2DataMap& id2DataMap)
    {
        bool value;
        
        if(mapper.m_node)
            value = mapper.m_node->tryGetTraverse(id2DataMap);
        else
            value = mapper.tryGetTraverse(id2DataMap);
        
        mapper.clean();
        
        return value;
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
    
    void Id2DataPair::clean()
    {
        delete m_node;
        m_node = 0;
        m_rhs = 0;
        m_lhs = 0;
        m_type = LEAF;
    }
    
    const bool Id2DataPair::tryGetTraverse(const Id2DataMap& id2DataMap) const
    {
        switch(m_type)
        {
        case AND:
            return m_lhs->tryGetTraverse(id2DataMap) && m_rhs->tryGetTraverse(id2DataMap);
        case OR:
            return m_lhs->tryGetTraverse(id2DataMap) || m_rhs->tryGetTraverse(id2DataMap);
        case LEAF:
            return id2DataMap[m_id] != 0;
        default:
            BOOST_ASSERT(false);
        }   
        
    }
    
    void get(Id2DataPair& mapper, Id2DataMap& id2DataMap)
    {
        try
        {
            if(mapper.m_node)
                mapper.m_node->getTraverse(id2DataMap);
            else
                mapper.getTraverse(id2DataMap);
            
            mapper.clean();
        }
        catch(InvalidStateException& e)
        {
            mapper.clean();
            throw e;
        }     
    }
    
    void Id2DataPair::getTraverse(stream::Id2DataMap& id2DataMap)
    {
        switch(m_type)
        {
        case AND:
            m_lhs->getTraverse(id2DataMap);
            m_rhs->getTraverse(id2DataMap);
            break;
        case OR:
            if(m_lhs->tryGetTraverse(id2DataMap))
                m_lhs->getTraverse(id2DataMap);
            else
                m_rhs->getTraverse(id2DataMap);
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
    
    void set(Id2DataPair& mapper, Id2DataMap& id2DataMap)
    {
        try
        {
            if(mapper.m_node)
                mapper.m_node->setTraverse(id2DataMap);
            else
                mapper.setTraverse(id2DataMap);
            
            mapper.clean();
        }
        catch(InvalidStateException& e)
        {
            mapper.clean();
            throw e;
        }
    }

    void Id2DataPair::setTraverse(stream::Id2DataMap& id2DataMap)
    {
        switch(m_type)
        {
        case AND:
            m_lhs->setTraverse(id2DataMap);
            m_rhs->setTraverse(id2DataMap);
            break;
        case OR:
            if(m_lhs->trySetTraverse(id2DataMap))
                m_lhs->setTraverse(id2DataMap);
            else
                m_rhs->setTraverse(id2DataMap);
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
        delete m_node;
        
        if(m_type != LEAF)
        {
            if(m_rhs->m_type != LEAF)
                delete m_rhs;
            
            if(m_lhs->m_type != LEAF)
                delete m_lhs;
        }
            
        m_node = 0;
        m_lhs = 0;
        m_rhs = 0;
        m_type = LEAF;          
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
