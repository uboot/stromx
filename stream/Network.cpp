#include "Network.h"
#include "OperatorInterface.h"
#include "OperatorNode.h"
#include "OperatorWrapper.h"
#include "Exception.h"

namespace stream
{
    Network::Network()
        : m_status(INACTIVE)
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
	if (m_status == ACTIVE)
	{
	    throw InvalidStateException("Network already active");
	}
	
	for(std::vector<OperatorNode*>::iterator iter = m_operators.begin();
            iter != m_operators.end();
            ++iter)
        {
            (*iter)->op()->activate();
        }
        
	m_status = ACTIVE;
    }

    void Network::deactivate()
    {
	for(std::vector<OperatorNode*>::iterator iter = m_operators.begin();
            iter != m_operators.end();
            ++iter)
        {
            (*iter)->op()->deactivate();
        }
        
	m_status = INACTIVE;
    }

    OperatorNode*const Network::addOperator(Operator*const op)
    {
	for(std::vector<OperatorNode*>::iterator iter = m_operators.begin();
	    iter != m_operators.end();
	    ++iter)
	{
	    if ((*iter)->op()->info() == reinterpret_cast<OperatorInfo*>(op))
	    {
	      throw InvalidStateException("Operator already exists");
	      break;
	    }
	    
	    OperatorWrapper* wrapper = new OperatorWrapper(op);
            OperatorNode* node = new OperatorNode(wrapper);
            m_operators.push_back(node);
	    return node;
	}
    }

    void Network::removeOperator(OperatorNode*const op)
    {
	if (op == 0)
	{
	  throw ArgumentException("Invalid argument: Null pointer");
	}
	
	for(std::vector<OperatorNode*>::iterator iter = m_operators.begin();
	    iter != m_operators.end();
	    ++iter)
	{
	    if ((*iter)->op() == reinterpret_cast<OperatorInterface*>(op))
	    {
	      m_operators.erase(iter);
	      break;
	    }
	}
	
	throw ArgumentException("Operator does not exist");
    }
    
    OperatorNode* const Network::getOperator(const std::string & name)
    {
	for(std::vector<OperatorNode*>::iterator iter = m_operators.begin();
	    iter != m_operators.end();
	    ++iter)
	{
	  if ((*iter)->name() == name)
	  {
	    return *iter;
	  }
	}  
	
	throw InvalidStateException("Operator does not exist");
	return 0;
    }
}