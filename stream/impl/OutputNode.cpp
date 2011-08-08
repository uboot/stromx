#include "OutputNode.h"

#include "../Exception.h"
#include "../DataContainer.h"
#include "../Operator.h"

namespace stream
{
    OutputNode::OutputNode(Operator*const op, const unsigned int outputId)
      : m_operator(op),
        m_outputId(outputId),
        m_remainingCopies(0)
    {}
    
    void OutputNode::addConnectedInput(InputNode*const input)
    {
        if(! input)
            throw WrongArgument("Passed null as input.");
        
        if(m_connectedInputs.count(input))
            throw WrongArgument("Input node has already been connected to this output node.");
        
        m_connectedInputs.insert(input);
    }

    void OutputNode::removeConnectedInput(stream::InputNode*const input)
    {
        if(m_connectedInputs.count(input))
            m_connectedInputs.erase(input);
    }

    DataContainer OutputNode::getOutputData()
    {
        DataContainer value = m_operator->getOutputData(m_outputId);
        
        // the data has been obtained
        // now make sure the connection counter is adapted in an atomic operation
        {
            lock_t lock(m_mutex);
            
            if(! m_remainingCopies)
            {
                m_operator->clearOutputData(m_outputId);
                m_remainingCopies = m_connectedInputs.size();
            }
            
            m_remainingCopies--; 
        }
        
        return value;
    }

}
