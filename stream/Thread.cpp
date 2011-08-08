#include "Thread.h"

#include "Exception.h"
#include "impl/Network.h"
#include "Operator.h"

#include "impl/InputNode.h"

namespace stream
{
    Thread::Thread(const stream::Network*const network)
      : m_network(network)
    {
        if(! network)
            throw InvalidArgument("Passed null pointer as network.");
    }
        
    void Thread::addNode(Operator* const op, const unsigned int inputId)
    {
        InputNode* inputNode = m_network->getInputNode(op, inputId);
        m_thread.addNode(inputNode);
        m_nodeSequence.push_back(Node(op, inputId));
    }

    void Thread::insertNode(const unsigned int position, Operator* const op, const unsigned int inputId)
    {
        InputNode* inputNode = m_network->getInputNode(op, inputId);
        m_thread.insertNode(position, inputNode);
        m_nodeSequence.insert(m_nodeSequence.begin() + position, Node(op, inputId));
    }

    void Thread::removeNode(const unsigned int position)
    {
        m_thread.removeNode(position);
        m_nodeSequence.erase(m_nodeSequence.begin() + position);
    }
} 
