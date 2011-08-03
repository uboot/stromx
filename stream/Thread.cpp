#include "Thread.h"

#include "Exception.h"
#include "Network.h"
#include "InputNode.h"
#include "Operator.h"

namespace stream
{
    Thread::Thread(const stream::Network*const network)
      : m_network(network)
    {
        if(! network)
            throw ArgumentException("Passed null pointer as network.");
    }
        
    void Thread::addNode(const stream::Node& node)
    {
        InputNode* inputNode = m_network->getInputNode(node);
        m_thread.addNode(inputNode);
    }

    void Thread::insertNode(const unsigned int position, const stream::Node& node)
    {
        InputNode* inputNode = m_network->getInputNode(node);
        m_thread.insertNode(position, inputNode);
    }

    void Thread::removeNode(const unsigned int position)
    {
        m_thread.removeNode(position);
    }
} 
