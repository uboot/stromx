#include "Network.h"

#include "OperatorNode.h"

namespace stream
{
    Network::Network(const std::string & name)
        : m_name(name),
          m_status(INACTIVE)
    {
    }
    
    Network::~Network()
    {
        for(std::vector<OperatorNode*>::iterator iter = m_operators.begin();
            iter != m_operators.end();
            ++iter)
        {
            delete *iter;
        }

        m_operators.clear();
    }
    
    void Network::activate()
    {
    }

    void Network::deactivate()
    {
    }

    OperatorNode*const Network::addOperator(Operator*const op)
    {
    }

    void Network::removeOperator(OperatorNode*const op)
    {
    }
}