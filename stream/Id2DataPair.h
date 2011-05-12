#ifndef STREAM_ID2DATAPAIR_H
#define STREAM_ID2DATAPAIR_H

#include "Id2DataMapper.h"

namespace stream
{  
    class Id2DataPair : public Id2DataMapper
    {
    private:
        enum NodeType
        {
            LEAF,
            AND,
            OR
        };
        
    public:
        friend Id2DataPair & operator&&(Id2DataPair & lhs, Id2DataPair & rhs)
        {
            createNode(lhs, rhs, AND);
        }
        
        friend Id2DataPair & operator||(Id2DataPair & lhs, Id2DataPair & rhs)  
        {
            createNode(lhs, rhs, OR);
        }
    
        Id2DataPair(const unsigned int id, DataContainer* data)
          : m_id(id),
            m_data(data),
            m_rhs(0),
            m_lhs(0),
            m_node(),
            m_type(LEAF)
        {}
        
        Id2DataPair(const unsigned int id)
          : m_id(id),
            m_data(0),
            m_rhs(0),
            m_lhs(0),
            m_node(),
            m_type(LEAF)
        {}
        
        Id2DataPair(Id2DataPair* const lhs, Id2DataPair* const rhs, const NodeType type)
          : m_id(0),
            m_data(0),
            m_rhs(lhs),
            m_lhs(rhs),
            m_node(0),
            m_type(type)
        {}
        
        ~Id2DataPair();
        
        const unsigned int id() const { return m_id; }
        DataContainer* const data() const { return m_data; }
        
        const bool trySet(const Id2DataMap& id2DataMap) const;
        const bool tryGet(const Id2DataMap& id2DataMap) const;
        
        void set(Id2DataMap& id2DataMap);
        void get(Id2DataMap& id2DataMap); 
    
    private:
        static Id2DataPair & createNode(Id2DataPair & lhs, Id2DataPair & rhs, const NodeType type);
        
        const bool trySetTraverse(const Id2DataMap& id2DataMap) const;
        
        unsigned int m_id;
        DataContainer* m_data;
        
        Id2DataPair* m_rhs;
        Id2DataPair* m_lhs;
        Id2DataPair* m_node;
        
        NodeType m_type;
    }; 
    
   
}

#endif // STREAM_ID2DATAPAIR_H
