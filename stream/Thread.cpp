#include "Thread.h"

#include "Exception.h"
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

    }

    void Thread::insertNode(const unsigned int position, const stream::Node& node)
    {

    }

    void Thread::removeNode(const stream::Node& node)
    {

    }
} 
