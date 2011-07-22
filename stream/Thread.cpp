#include "Thread.h"

#include "Exception.h"
#include "InputNode.h"
#include "OperatorInterface.h"
#include "OperatorInfo.h"

#include <boost/bind.hpp>

namespace stream
{
    Thread::Thread()
      : m_thread(0),
        m_status(INACTIVE)
    {
    }
    
    Thread::~Thread()
    {
        stop();
    }
    
    void Thread::addNode(InputNode* const op)
    {
        if(m_status != INACTIVE)
            throw WrongState("Thread must be inactive.");
        
        if(! op)
            throw ArgumentException("Passed null as input node.");
        
        m_nodeSequence.push_back(op);
    }

    void Thread::insertNode(const unsigned int position, InputNode* const op)
    {
        if(m_status != INACTIVE)
            throw WrongState("Thread must be inactive.");
        
        if(position > m_nodeSequence.size())
            throw ArgumentException("Can only insert at an existing position of at the end of the node sequence.");
        
        m_nodeSequence.insert(m_nodeSequence.begin() + position, op);
    }

    void Thread::removeNode(const unsigned int position)
    {
        if(m_status != INACTIVE)
            throw WrongState("Thread must be inactive.");
        
        if(position >= m_nodeSequence.size())
            throw ArgumentException("This position is occupied in the node sequence of this thread.");
        
        m_nodeSequence.erase(m_nodeSequence.begin() + position);
    }

    void Thread::start()
    {
        if(m_status != INACTIVE)
            throw WrongState("Thread must be inactive.");
        
        BOOST_ASSERT(! m_thread);
        
        m_thread = new boost::thread(boost::bind(&Thread::loop, this));
        
        m_status = ACTIVE;
    }

    void Thread::stop()
    {
        if(m_status != ACTIVE)
            return;
        
        BOOST_ASSERT(m_thread);
        
        m_thread->interrupt();
        
        m_status = DEACTIVATING;
    }

    void Thread::join()
    {
        if(m_status != DEACTIVATING)
            throw WrongState("Thread must have been stopped.");
        
        BOOST_ASSERT(m_thread);
        
        m_thread->join();
        
        m_thread = 0;
        
        m_status = INACTIVE;
    }
    
    void Thread::loop()
    {
        try
        {      
            while(true)
            {
                for(std::vector<InputNode*>::iterator node = m_nodeSequence.begin();
                        node != m_nodeSequence.end();
                        ++node)
                {
                    (*node)->setInputData();
                    // TODO: handle exceptions in setInputData(), i.e. in Operator::execute()
                    
                    try
                    {
                        boost::this_thread::interruption_point();
                    }
                    catch(boost::thread_interrupted&)
                    {
                        throw InterruptException();
                    } 
                }
            }
        }
        catch(InterruptException&)
        {
        }
    }
}
