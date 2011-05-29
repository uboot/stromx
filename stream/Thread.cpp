#include "Thread.h"

#include "Exception.h"
#include "InputNode.h"
#include "OperatorNode.h"
#include "OperatorInterface.h"

#include <boost/bind.hpp>

namespace stream
{
    Thread::Thread(const std::string& name)
      : m_thread(0),
        m_name(name)
    {
    }
    
    Thread::~Thread()
    {
        stop();
    }
    
    void Thread::addOperator(OperatorNode*const op)
    {
        if(! op)
            throw ArgumentException("Passed null as operator node.");
        
        m_operatorSequence.push_back(op);
    }

    void Thread::insertOperator(const unsigned int position, OperatorNode*const op)
    {
        if(position > m_operatorSequence.size())
            throw ArgumentException("Can only insert at an existing position of at the end of the operator sequence.");
        
        m_operatorSequence.insert(m_operatorSequence.begin() + position, op);
    }

    void Thread::removeOperator(const unsigned int position)
    {
        if(position >= m_operatorSequence.size())
            throw ArgumentException("This position is occupied in the operator sequence of this thread.");
        
        m_operatorSequence.erase(m_operatorSequence.begin() + position);
    }

    void Thread::start()
    {
        if(m_thread)
            throw InvalidStateException("Can not start thread because it is active.");
        
        m_thread = new boost::thread(boost::bind(&Thread::loop, this));
    }

    void Thread::stop()
    {
        if(! m_thread)
            return;
        
        m_thread->interrupt();
    }

    void Thread::join()
    {
        if(! m_thread)
            return;
        
        m_thread->join();
        
        m_thread = 0;
    }
    
    void Thread::loop()
    {
        try
        {      
            while(true)
            {
                for(std::vector<OperatorNode*>::iterator operatorNode = m_operatorSequence.begin();
                        operatorNode != m_operatorSequence.end();
                        ++operatorNode)
                {
                    for(std::vector<Description>::const_iterator input = (*operatorNode)->op()->inputs().begin();
                        input != (*operatorNode)->op()->inputs().end();
                        ++input)
                    {
                        InputNode* inputNode = (*operatorNode)->getInputNode(input->id());
                        
                        inputNode->setInputData();
                        // TODO: handle exceptions in setInputData(), i.e. in the operator
                        
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
        }
        catch(InterruptException&)
        {
        }
    }
}
